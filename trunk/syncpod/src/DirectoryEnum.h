// Copyright (c)2005 Jean-Baptiste Lab (jean-baptiste dot lab at laposte dot net)
// See the file copying.txt for copying permission.

#if !defined DIRECTORYENUM_H
#define DIRECTORYENUM_H

#include <sys/types.h>
#include <dirent.h>

#include "FileInfo.h"

/**
 * @brief Simple directory enumeration class.
 * @note This is not thread-safe, use different objects!
 */
class DirectoryEnum
{
 public:
  /**
   * @brief Default constructor
   *
   * Construct a directory enumeration object.
   */
  DirectoryEnum();
  /**
   * @brief Destructor
   *
   * Destroy a directory enumeration object, freeing any allocating memory.
   */  
  ~DirectoryEnum();
  /**
   * @brief Find the first entry of the given path.
   * Find the first entry of the given path.
   * The first entry returned will be ".".
   * @param pPath [in] path to enumerate
   * @return pointer to a FileInfo object, NULL if pPath cannot be enumerated
   * @sa FileInfo
   */
  FileInfo *findFirst( const char *pPath );
  /**
   * @brief Find the next entry of the given enumeration object.
   * Finds the next entry of the given enumeration object.
   * The first entry returned will be ".."
   * @return pointer to a FileInfo object, NULL if pPath cannot be enumerated
   * or end of enumeration.
   * @sa FileInfo
   */
  FileInfo *findNext( void );
 private:
  /** OS handle to directory */
  DIR  *m_dirHnd; 
  /** Initial path of this enumeration object */
  char *m_pPath;
  /** Holds current enumerated item informtion */
  FileInfo m_curEnt;
  /**
   * @brief Reset state information of this object.
   *
   * This method will be called in the findFirst method to
   * clear any previous enumeration information.
   */
  void reset( void );
};

#endif // DIRECTORYENUM_H
