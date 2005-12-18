// Copyright (c)2005 Jean-Baptiste Lab (jean-baptiste dot lab at laposte dot net)
// See the file copying.txt for copying permission.

#if !defined PODSYNC_H
#define PODSYNC_H

#include "DirectoryEnum.h"

class ConfigFile;

/** 
 * @mainpage syncpod documentation
 *
 * @section introduction Introduction
 *
 * Syncpod is a small tool that I used to synchronize my MP3 player with my podcast
 * subscriptions that I download automatically on my computer.
 * 
 * A program on my PC checks for podcast updates and download them to a folder.
 * Because my MP3 player isn't always connected to my computer and because I don't
 * want to spend time manually checking whether my podcasts on the player are up-to-date,
 * I wrote this tool so that every time I plug my MP3 player, it will do that for me.
 *
 * @section development Development
 *
 * syncpod is developped using Emacs, mingw on Windows XP.
 * The code is compiled with several falvours of GCC (mingw and cygwin) using the '-ansi'
 * parameter.
 * The free Borland compiler is also used regularly to build syncpod.
 *
 * @section dependencies Dependencies
 *
 * @section installation Installation
 *
 * @section license License
 * 
 * syncpod is released under the MIT License:
 * <pre>
 * Copyright (c)2005 Jean-Baptiste Lab (jean-baptiste dot lab at laposte dot net)
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * </pre>
 * 
 */
/** 
 * @brief Main class performing the synchronization work.
 */
class PodSync
{
 private:
  //! Config object
  ConfigFile *m_pConfig;
#if defined (UNIT_TESTS)
 public:
#endif
  /**
   * @brief Perform synch between src and dst.
   *
   * Synchronizes the pDst path from the pSrc one.
   * Synchronization is based on the modification time of
   * the pDst and pSrc files if they both exist, if a pDst file is
   * missing, it will be copied.
   * @param pSrc [in] path to synch from
   * @param pDst [in] path to sync to
   * @return true if sync sucessful
   * @todo Make performSync return value less Manichean
   */
  bool performSync( const char *pSrc, const char *pDst );
  /**
   * @brief Copy pSrc to pDst
   * @param pSrc [in] source file name to copy
   * @param pDst [in] detination file name
   * @return true if copy was successful.
   */
  bool copy( const char *pSrc, const char *pDst );
  /**
   * @brief Create a path.
   * 
   * Creates a directory. The missing component of the path will be created.
   * @param pPath [in] path name to create
   * @return true if the creation was successful.
   */  
  bool makeDirs( const char *pPath );
  /**
   * @brief Process a synchronization item.
   * 
   * @param pSrc [in]
   * @param pDst [in]
   * @return true if the synchro was successful.
   */  
  bool processOneItem( FileInfo *pSrc, FileInfo *pDst );
  /**
   * @brief Return the basename of a path.
   * 
   * @param pPath [in] path to extract the basename from
   * @return right most component of the given path.
   */  
  const char *basename( const char *pPath );
  /**
   * @brief Create a directory.
   * 
   * Creates one directory. Helper method for makeDirs.
   * @sa makeDirs
   * @param pPath [in] path name to create
   * @return true if the creation was successful.
   */  
  bool makeDir(  const char *pPath );
 public:
  /**
   * @brief Default constructor
   */
  PodSync();
  /**
   * @brief Destructor
   */
  ~PodSync();
  /**
   * @brief Perform synchronization.
   *
   * Call this method to perform synchronization according to the current
   * configuration.
   */
  void doSync( void );
  /**
   * @brief Call this method when a USB mass storage device is inserted
   *
   * @param drive [in] Windows drive letter of the inserted drive
   */
  void OnDriveInserted( char drive );
  /**
   * @brief Set the config file to use
   *
   * Set the config file to use. This will reload the configuration.
   *
   * @param pConfig [in] path to the config file name.
   */
  void setConfig( const char *pConfig );
};

#endif // PODSYNC_H
