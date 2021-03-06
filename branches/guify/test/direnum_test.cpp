// Copyright (c)2005 Jean-Baptiste Lab (jean-baptiste dot lab at laposte dot net)
// See the file copying.txt for copying permission.

#include <cppunitlite/Test.h>
#include <cppunitlite/TestResult.h>
#include <cppunitlite/TestRegistry.h>
#include <stdio.h>
#include "src/DirectoryEnum.h"

TEST( DirectoryEnum, findFirst )
{
  DirectoryEnum enumerator;
  CHECK( 0 != enumerator.findFirst( "." ));
}

TEST( DirectoryEnum, findNext )
{
  DirectoryEnum enumerator;
  enumerator.findFirst( "." );
  CHECK( 0 != enumerator.findNext() );
}

TEST( DirectoryEnum, findFirstCheck )
{
  DirectoryEnum enumerator;
  FileInfo *pInfo = enumerator.findFirst( "." );
  CHECK( 0 != pInfo && 0 != pInfo->getName() && 0 != pInfo->getAccessTime() );
}

TEST( DirectoryEnum, findNextCheck )
{
  DirectoryEnum enumerator;
  FileInfo *pInfo = enumerator.findFirst( "." );
  pInfo = enumerator.findNext();
  CHECK( 0 != pInfo && 0 != pInfo->getName() && 0 != pInfo->getAccessTime() );
}

TEST( DirectoryEnum, doubleFindFirst )
{
  DirectoryEnum enumerator;
  CHECK( 0 != enumerator.findFirst( ".." ));
  CHECK( 0 != enumerator.findFirst( "." ));
}

TEST( FileInfo, getInfo )
{
  FileInfo info;
  CHECK( 0 != info.getInfo( "bld/test/direnum_test" ));
}

int main( int, char *[] )
{
  TestResult result;
  TestRegistry::runAllTests( result );
  return result.getFailureCount();
}
