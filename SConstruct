import os
import sys

# add a --prefix option for the installation path
AddOption('--prefix', dest='prefix', type='string', nargs=1,
          action='store', metavar='DIR', default='/usr/local',
          help='Installation Prefix')

AddOption('--debug-mode', dest='debug', action='store_true', default=False,
          help='Install with minimal optimisation and GDB debugging enabled') 

env = Environment(PREFIX=GetOption('prefix'))

# set the compiler
env['CC'] = 'g++'

# add required compiler flags
if GetOption('debug'):
  env['CCFLAGS'] = '-std=c++11 -O0 -Wall -Wextra -pedantic -g'
else:
  env['CCFLAGS'] = '-std=c++11 -O3 -Wall -Wextra -pedantic -DNDEBUG'

# code directory
codedir = 'code'
rjobdir = os.path.join(codedir, 'RJObject')
rjobcpdir = os.path.join(rjobdir, 'ConditionalPriors')

cppfiles = Glob(os.path.join(codedir, '*.cpp'))
cppfiles += Glob(os.path.join(rjobdir, '*.cpp'))
cppfiles += Glob(os.path.join(rjobcpdir, '*.cpp'))

# create libraries
sharedlib = env.SharedLibrary('libdnest4', cppfiles)
staticlib = env.StaticLibrary('libdenst4', cppfiles)

# install libraries
installlibDir = os.path.join(env['PREFIX'], 'lib')
if not os.path.isdir(installlibDir):
  print("Error... install directory '%s' is not present" % installlibDir)
  sys.exit(1)

sharedinstall = env.Install(installlibDir, sharedlib)
staticinstall = env.Install(installlibDir, staticlib)

# install headers
hfiles = Glob(os.path.join(codedir, '*.h'))
hfiles += Glob(os.path.join(rjobdir, '*.h'))
hfiles += Glob(os.path.join(rjobcpdir, '*.h'))

#print(env.Dictionary())

installhdir = os.path.join(env['PREFIX'], 'include/dnest4')
if not os.path.isdir(installhdir):
  # try making directory
  try:
    os.mkdir(installhdir)
  except:
    print("Error... could not create '%s' directory" % installhdir)
    sys.exit(1)

hinstall = env.Install(installhdir, hfiles)

# create an 'install' target
env.Alias('install', [sharedinstall, staticinstall, hinstall])

