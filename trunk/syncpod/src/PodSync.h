// Copyright (c)2005 Jean-Baptiste Lab (jean-baptiste dot lab at laposte dot net)
// See the file copying.txt for copying permission.

#if !defined PODSYNC_H
#define PODSYNC_H

#include "DirectoryEnum.h"

class ConfigFile;

class PodSync
{
 private:
  ConfigFile *m_pConfig;
#if defined (UNIT_TESTS)
 public:
#endif
  bool performSync( const char *, const char * );
  bool copy( const char *, const char * );
  bool makeDirs( const char * );
  bool processOneItem( FileInfo *, FileInfo * );
 public:
  PodSync();
  ~PodSync();
  void doSync( void );
  void OnDriveInserted( char drive );
};

#endif // PODSYNC_H
