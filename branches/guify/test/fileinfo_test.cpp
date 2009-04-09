// Copyright (c)2005 Jean-Baptiste Lab (jean-baptiste dot lab at laposte dot net)
// See the file copying.txt for copying permission.

#include <cppunitlite/Test.h>
#include <cppunitlite/TestResult.h>
#include <cppunitlite/TestRegistry.h>

#include "src/FileInfo.h"

TEST( FileInfo, getInfoOK )
{
  FileInfo info;
  CHECK( true == info.getInfo( "." ));
}

TEST( FileInfo, getInfoNOK )
{
  FileInfo info;
  CHECK( false == info.getInfo( "qwertyuiop" ));
}

TEST( FileInfo, getName )
{
  FileInfo info;
  bool res = info.getInfo( "." );
  const char *pName = 0;
  
  CHECK( true == res );
  if( true == res )
  {
    pName = info.getName();
    CHECK( 0 != pName );
  }
}

TEST( FileInfo, isDir )
{
  FileInfo info;
  bool res = info.getInfo( "." );
  CHECK( true == res );
  if( true == res )
  {
    CHECK( true == info.isDir());
  }
}

TEST( FileInfo, isNotDir )
{
  FileInfo info;
  bool res = info.getInfo( "SConstruct" );
  CHECK( true == res );
  if( true == res )
  {
    CHECK( false == info.isDir());
  }
}

int main( int argc, char *argv[] )
{
  TestResult result;
  TestRegistry::runAllTests( result );
  return result.getFailureCount();
}
