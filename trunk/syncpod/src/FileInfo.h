// Copyright (c)2005 Jean-Baptiste Lab (jean-baptiste dot lab at laposte dot net)
// See the file copying.txt for copying permission.

#if !defined FILEINFO_H
#define FILEINFO_H

/**
 * @brief Determines whether the given character is a (back)slash.
 */
#define IS_SLASH(x) ( '\\' == (x) || '/'  == (x) )

/**
 * @brief File information wrapper object.
 *
 * File information wrapper object.
 * Contains information about a given file on the file system.
 */
class FileInfo
{
 public:
  FileInfo();
  ~FileInfo();
  /**
   * @brief Return the file name of this instance.
   * @return file name, no path information
   */  
  const char *getName();
  /**
   * @brief Return the access time of this instance.
   * @return access time in seconds since the epoch
   */  
  int   getAccessTime();
  /**
   * @brief Return the modification time of this instance.
   * @return modification time in seconds since the epoch
   */  
  int   getModificationTime();
  /**
   * @brief Return the creation time of this instance.
   * @return creation time in seconds since the epoch
   */  
  int   getCreationTime();
  /**
   * @brief Determine whether this instance is a directory.
   * @return true if instance is a directory
   */  
  bool  isDir();
  /**
   * @brief Update this instance with information for the given file name.
   * @param pName [in] name of file/directory for which to retrieve info
   * @return true info could be retrieved
   */  
  bool  getInfo( const char *pName );
 private:
  char *m_pName;
  int   m_accessTime;
  int   m_modificationTime;
  int   m_creationTime;
  bool  m_isDir;
  void  reset();
};

#endif // FILEINFO_H
