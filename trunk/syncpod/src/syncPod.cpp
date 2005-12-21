// Copyright (c)2005 Jean-Baptiste Lab (jean-baptiste dot lab at laposte dot net)
// See the file copying.txt for copying permission.

#include <windows.h>
#include <dbt.h>
#include <stdio.h>
#include <getopt.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#if defined HAVE_RLOG
#include <rlog/rlog.h>
#include <rlog/StdioNode.h>
#include <rlog/RLogChannel.h>
using namespace rlog;
#else
#include "no_rlog.h"
#endif

#include "PodSync.h"

/**
 * @brief Syncpod options class
 *
 * Defines options that can be set on the command line.
 */
class SyncPodOptions
{
public:
  /**
   * Log file path as specified on the command line.
   */
  const char *logFile;
  /**
   * Config file to use as specified on the command line.
   */  
  const char *configFile;
  /**
   * Level of logging: 0: critical, 1: warnings, 2: everything
   */
  unsigned int logLevel;
  /**
   * @brief Initialise the members to 0.
   */
  SyncPodOptions();
};

SyncPodOptions::SyncPodOptions()
{
  logFile    = 0;
  configFile = 0;
  logLevel   = 0;
}

static PodSync g_app;
static int     g_appResult = 0;

static char driveFromMask( ULONG unitmask )
{
  char i;

  for (i = 0; i < 26; ++i)
  {
    if (unitmask & 0x1)
      break;
    unitmask = unitmask >> 1;
  }

  return (char)(i + 'A');
}

static LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  switch( msg )
  {
  case WM_CLOSE:
    DestroyWindow( hwnd );
    break;
  case WM_DESTROY:
    PostQuitMessage( 0 );
    break;
  case WM_DEVICECHANGE:
    switch( wParam )
    {
    case DBT_DEVICEARRIVAL:
    {
      PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;
      if (lpdb -> dbch_devicetype == DBT_DEVTYP_VOLUME)
      {
        PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
        if( false == g_app.OnDriveInserted( driveFromMask( lpdbv ->dbcv_unitmask )))
        {
          rError( "Problem during synchronization, see logs" );
          g_appResult = 1;
          PostQuitMessage( g_appResult );
        }
      }
    }
    break;
    default:
      break;
    }    
    break;
  default:
    return DefWindowProc( hwnd, msg, wParam, lParam );
  }
  return 0;
}

static void usage( void )
{
  printf( "Usage: syncPod [OPTIONS]\n" );
  printf( "  -c FILE Use FILE as config (defaults to syncPod.cfg)\n" );
  printf( "  -l FILE Log to FILE (requires rLog-enabled build: " );
#if defined HAVE_RLOG
  printf( "ON." );
#else
  printf( "OFF." );
#endif
  printf( ")\n" );
  printf( "  -v LEVEL Set the logging level 0: critical(DEFAULT), 1: warnings, 2: everything.\n\n" );
  printf( "  -h --help Display this help and exit.\n\n" );
  printf( "Copyright (c)2005 Jean-Baptiste Lab (jean-baptiste dot lab at laposte dot net)\n" );
  printf( "See the file copying.txt for copying permission.\n" );
}

static bool parseOptions( int argc, char *argv[], SyncPodOptions *options )
{
  int c;
  bool result = true;
  while (1)
  {  
    c = getopt( argc, argv, "c:l:v:h" );
    /* Detect the end of the options. */
    if( c == -1 )
      break;
    switch (c)
    {
    case 'c':
      options->configFile = optarg;
      break;
    case 'l':
      options->logFile = optarg;
      break;
    case 'v':
      sscanf( optarg, "%u", &options->logLevel );
      break;
    case 'h':
      usage();
      result = false;
      break;
    default:
      break;
    }
  }   
  return result;
}

static bool createInvisibleWindow( void )
{
  WNDCLASSEX wc;
  HWND hwnd;
  const char g_szClassName[] = "myWindowClass";
  wc.cbSize        = sizeof( WNDCLASSEX );
  wc.style         = 0;
  wc.lpfnWndProc   = WndProc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hInstance     = 0;
  wc.hIcon         = LoadIcon( NULL, IDI_APPLICATION );
  wc.hCursor       = LoadCursor( NULL, IDC_ARROW );
  wc.hbrBackground = (HBRUSH)( COLOR_WINDOW+1 );
  wc.lpszMenuName  = NULL;
  wc.lpszClassName = g_szClassName;
  wc.hIconSm       = LoadIcon( NULL, IDI_APPLICATION );

  if(!RegisterClassEx(&wc))
  {
    MessageBox( NULL, 
                "Window Registration Failed!",
                "Error!",
                MB_ICONEXCLAMATION | MB_OK );
    return false;
  }

  hwnd = CreateWindowEx( WS_EX_CLIENTEDGE,
                         g_szClassName,
                         "The title of my window",
                         WS_OVERLAPPEDWINDOW,
                         CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
                         NULL, NULL, 0, NULL );

  if( NULL == hwnd )
  {
    MessageBox( NULL, 
                "Window Creation Failed!",
                "Error!",
                MB_ICONEXCLAMATION | MB_OK );
    return false;
  }
  return true;
}

int main( int argc, char *argv[] )
{
#if defined HAVE_RLOG
  StdioNode *log = 0;
  int fd = -1;
#endif
  SyncPodOptions l_options;
  if( true == parseOptions( argc, argv, &l_options ))
  {
    rInfo( "Options ok" );
#if defined HAVE_RLOG
    /** @todo Actually use the log file argument */
    if( 0 != l_options.logFile )
    {
      fd = open( l_options.logFile, _O_CREAT | _O_TRUNC | _O_RDWR | _O_BINARY, _S_IWRITE | _S_IREAD );
      if( -1 != fd )
      {
        log = new StdioNode( fd );
      }
      else
      {
        log = new StdioNode;
      }
    }
    else
    {
      log = new StdioNode;
    }
    switch( l_options.logLevel )
    {
    case 0:
      log->subscribeTo( GetGlobalChannel("error") );
      break;
    case 1:
      log->subscribeTo( GetGlobalChannel("error") );
      log->subscribeTo( GetGlobalChannel("warning") );
      break;
    default:
      log->subscribeTo( GetGlobalChannel("") );
      break;
    }
#endif
    if( 0 != l_options.configFile )
    {
      g_app.setConfig( l_options.configFile );
    }
    else
    {
      g_app.setConfig( "syncpod.cfg" );
    }
    if( true == createInvisibleWindow() )
    {
      rInfo( "createInvisibleWindow ok" );
      MSG Msg;
      if( false == g_app.doSync() )
      {
        rError( "Problem during synchronization, see logs" );
        g_appResult = 1;
        PostQuitMessage( g_appResult );
      }
      while( GetMessage( &Msg, NULL, 0, 0 ) > 0 )
      {
        TranslateMessage( &Msg );
        DispatchMessage( &Msg );
      }
    }
  }
#if defined HAVE_RLOG
  if( -1 != fd )
  {
    close( fd );
  }
  delete log;
#endif
  return g_appResult;
}
