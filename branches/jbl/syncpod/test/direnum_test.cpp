// Copyright (c)2005 Jean-Baptiste Lab (jean-baptiste dot lab at laposte dot net)
// See the file copying.txt for copying permission.

#include <cppunitlite/Test.h>
#include <cppunitlite/TestResult.h>
#include <cppunitlite/TestRegistry.h>

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

TEST( DirectoryEnum, findFirstVerifyData )
{
  DirectoryEnum enumerator;
  FileInfo *pInfo = enumerator.findFirst( "." );
  // We *know* it is a directory (1st entry is '.')
  CHECK( 0 != pInfo && 0 != pInfo->getName() && 0 != pInfo->getAccessTime() && true == pInfo->getIsDir() );
}

TEST( DirectoryEnum, findNextCheck )
{
  DirectoryEnum enumerator;
  FileInfo *pInfo = enumerator.findFirst( "." );
  pInfo = enumerator.findNext();
  CHECK( 0 != pInfo && 0 != pInfo->getName() && 0 != pInfo->getAccessTime() );
}

TEST( DirectoryEnum, findNextVerifyData )
{
  DirectoryEnum enumerator;
  FileInfo *pInfo = enumerator.findFirst( "." );
  pInfo = enumerator.findNext();
  // We *know* it is a directory (2nd entry is '..')
  CHECK( 0 != pInfo && 0 != pInfo->getName() && 0 != pInfo->getAccessTime() && true == pInfo->getIsDir() );
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
  CHECK( 0 != info.getInfo( "bld/test/direnum_test.exe" ));
}

int main( int argc, char *argv[] )
{
  TestResult result;
  TestRegistry::runAllTests( result );
  return 0;
}
