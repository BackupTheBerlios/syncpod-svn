// Copyright (c)2005 Jean-Baptiste Lab (jean-baptiste dot lab at laposte dot net)
// See the file copying.txt for copying permission.

#if !defined CONFIGFILE_H
#define CONFIGFILE_H

class ConfigFile
{
 private:
  char *m_name;
  // @todo Use another (more efficient) data structure class to store these
  typedef struct
  {
    char *key;
    char *value;
  } ConfigData;
  ConfigData *m_data;
  int m_numKeys;
 public:
  ConfigFile();
  ConfigFile( const char *pFile );
  ~ConfigFile();
  const char *getString( const char *pKey );
  void setString( const char *pKey, const char *pValue );
  void setName( const char *pFile );
  bool write();
  bool write( const char *pFile );
  bool reload();
};

#endif // CONFIGFILE_H
