// Copyright (c)2005 Jean-Baptiste Lab (jean-baptiste dot lab at laposte dot net)
// See the file copying.txt for copying permission.

#include <cppunitlite/Test.h>
#include <cppunitlite/TestResult.h>
#include <cppunitlite/TestRegistry.h>
#include <string.h>

#include "src/PodSync.h"

TEST( PodSync, copyPass )
{
  PodSync podSync;
  CHECK( true == podSync.copy( "SConstruct", "bld/test/copy_test" ));
}

TEST( PodSync, copyFail )
{
  PodSync podSync;
  CHECK( false == podSync.copy( "SConstruct", "bld/test" ));
}

TEST( PodSync, performSync )
{
  PodSync podSync;
  CHECK( true == podSync.performSync( "SConstruct", "bld/test" ));
}

TEST( PodSync, makeDirs1 )
{
  PodSync podSync;
  CHECK( true == podSync.makeDirs( "bld/test/makedirs" ));
}

TEST( PodSync, makeDirs2 )
{
  PodSync podSync;
  CHECK( true == podSync.makeDirs( "bld/test/makedirs/tests/of/deep/dirs" ));
}

TEST( PodSync, makeDirs3 )
{
  PodSync podSync;
  CHECK( true == podSync.makeDirs( "bld/test/makedirs/tests/of/deep/dirs/endSlash/" ));
}

TEST( PodSync, makeDirs4 )
{
  PodSync podSync;
  CHECK( true == podSync.makeDirs( "" ));
}

TEST( PodSync, makeDirs5 )
{
  PodSync podSync;
  CHECK( false == podSync.makeDirs( "/" ));
}

TEST( PodSync, makeDirs6 )
{
  PodSync podSync;
  CHECK( false == podSync.makeDirs( 0 ));
}

TEST( PodSync, makeDirs7 )
{
  PodSync podSync;
  CHECK( true == podSync.makeDirs( "bld\\test\\makedirs" ));
}

TEST( PodSync, makeDirs8 )
{
  PodSync podSync;
  CHECK( true == podSync.makeDirs( "bld\\test\\makedirs\\tests\\of\\deep\\dirs" ));
}

TEST( PodSync, makeDirs9 )
{
  PodSync podSync;
  CHECK( true == podSync.makeDirs( "bld\\test\\makedirs\\tests\\of\\deep\\dirs\\endBackSlash\\" ));
}

TEST( PodSync, basename1 )
{
  PodSync podSync;
  const char *pPath = "bld\\test\\makedirs\\tests\\of\\deep\\dirs";
  const char *pBasename = "dirs";
  CHECK( 0 == strcmp( podSync.basename( pPath ), pBasename ));
}

TEST( PodSync, basename2 )
{
  PodSync podSync;
  const char *pPath = "tests/of/deep/dirs";
  const char *pBasename = "dirs";
  CHECK( 0 == strcmp( podSync.basename( pPath ), pBasename ));
}

TEST( PodSync, basename3 )
{
  PodSync podSync;
  const char *pPath = "tests/of/deep/dirs/";
  const char *pBasename = "";
  CHECK( 0 == strcmp( podSync.basename( pPath ), pBasename ));
}

TEST( PodSync, basename4 )
{
  PodSync podSync;
  const char *pPath = "tests";
  const char *pBasename = "tests";
  CHECK( 0 == strcmp( podSync.basename( pPath ), pBasename ));
}

int main( int argc, char *argv[] )
{
  TestResult result;
  TestRegistry::runAllTests( result );
  return result.getFailureCount();
}

