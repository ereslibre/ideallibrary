# This file is part of the Ideal Library
# Copyright (C) 2011 Rafael Fernández López <ereslibre@ereslibre.es>
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with this library; see the file COPYING.LIB.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301, USA.

import Options, Logs

LIBVERSION = '0.0.9'

VERSION = 'v' + LIBVERSION
APPNAME = 'ideallibrary'
top     = '.'
out     = 'build'

subdirs = 'src/core src/modules'

posixPlatforms = ['linux', 'darwin']

checkCompilerFeatures = '''struct A {}; struct B {};
                           template <typename... Type>
                           struct C : public Type... {
                               C(const Type&... obj) : Type(obj)... {}
                           };
                           int main(int argc, char **argv)
                           {
                               A a; B b;
                               C<A, B> *c = new C<A, B>(a, b);
                               delete c;
                               return 0;
                           }'''

def init(ctx):
    pass

def options(opt):
    opt.load('compiler_cxx')
    opt.load('waf_unit_test')
    opt.add_option('--release', action = 'store_true', default = False,
                   help = 'Do not build unit tests. Compile without debug information')

def configure(conf):
    conf.env['POSIX_PLATFORMS'] = posixPlatforms
    conf.load('compiler_cxx')
    conf.load('waf_unit_test')

    if conf.env['COMPILER_CXX'] == []:
        conf.fatal('A C++ compiler is needed. Please, install it and try again')

    conf.env['CXXFLAGS_CONFTESTS'] += ['-std=c++0x'];

    conf.check(fragment = checkCompilerFeatures,
               msg = 'Checking whether ' + conf.env['COMPILER_CXX'] + ' supports C++0x',
               uselib = 'CONFTESTS',
               mandatory = 1)

    conf.sub_config(subdirs)

    conf.env['RELEASE'] = Options.options.release
    conf.env['CXXFLAGS'] += ['-std=c++0x', '-fvisibility=hidden']

    if conf.env['DEST_OS'] in posixPlatforms:
        conf.define('IDEAL_OS_POSIX', 1)
    else:
        conf.undefine('IDEAL_OS_POSIX')

    conf.define('IDEALLIBRARY_PREFIX', conf.env['PREFIX'])
    conf.define('IDEALLIBRARY_VERSION', VERSION)

    if Options.options.release:
        conf.define('NDEBUG', 1)
    else:
        conf.undefine('NDEBUG')

    if Options.options.release:
        Logs.pprint('BLUE', '*** Going to compile in RELEASE mode')
        conf.env['CXXFLAGS'] += ['-O2', '-w']
    else:
        Logs.pprint('BLUE', '*** Going to compile in DEBUG mode (default)')
        Logs.pprint('BLUE', '*** To compile in RELEASE mode run "./waf configure --release [more-options]"')
        conf.env['CXXFLAGS'] += ['-O', '-g', '-Wall', '-Werror']

    conf.write_config_header('src/ideal_conf.h')

def build(bld):
    bld.env['LIBVERSION'] = LIBVERSION
    bld.add_subdirs(subdirs)
    bld.install_files('${PREFIX}/include/ideal', 'src/ideal_conf.h')
    bld.install_files('${PREFIX}/include/ideal', 'src/ideal_export.h')
    bld.install_files('${PREFIX}/include/ideal', 'src/ideal_globals.h')

def check(ctx):
	ctx.exec_command('./waf --alltests')
