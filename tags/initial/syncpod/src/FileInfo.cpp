// Copyright (c)2005 Jean-Baptiste Lab (jean-baptiste dot lab at laposte dot net)
// See the file copying.txt for copying permission.

#include <string.h>
#include <sys/stat.h>

#if defined HAVE_RLOG
#include <rlog/rlog.h>
#else
#include "no_rlog.h"
#endif

#if defined HAVE_NVWA
#include <nvwa/debug_new.h>
#endif

#include "FileInfo.h"

FileInfo::FileInfo()
{
  m_pName            = 0;
  m_accessTime       = 0;
  m_modificationTime = 0;
  m_creationTime     = 0;
  m_isDir            = false;
}

FileInfo::~FileInfo()
{
  reset();
}

void FileInfo::reset()
{
  if( 0 != m_pName )
  {
    delete[] m_pName;
  }
//   memset( this, 0, sizeof( this ));
}

bool FileInfo::getInfo( const char *pName )
{
  struct stat statBuf;
  reset();
  if( 0 == stat( pName, &statBuf ))
  {
    m_accessTime       = statBuf.st_atime;
    m_modificationTime = statBuf.st_mtime;
    m_creationTime     = statBuf.st_ctime;
    m_isDir            = (statBuf.st_mode & S_IFDIR)?true:false;
    m_pName            = new char[ strlen( pName ) + 1];
    strcpy( m_pName, pName );
    return true;
  }
  else
  {
    rWarning( "stat failed: %s", pName );
    return false;
  }
}

const char *FileInfo::getName()
{
  return m_pName;
}

void FileInfo::setName( const char *pName )
{
  reset();
  m_pName = new char[ strlen( pName ) + 1];
  strcpy( m_pName, pName );
}

int FileInfo::getAccessTime()
{
  return m_accessTime;
}

int FileInfo::getModificationTime()
{
  return m_modificationTime;
}

int FileInfo::getCreationTime()
{
  return m_creationTime;
}

bool FileInfo::getIsDir()
{
  return m_isDir;
}
