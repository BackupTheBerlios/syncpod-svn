// Copyright (c)2005 Jean-Baptiste Lab (jean-baptiste dot lab at laposte dot net)
// See the file copying.txt for copying permission.

#if defined HAVE_RLOG
#include <rlog/rlog.h>
#else
#include "no_rlog.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
// Mess to get mkdir's prototype
#if defined __MINGW32__
// Mingw defines it here
#include <io.h>
#elif defined __BORLANDC__
// Borland defines it here
#include <dir.h>
#else
// Posix defines it here
#include <sys/stat.h>
#endif

#if defined HAVE_NVWA
#include <nvwa/debug_new.h>
#endif

#include "PodSync.h"
#include "ConfigFile.h"

PodSync::PodSync()
{
  m_pConfig = 0;
}

PodSync::~PodSync()
{  
  delete m_pConfig;
}

const char *PodSync::basename( const char *pPath )
{
  const char *pBasename = strrchr( pPath, '/' );
  // Backslash maybe ?
  if( 0 == pBasename )
  {
    pBasename = strrchr( pPath, '\\' );
  }
  if( 0 != pBasename )
  {
    pBasename++;
  }
  else
  {
    pBasename = pPath;
  }
  return pBasename;
}

bool PodSync::makeDir( const char *pPath )
{
#if defined __MINGW32__ || defined __BORLANDC__
  if( 0 != mkdir( pPath ))
#else
  if( 0 != mkdir( pPath, 0700 ))
#endif
  {
#if defined __BORLANDC__
    // Borland returns EACCES if the dir exists...
    if( EACCES != errno && EEXIST != errno )
#else
    if( EEXIST != errno )
#endif
    {
      return false;
    }
  }
  return true;
}

bool PodSync::copy( const char *pSrc, const char *pDst )
{
  FILE *dst;
  FILE *src;
  src = fopen( pSrc, "rb" );
  if( 0 == src  )
  {
    rError( "Could not open source: %s", pSrc );
    return false;
  }
  dst = fopen( pDst, "wb" );
  if( 0 == dst  )
  {
    rError( "Could not open destination: %s", pDst );
    fclose( src );
    return false;
  }
  char *buffer = new char[102400];
  unsigned int bytesRead;
  unsigned int bytesWritten;
  do
  {
    bytesRead = fread( buffer, 1, 102400, src );
    bytesWritten = fwrite( buffer, 1, bytesRead, dst );
    if( bytesRead != bytesWritten )
    {
      rError( "Writing %s failed (read %d, written %d)", pDst, bytesRead, bytesWritten );
      perror( 0 );
      fclose( dst );
      fclose( src);
      delete[] buffer;
      return false;
    }
  } while( 0 == feof( src ));
  fclose( dst );
  fclose( src);
  delete[] buffer;
  return true;
}

bool PodSync::processOneItem( FileInfo *pSrcInfo, FileInfo *pDstInfo )
{
  FileInfo info;
  // Assume things are ok
  bool result = true;
  const char *srcFileName;
  srcFileName = basename( pSrcInfo->getName() );
  
  // + 2 for the path separator and the final 0
  char *dstFileName = new char[ strlen( pDstInfo->getName() ) + strlen( srcFileName ) + 2 ];
  strcpy( dstFileName, pDstInfo->getName() );
  strcat( dstFileName, "/" );
  strcat( dstFileName, srcFileName );
  rInfo( "Processing %s", pSrcInfo->getName() );

  // Ok, destination does not exist, copy
  if( false == info.getInfo( dstFileName ))
  {
    if( false == pSrcInfo->isDir() )
    {
      rInfo( "Copying %s to %s as it does not exist", pSrcInfo->getName(), dstFileName ) ;
      result = copy( pSrcInfo->getName(), dstFileName );
    }
    else
    {
      // Current item in src is a dir, recurse
      const char *srcBaseName = basename( pSrcInfo->getName() );
      char *pNewDst = new char[ strlen( pDstInfo->getName() ) + strlen( srcBaseName ) + 2 ];
      strcpy( pNewDst, pDstInfo->getName() );
      strcat( pNewDst, "/" );
      strcat( pNewDst, srcBaseName );
      performSync( pSrcInfo->getName(), pNewDst );
      delete[] pNewDst;
    }
  }
  else
  {
    // Destination is older that source, copy
    if( info.getModificationTime() < pSrcInfo->getModificationTime() )
    {
      rInfo( "Copying %s to %s as it is newer", pSrcInfo->getName(), dstFileName ) ;
      result = copy( pSrcInfo->getName(), dstFileName );
    }
    else
    {
      rInfo( "Skipping %s as %s is up-to-date", pSrcInfo->getName(), dstFileName ) ;
    }
  }

  delete[] dstFileName;

  return result;
}

