// Copyright (c)2005 Jean-Baptiste Lab (jean-baptiste dot lab at laposte dot net)
// See the file copying.txt for copying permission.

#if defined HAVE_RLOG
#include <rlog/rlog.h>
#else
#include "no_rlog.h"
#endif
#include <sys/stat.h>
#include <string.h>

#if defined HAVE_NVWA
#include <nvwa/debug_new.h>
#endif

#include "DirectoryEnum.h"

DirectoryEnum::DirectoryEnum()
{  
  m_dirHnd = 0;
  m_pPath  = 0;
}

DirectoryEnum::~DirectoryEnum()
{
  reset();
}

void DirectoryEnum::reset()
{
  if( 0 != m_dirHnd )
  {
    closedir( m_dirHnd );
    m_dirHnd = 0;
  }
  if( 0 != m_pPath )
  {
    delete[] m_pPath;
    m_pPath = 0;
  }
}

FileInfo* DirectoryEnum::findFirst( const char *pPath )
{
  reset();
  m_dirHnd = opendir( pPath );
  if( 0 != m_dirHnd )
  {
    m_pPath = new char[ strlen( pPath ) + 1 ];
    strcpy( m_pPath, pPath );
    // Reuse code...
    return findNext();
  }
  else
  {
    rError( "Could not open directory: %s", pPath );
  }
  return 0;
}

FileInfo* DirectoryEnum::findNext( void )
{
  if( 0 != m_dirHnd )
  {
    struct dirent *pDirEnt;
    pDirEnt = readdir( m_dirHnd );
    if( 0 != pDirEnt )
    {
      // +2 for 0 and '/'
      char *dirAndFileName = new char[ strlen( m_pPath ) + strlen( pDirEnt->d_name ) + 2 ];
      strcpy( dirAndFileName, m_pPath );
      strcat( dirAndFileName, "/" );
      strcat( dirAndFileName, pDirEnt->d_name );
      rInfo( "Stating: %s", dirAndFileName );
      if( true == m_curEnt.getInfo( dirAndFileName ))
      {
        delete[] dirAndFileName;
        return &m_curEnt;
      }
      delete[] dirAndFileName;
    }
    else
    {
      reset();
    }
  }
  return 0;
}
