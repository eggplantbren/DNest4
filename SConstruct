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

# code directory
codedir = 'code'
rjobdir = os.path.join(codedir, 'RJObject')
rjobcpdir = os.path.join(rjobdir, 'ConditionalPriors')
distrdir = os.path.join(codedir, 'Distributions')

cppfiles = Glob(os.path.join(codedir, '*.cpp'))
cppfiles += Glob(os.path.join(rjobdir, '*.cpp'))
cppfiles += Glob(os.path.join(rjobcpdir, '*.cpp'))
cppfiles += Glob(os.path.join(distrdir, '*.cpp'))

# include path for headers
env.Append(CPPPATH = [codedir])

# add required compiler flags
if GetOption('debug'):
  env.Append(CCFLAGS = ['-std=c++11', '-O0', '-Wall', '-Wextra', '-pedantic', '-g'])
else:
  env.Append(CCFLAGS = ['-std=c++11', '-O3', '-march=native', '-Wall', '-Wextra', '-pedantic', '-DNDEBUG'])

# create libraries
sharedlib = env.SharedLibrary('libdnest4', cppfiles)
staticlib = env.StaticLibrary('libdnest4', cppfiles)

# install libraries
installlibDir = os.path.join(env['PREFIX'], 'lib')
if not os.path.isdir(installlibDir):
  print("Error... install directory '%s' is not present" % installlibDir)
  sys.exit(1)

sharedinstall = env.Install(installlibDir, sharedlib)
staticinstall = env.Install(installlibDir, staticlib)

installlist = [sharedinstall, staticinstall]

hinstpref = {codedir: os.path.join(env['PREFIX'], 'include/dnest4'),
             rjobdir: os.path.join(env['PREFIX'], 'include/dnest4/RJObject'),
             rjobcpdir: os.path.join(env['PREFIX'], 'include/dnest4/RJObject/ConditionalPriors'),
             distrdir: os.path.join(env['PREFIX'], 'include/dnest4/Distributions')}

# install headers
for d in hinstpref: 
  hfiles = Glob(os.path.join(d, '*.h'))

  installhdir = hinstpref[d]
  if not os.path.isdir(installhdir):
    # try making directory
    try:
      os.makedirs(installhdir)
    except:
      print("Error... could not create '%s' directory" % installhdir)
      sys.exit(1)

  hinstall = env.Install(installhdir, hfiles)
  installlist.append(hinstall)

# create an 'install' target
env.Alias('install', installlist)