bool PodSync::makeDirs( const char *pPath )
{
  if( 0 == pPath )
  {
    return false;
  }
  
  char *pIndex = new char[strlen( pPath ) + 1];
  char *pStart = pIndex;
  memcpy( pIndex, pPath, strlen( pPath ) + 1 );

  while ( 0 != *pIndex )
  {
    while( 0 != *pIndex && !IS_SLASH( *pIndex ))
    {
      pIndex++;
    }
    if( 0 != *pIndex )
    {
      *pIndex = 0;
      if( false == makeDir( pStart ))
      {
        delete[] pStart;
        return false;
      }
      *pIndex = '/';
      pIndex++;
    }
  }
  // The +1 here takes care of attempting to mkdir '/' (which fails)
  if( pIndex != pStart && pIndex != ( pStart + 1 ))
  {
    if( false == makeDir( pStart ))
    {
      delete[] pStart;
      return false;
    }
  }
  delete[] pStart;
  return true;
}

bool PodSync::performSync( const char *pSrc, const char *pDst )
{
  DirectoryEnum srcEnumerator;
  DirectoryEnum dstEnumerator;
  FileInfo srcInfo;
  FileInfo dstInfo;
  FileInfo *pSrcInfo = &srcInfo;
  bool result = false;

  // Check that source is valid
  if( false == srcInfo.getInfo( pSrc ))
  {
    rWarning( "Could not get info: %s", pSrc );
    return false;
  }
  // Check that destination is valid
  if( false == dstInfo.getInfo( pDst ))
  {
    rWarning( "Could not get info: %s", pDst );
    if( false == makeDirs( pDst ))
    {
      rError( "Could not create destination: %s", pDst );
      return false;
    }
    else
    {
      if( false == dstInfo.getInfo( pDst ))
      {
        rError( "Could not get info: %s", pDst );
        return false;
      }
    }
  }
  
  // Check that destination is a directory
  if( false == dstInfo.isDir() )
  {
    rError( "%s is not a directory", pDst );
    return false;
  }
  rInfo( "Source: %s", srcInfo.getName() );
  if( true == srcInfo.isDir() )
  {
    rInfo( "Enumerating %s", srcInfo.getName() );
    // Skip . and ..
    srcEnumerator.findFirst( pSrc );
    srcEnumerator.findNext();
    pSrcInfo = srcEnumerator.findNext();
    while( 0 != pSrcInfo )
    {
      result = processOneItem( pSrcInfo, &dstInfo );
      if( false == result )
      {
        rError( "Sync %s failed", pSrcInfo->getName() );
      }
      pSrcInfo = srcEnumerator.findNext();
    }
  }
  else
  {
    result = processOneItem( pSrcInfo, &dstInfo );
  }
  return result;
}

bool PodSync::doSync( void )
{
  bool result = true;
  const char *pDst = m_pConfig->getString( "destination_directory" );
  const char *pSrc = m_pConfig->getString( "source_directory" );
  if( 0 != pDst && 0 != pSrc )
  {
    rInfo( "Performing sync from %s to %s", pSrc, pDst );
    if( true == performSync( pSrc, pDst ))
    {
      rInfo( "Sync completed" );
    }
    else
    {  
      rWarning( "Sync failed" );
    }
  }
  else
  {
    rWarning( "Bad config" );
    result = false;
  }
  return result;
}


bool PodSync::OnDriveInserted( char drive )
{
  rInfo( "Media detected %c", drive );
  return doSync();
}

void PodSync::setConfig( const char *pConfig )
{
  if( 0 != m_pConfig )
  {
    delete m_pConfig;
  }
  m_pConfig = new ConfigFile( pConfig );
}
