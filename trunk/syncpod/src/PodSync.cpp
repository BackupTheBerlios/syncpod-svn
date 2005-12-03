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
#if defined __CYGWIN32__
/* */
#include <sys/stat.h>
#endif

#if defined HAVE_NVWA
#include <nvwa/debug_new.h>
#endif

#include "PodSync.h"
#include "ConfigFile.h"

PodSync::PodSync()
{
  m_pConfig = new ConfigFile( "syncPod.cfg" );
}

PodSync::~PodSync()
{
  delete m_pConfig;
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
  DirectoryEnum enumerator;
  // Assume things are ok
  bool result = true;
  const char *srcFileName;
  srcFileName = strrchr( pSrcInfo->getName(), '/' );
  // Backslash maybe ?
  if( 0 == srcFileName )
  {
    srcFileName = strrchr( pSrcInfo->getName(), '\\' );
  }
  if( 0 != srcFileName )
  {
    srcFileName++;
  }
  else
  {
    srcFileName = pSrcInfo->getName();
  }
  
  // + 2 for the path separator and the final 0
  char *dstFileName = new char[ strlen( pDstInfo->getName() ) + strlen( srcFileName ) + 2 ];
  strcpy( dstFileName, pDstInfo->getName() );
  strcat( dstFileName, "/" );
  strcat( dstFileName, srcFileName );
  rInfo( "Processing %s", pSrcInfo->getName() );

  // Ok, destination does not exist, copy
  if( false == info.getInfo( dstFileName ))
  {
    rInfo( "Copying %s to %s as it does not exist", pSrcInfo->getName(), dstFileName ) ;
    result = copy( pSrcInfo->getName(), dstFileName );
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

#if defined _WIN32
  if( ':'  == pIndex[1] &&
      IS_SLASH( pIndex[2] ))
  {
    pIndex += 3;
  }
#endif
  while ( 0 != *pIndex )
  {
    while( 0 != *pIndex && !IS_SLASH( *pIndex ))
    {
      pIndex++;
    }
    if( 0 != *pIndex )
    {
      *pIndex = 0;
#if defined __CYGWIN32__
      if( 0 != mkdir( pStart, 0700 ))
#else
      if( 0 != mkdir( pStart ))
#endif
      {
        if( EEXIST != errno )
        {
          rWarning( "Unknown error while mkdir: %s", pStart );
          delete[] pStart;
          return false;
        }
      }
      *pIndex = '/';
      pIndex++;
    }
  }
  // The +1 here takes care of attempting to mkdir '/' (which fails)
  if( pIndex != pStart && pIndex != ( pStart + 1 ))
  {
#if defined __CYGWIN32__
      if( 0 != mkdir( pStart, 0700 ))
#else
      if( 0 != mkdir( pStart ))
#endif
    {
      if( EEXIST != errno )
      {
        rWarning( "Unknow error while mkdir: %s", pStart );
        delete[] pStart;
        return false;
      }
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
  if( false == dstInfo.getIsDir() )
  {
    rError( "%s is not a directory", pDst );
    return false;
  }
  rInfo( "Source: %s", srcInfo.getName() );
  if( true == srcInfo.getIsDir() )
  {
    rInfo( "Enumerating %s", srcInfo.getName() );
    // Skip . and ..
    pSrcInfo = srcEnumerator.findFirst( pSrc );
    pSrcInfo = srcEnumerator.findNext();
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

void PodSync::doSync( void )
{
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
  }
}


void PodSync::OnDriveInserted( char drive )
{
  rInfo( "Media detected %c", drive );
  doSync();
}
