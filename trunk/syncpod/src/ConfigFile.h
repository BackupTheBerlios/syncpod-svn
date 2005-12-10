// Copyright (c)2005 Jean-Baptiste Lab (jean-baptiste dot lab at laposte dot net)
// See the file copying.txt for copying permission.

#if !defined CONFIGFILE_H
#define CONFIGFILE_H

/**
 * @brief Config file object.
 *
 * Allow storage and retrieval of configuration items.
 * Will serialize to and deserialize from a file.
 * @todo Improve storage data structure for item storage
 */
class ConfigFile
{
 private:
  char *m_name;                 //!< File name for persistent storage
  /**
   * @brief Data structure to store config items.
   */
  typedef struct
  {
    char *key;                  //!< Key of the config item
    char *value;                //!< Value of the config item
  } ConfigData;
  ConfigData *m_data;           //!< Dynamically sized array of config items
  int m_numKeys;                //!< Number of elements in the config array

#if defined (UNIT_TESTS)
 public:
#endif
  /**
   * @brief Equivalent of strdup (which is not ANSI).
   * 
   * Equivalent of strdup, implemented using new[], allowing this to be
   * taken into account by nvwa.
   * @param pStr [in] string to duplicate
   * @return pointer to copy of string, 0 if no more memory
   * @important remember to delete[] the returned value
   */
  char *dupStr( const char *pStr );
 public:
  ConfigFile();
  ConfigFile( const char *pFile );
  ~ConfigFile();
  /**
   * @brief Retrieve the data associated with the given key.
   * @param pKey [in] key to search
   * @return 0 if no item with this key found, value of that key otherwise
   */
  const char *getString( const char *pKey );
  /**
   * @brief Store a value associated with the given key
   *
   * If no item with this key is found, a new item is added.
   * @param pKey [in] item key
   * @param pValue [in] item value
   */
  void setString( const char *pKey, const char *pValue );
  /**
   * @brief Set the name of the file for persistent storage
   * @param pFile [in] file name
   */
  void setName( const char *pFile );
  /**
   * @brief Flush the current config data to the current file name
   * @return true if write was succesfull
   */
  bool write();
  /**
   * @brief Flush the current config data to the file name
   * @param pFile [in] file name
   * @return true if write was succesfull
   */
  bool write( const char *pFile );
  /**
   * @brief Refresh the current config with the content of the current file
   * @return true if reloading was succesfull
   */
  bool reload();
};

#endif // CONFIGFILE_H
