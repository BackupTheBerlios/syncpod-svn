// Copyright (c)2005 Jean-Baptiste Lab (jean-baptiste dot lab at laposte dot net)
// See the file copying.txt for copying permission.

#if !defined FILEINFO_HXX
#define FILEINFO_HXX

#define IS_SLASH(x) ( '\\' == (x) || '/'  == (x) )

class FileInfo
{
 public:
  FileInfo();
  ~FileInfo();
  const char *getName();
  void  setName( const char * );
  int   getAccessTime();
  int   getModificationTime();
  int   getCreationTime();
  bool  getIsDir();
  bool  getInfo( const char * );
 private:
  char *m_pName;
  int   m_accessTime;
  int   m_modificationTime;
  int   m_creationTime;
  bool  m_isDir;
  void  reset();
};

#endif // FILEINFO_HXX  typedef struct
