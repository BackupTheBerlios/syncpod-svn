## Copyright (c)2005 Jean-Baptiste Lab (jean-baptiste dot lab at laposte dot net)
## See the file copying.txt for copying permission.

import sys
from SCons import *
from SCons.Script import Options
from SCons.Options import EnumOption
import SCons.Environment

def JBL_GenerateEnv( project, opts = None, ansi = False ):
    # Setup the options
    if sys.platform == 'win32':
        defaultCompiler = 'mingw'
        defaultInstallPrefix = 'c:/mingw'
    elif sys.platform == 'cygwin':
        defaultCompiler = 'cygwin'
        defaultInstallPrefix = '/usr'
    if opts is None:
        opts = Options()
    # Adds some standard options
    opts.Add( 'prefix', 'Set the installation prefix', defaultInstallPrefix )
    opts.Add( 'debug', 'Set to 1 to build with debug symbols', 0 )
    opts.Add( EnumOption('compiler', 'Attempt to build using the given compiler', defaultCompiler, allowed_values = ( 'mingw', 'cygwin', 'bcc32' ) ))
    # Dummy environment to get the options
    env = SCons.Environment.Environment( options = opts, tools = [] )
    env.SConsignFile( project + '_sign' )
    # Set up the env
    if env['compiler'] == 'mingw':
        tools = [ 'mingw' ]
        compilerType = 'gcc'
    elif env['compiler'] == 'cygwin':
        tools = ['default']      
        compilerType = 'gcc'
    elif env['compiler'] == 'bcc32':
        tools = ['bcc32', 'tlib', 'ilink32']
        compilerType = 'borland'

    # Now creates the environment with the correct tools
    env = env.Copy( tools = tools, options = opts )
    env.Help(opts.GenerateHelpText(env))
    # Set up the compiler options according to the compiler type/platform
    if compilerType == 'gcc':
        env.Append( CPPFLAGS = [ '-Wall', '-pedantic' ] )
        if ansi is True:
            env.Append( CPPFLAGS = [ '-ansi' ] )
        # Only on Windows type platform
        if env['PLATFORM'] == 'cygwin' or env['PLATFORM'] == 'win32':
            env.Append( LINKFLAGS = ['-mwindows'] )
    elif compilerType == 'borland':
        env.Append( CPPFLAGS = ['-w', '-Q', '-DWIN32_LEAN_AND_MEAN'] )

    if int( env['debug'] ) == 1:
        if compilerType == 'gcc':
            env.Append( CPPFLAGS = [ '-g' ] )
        elif compilerType == 'borland':
            # Find out which Borland options to set for debug
            env.Append( CPPFLAGS = [] )
    else:
        if compilerType == 'gcc':
            env.Append( CPPFLAGS = [ '-O2' ] )
        elif compilerType == 'borland':
            # Find out which Borland options to set
            env.Append( CPPFLAGS = [] )
    return env
