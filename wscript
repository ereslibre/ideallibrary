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

import Options, Logs, re

LIBVERSION = '0.0.9'

VERSION = 'v' + LIBVERSION
APPNAME = 'ideallibrary'
top     = '.'
out     = 'build'

subdirs = 'src/core'

posixPlatforms = ['linux', 'darwin', 'haiku']

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
	           uselib = 'CONFTESTS', mandatory = True)

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

	conf.env['RPATH_IDEAL'] = conf.env['PREFIX'] + '/lib'

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
	if bld.is_install or bld.options.all_tests:
		bld.options.all_tests = True
		bld.add_post_fun(cppunit_summary)

def check(ctx):
	ctx.exec_command('./waf --alltests')

def cppunit_summary(bld):
	lst = getattr(bld, 'utest_results', [])
	if lst:
		Logs.pprint('CYAN', 'execution summary')

		total = len(lst)
		tfail = len([x for x in lst if x[1]])

		Logs.pprint('CYAN', '  tests that pass %d/%d' % (total-tfail, total))
		for (f, code, out, err) in lst:
			if not code:
				Logs.pprint('CYAN', '    %s' % f)

		Logs.pprint('CYAN', '  tests that fail %d/%d' % (tfail, total))
		for (f, code, out, err) in lst:
			if code:
				Logs.pprint('CYAN', '    %s' % f)
				err = err.decode()
				err_info = re.findall(r'([\w.]+):(\d+)[^\n]+\nTest name:\s+([^\n]+)\n([^\n]+)\n', err)
				if err_info:
					(filename, line, test_failed, assertion_type) = err_info[0]
					Logs.pprint('RED',  '      %s:%s - %s (%s)' % (filename, line, test_failed, assertion_type))
					equality_info = re.findall(r'- Expected\s*:\s+([^\n]+)\n- Actual\s*:\s+(.+)', err)
					if equality_info:
						(expected, actual) = equality_info[0]
						Logs.pprint('RED',  '        Expected : %s' % (expected))
						Logs.pprint('RED',  '        Actual   : %s' % (actual))
						continue
					assertion_info = re.findall(r'- Expression\s*:\s+([^\n]+)\n', err)
					if assertion_info:
						(expression) = assertion_info[0]
						Logs.pprint('RED',  '        Expression : %s' % (expression))
