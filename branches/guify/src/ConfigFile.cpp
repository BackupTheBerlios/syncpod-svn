// Copyright (c)2005 Jean-Baptiste Lab (jean-baptiste dot lab at laposte dot net)
// See the file copying.txt for copying permission.

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#if defined HAVE_NVWA
#include <nvwa/debug_new.h>
#endif

#include "ConfigFile.h"

ConfigFile::ConfigFile()
{
  m_name    = NULL;
  m_data    = NULL;
  m_numKeys = 0;
}

ConfigFile::ConfigFile( const char *pName )
{
  m_name    = dupStr( pName );
  m_data    = NULL;
  m_numKeys = 0;
  reload();
}

ConfigFile::~ConfigFile()
{
  if( NULL != m_name )
  {
    delete[] m_name;
  }
  if( NULL != m_data )
  {
    for( int i = 0; i < m_numKeys; i++ )
    {
      if( NULL != m_data[i].key )
      {
        delete[] m_data[i].key;
      }
      if( NULL != m_data[i].value )
      {
        delete[] m_data[i].value;
      }
    }
    delete[] m_data;
  }
}

char *ConfigFile::dupStr( const char *pStr )
{
  char *result = 0;
  if( 0 != pStr )
  {
    result = new char[ strlen( pStr ) + 1 ];
    strcpy( result, pStr );
  }
  return result;
}

const char *ConfigFile::getString( const char *pKey )
{
  if( NULL != m_data )
  {
    int i = 0;
    while( NULL != m_data[i].key )
    {
      if( 0 == strcmp( m_data[i].key, pKey ))
      {
        return m_data[i].value;
      }
      i++;
    }
  }
  
  return NULL;
}

void ConfigFile::setString( const char *pKey, const char *pValue )
{
  if( NULL != m_data )
  {
    int i;
    for( i = 0; i < m_numKeys; i++ )
    {
      if( 0 == strcmp( m_data[i].key, pKey ))
      {
        if( NULL != m_data[i].value )
        {
          delete[] m_data[i].value;
        }
        if( NULL !=  pValue )
        {
          m_data[i].value = dupStr( pValue );
        }
        else
        {
          m_data[i].value = NULL;
        }
        break;
      }
    }
    if( i >= m_numKeys )
    {
      ConfigData *pNewData = new ConfigData[ m_numKeys + 1 ];
      if( NULL != pNewData )
      {
        memcpy( pNewData, m_data, m_numKeys * sizeof( ConfigData ));
        delete[] m_data;
        m_data = pNewData;
      }
      m_data[m_numKeys].key   = dupStr( pKey );
      m_data[m_numKeys].value = dupStr( pValue );
      m_numKeys++;
    }
  }
  else
  {
    m_data = new ConfigData[2];
    if( NULL != m_data )
    {
      m_data[0].key   = dupStr( pKey );
      m_data[0].value = dupStr( pValue );
      m_numKeys = 1;
    }
  }
}

void ConfigFile::setName( const char *pName )
{
  if( NULL != m_name )
  {
    delete[] m_name;
    m_name = NULL;
  }
  if( NULL != pName )
  {
    m_name = dupStr( pName );
  }
  else
  {
    m_name = NULL;
  }
}

bool ConfigFile::write()
{
  if( NULL != m_name )
  {
    FILE *f = fopen( m_name, "wb" );
    if( NULL != f )
    {
      for( int i = 0; i < m_numKeys; i++ )
      {
        fprintf( f, "%s=%s\n", m_data[i].key, m_data[i].value );
      }
      fclose( f );
      return true;
    }
  }
  return false;
}

bool ConfigFile::write( const char *pName )
{
  char *tempName = NULL;
  bool result;
  if( NULL != m_name )
  {
    tempName = dupStr( m_name );
    setName( pName );
    result = write();
    setName( tempName );
    delete[] tempName;
  }
  else
  {
    setName( pName );
    result = write();
    setName( tempName );
  }
  return result;
}

bool ConfigFile::reload()
{
  if( NULL != m_name )
  {
    FILE *f = fopen( m_name, "rb" );
    if( NULL != f )
    {
      char buffer[1024];
      char *equal;
      while( NULL != fgets( buffer, 1024, f ))
      {
        // Skip comments
        if( '#' != buffer[0] )
        {
          // Chop off the \n
          if( 0x0D == buffer[strlen( buffer ) - 3] || 0x0A == buffer[strlen( buffer ) - 3] )
          {
            buffer[strlen( buffer ) - 3] = '\0';
          }
          // Chop off the \r
          if( 0x0D == buffer[strlen( buffer ) - 2] || 0x0A == buffer[strlen( buffer ) - 2] )
          {
            buffer[strlen( buffer ) - 2] = '\0';
          }
          
          equal = strchr( buffer, '=' );
          if( NULL != equal )
          {
            *equal = '\0';
            // Ugly allocation scheme...
            if( NULL == m_data )
            {
              m_data = new ConfigData[2];
              m_numKeys = 0;
            }
            else
            {
              // + 2 for the NULL termination...
              ConfigData *pNewData = new ConfigData[ m_numKeys + 2 ];
              if( NULL != pNewData )
              {
                memcpy( pNewData, m_data, m_numKeys * sizeof( ConfigData ));
                delete[] m_data;
                m_data = pNewData;
              }
            }
            m_data[m_numKeys].key               = dupStr( buffer );
            m_data[m_numKeys].key[equal-buffer] = '\0';
            m_data[m_numKeys].value             = dupStr( equal + 1 );
            m_numKeys++;
          }
        }
      }
      fclose( f );
    }
    return true;
  }
  return false;
}
