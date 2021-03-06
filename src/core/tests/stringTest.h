/*
 * This file is part of the Ideal Library
 * Copyright (C) 2009 Rafael Fernández López <ereslibre@ereslibre.es>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include <cppunit/extensions/HelperMacros.h>

#include <core/ideal_string.h>

class StringTest
    : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(StringTest);
    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testContains);
    CPPUNIT_TEST(testSubstr);
    CPPUNIT_TEST(testSplit);
    CPPUNIT_TEST(testPrepend);
    CPPUNIT_TEST(testAppend);
    CPPUNIT_TEST(miscTests);
    CPPUNIT_TEST(testOperators);
    CPPUNIT_TEST(testToConversion);
    CPPUNIT_TEST(testNumber);
    CPPUNIT_TEST(testMisc);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void testConstructor();
    void testContains();
    void testSubstr();
    void testSplit();
    void testPrepend();
    void testAppend();
    void miscTests();
    void testOperators();
    void testToConversion();
    void testNumber();
    void testMisc();

private:
    IdealCore::String returnSpecialChars();
};
