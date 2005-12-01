// Copyright (c)2005 Jean-Baptiste Lab (jean-baptiste dot lab at laposte dot net)
// See the file copying.txt for copying permission.

#if !defined DIRECTORYENUM_H
#define DIRECTORYENUM_H

#include <sys/types.h>
#include <dirent.h>

#include "FileInfo.h"

class DirectoryEnum
{
 public:
  DirectoryEnum();
  ~DirectoryEnum();
  FileInfo *findFirst( const char * );
  FileInfo *findNext( void );
 private:
  DIR  *m_dirHnd;
  char *m_pPath;
  FileInfo m_curEnt;
  FileInfo m_curInfo;
  void reset( void );
};

#endif // DIRECTORYENUM_H
