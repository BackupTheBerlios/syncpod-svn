// Copyright (c)2005 Jean-Baptiste Lab (jean-baptiste dot lab at laposte dot net)
// See the file copying.txt for copying permission.

#include <cppunitlite/Test.h>
#include <cppunitlite/TestResult.h>
#include <cppunitlite/TestRegistry.h>
#include <string.h>
#include <iostream>

#include "src/ConfigFile.h"

TEST( ConfigFile, noFile )
{
  ConfigFile cfg;
  CHECK( false == cfg.reload() );
}

TEST( ConfigFile, reload )
{
  ConfigFile cfg( "test/test_cfg.cfg" );
  CHECK( true == cfg.reload() );
}

TEST( ConfigFile, getString )
{
  ConfigFile cfg( "test/test_cfg.cfg" );
  const char *pValue = cfg.getString( "key1" );
  CHECK( 0 != pValue );
  pValue = cfg.getString( "key2" );
  CHECK( 0 != pValue );
}

TEST( ConfigFile, setString )
{
  ConfigFile cfg;
  cfg.setString( "key3", "value3" );
  const char *pValue = cfg.getString( "key3" );
  CHECK( 0 != pValue );
}

TEST( ConfigFile, writeNew )
{
  ConfigFile cfg( "bld/test/auto_cfg_0.cfg" );
  cfg.setString( "key3", "value3" );
  CHECK( true == cfg.write() );
}

TEST( ConfigFile, setStrings )
{
  ConfigFile cfg( "bld/test/auto_cfg_1.cfg" );
  cfg.setString( "key1", "value1" );
  cfg.setString( "key2", "value2" );
  cfg.setString( "key3", "value3" );
  const char *pValue = cfg.getString( "key3" );
  CHECK( 0 != pValue );
  cfg.write();
}

TEST( ConfigFile, writeAppend )
{
  ConfigFile cfg( "bld/test/auto_cfg_0.cfg" );
  cfg.setString( "key1", "value1" );
  cfg.setString( "key2", "value2" );
  CHECK( true == cfg.write() );
}

int main( int argc, char *argv[] )
{
  TestResult result;
  TestRegistry::runAllTests( result );
  return 0;
}
