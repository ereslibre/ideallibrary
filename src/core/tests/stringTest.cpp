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

#include "stringTest.h"

#include <string.h>

using namespace IdealCore;

CPPUNIT_TEST_SUITE_REGISTRATION(StringTest);

void StringTest::setUp()
{
}

void StringTest::tearDown()
{
}

void StringTest::testConstructor()
{
    {
        String str;
        str.append("Test");
        CPPUNIT_ASSERT(!strcmp(str.data(), "Test"));
        String str2;
        CPPUNIT_ASSERT(!strcmp(str2.data(), ""));
    }
    setlocale(LC_ALL, "en_US.UTF-8");
    String test1("Test");
    CPPUNIT_ASSERT_EQUAL((size_t) 4, test1.size());
    String test2("Tést");
    CPPUNIT_ASSERT_EQUAL((size_t) 4, test2.size());
    String test3("Tést");
    CPPUNIT_ASSERT(test2 == test3);
    String test4("Téest");
    CPPUNIT_ASSERT(test2 != test4);
    String test5_1("Teñt");
    String test5_2("Tezt");
    CPPUNIT_ASSERT(test5_1 < test5_2);
    CPPUNIT_ASSERT(test5_1 != test5_2);
    CPPUNIT_ASSERT(!(test5_1 == test5_2));
    CPPUNIT_ASSERT(test5_1 <= test5_2);
    CPPUNIT_ASSERT(test5_2 >= test5_1);
    CPPUNIT_ASSERT(test5_2 > test5_1);
    String test6;
    const ichar *test6_1 = "file:///home/test";
    const size_t size = strlen(test6_1);
    ichar *test6_2 = (ichar*) calloc(size + 1, sizeof(ichar));
    strcpy(test6_2, test6_1);
    test6 = test6_2;
    CPPUNIT_ASSERT_EQUAL(size, test6.size());
    free(test6_2);
}

void StringTest::testContains()
{
    {
        String str("Hello");
        CPPUNIT_ASSERT(str.contains('H'));
        CPPUNIT_ASSERT(!str.contains('h'));
        CPPUNIT_ASSERT(str.contains('e'));
        CPPUNIT_ASSERT(!str.contains('u'));
        CPPUNIT_ASSERT(str.contains('o'));
    }
}

void StringTest::testSubstr()
{
    {
        String str("Hello");
        CPPUNIT_ASSERT(!str.substr(0, 0).size());
        CPPUNIT_ASSERT_EQUAL(String("ll"), str.substr(2, 2));
        CPPUNIT_ASSERT_EQUAL(String("Hello"), str.substr(0, 5));
        CPPUNIT_ASSERT_EQUAL(String("Hello"), str.substr(0, 10));
        CPPUNIT_ASSERT_EQUAL(String("ello"), str.substr(1, 4));
        CPPUNIT_ASSERT_EQUAL(String("ello"), str.substr(1, 10));
        CPPUNIT_ASSERT_EQUAL(String("ell"), str.substr(1, 3));
        CPPUNIT_ASSERT_EQUAL(String("o"), str.substr(4, 1));
    }
    {
        String str("Test");
        CPPUNIT_ASSERT_EQUAL(String("Test"), str.substr(0, 4));
    }
    {
        String str("Tést");
        CPPUNIT_ASSERT_EQUAL(String("Tést"), str.substr(0, 4));
    }
    {
        String str("ñTest");
        CPPUNIT_ASSERT_EQUAL(String("Test"), str.substr(1, 4));
    }
    {
        String str("𝛏𝛏Tést");
        CPPUNIT_ASSERT_EQUAL(String("Tést"), str.substr(2, 4));
    }
    {
        String str("Testñ");
        CPPUNIT_ASSERT_EQUAL(String("Test"), str.substr(0, 4));
    }
    {
        String str("Tést𝛏𝛏");
        CPPUNIT_ASSERT_EQUAL(String("Tést"), str.substr(0, 4));
    }
    {
        String str("𝛏𝛏Tést𝛏𝛏");
        CPPUNIT_ASSERT_EQUAL(String("Tést"), str.substr(2, 4));
    }
    {
        String str("𝛏𝛏Tés𝛏t𝛏𝛏");
        CPPUNIT_ASSERT_EQUAL(String("Tés𝛏t"), str.substr(2, 5));
    }
    {
        String str("áéíóú𝛏𝛏Tést𝛏𝛏áéíóú");
        CPPUNIT_ASSERT_EQUAL(String("Tést"), str.substr(7, 4));
    }
    {
        String str("áéíóú𝛏𝛏Tést𝛏𝛏áéíóú");
        CPPUNIT_ASSERT_EQUAL(String("𝛏Tést𝛏"), str.substr(6, 6));
    }
    {
        String str("áéíóú𝛏𝛏Tést𝛏𝛏áéíóú");
        CPPUNIT_ASSERT_EQUAL(String("𝛏𝛏Tést𝛏𝛏"), str.substr(5, 8));
    }
}

void StringTest::testSplit()
{
    {
        String test1("No split at all");
        CPPUNIT_ASSERT_EQUAL((size_t) 1, test1.split('w').size());
        CPPUNIT_ASSERT_EQUAL(String("No split at all"), test1.split('w').front());
    }
    {
        String test2("Option 1;Option 2");
        List<String> res = test2.split(';');
        CPPUNIT_ASSERT_EQUAL((size_t) 2, res.size());
        CPPUNIT_ASSERT_EQUAL(String("Option 1"), res.front());
        CPPUNIT_ASSERT_EQUAL(String("Option 2"), res.back());
    }
    {
        String test3(",Option 1,");
        CPPUNIT_ASSERT_EQUAL((size_t) 1, test3.split(',').size());
    }
    {
        String test4("Option 1;");
        CPPUNIT_ASSERT_EQUAL((size_t) 1, test4.split(';').size());
    }
    {
        String test5(";a;b;");
        List<String> res = test5.split(';');
        CPPUNIT_ASSERT_EQUAL((size_t) 2, res.size());
        CPPUNIT_ASSERT_EQUAL(String("a"), res.front());
        CPPUNIT_ASSERT_EQUAL(String("b"), res.back());
    }
    {
        String test6("aObocOd");
        CPPUNIT_ASSERT_EQUAL((size_t) 3, test6.split('O').size());
    }
    {
        String test7("a,b,c,d,e");
        List<String> res = test7.split(',');
        String list[] = { String("a"), String("b"), String("c"),
                          String("d"), String("e") };
        List<String>::iterator it = res.begin();
        iint32 i = 0;
        for (it = res.begin(); it != res.end(); ++it, ++i) {
            CPPUNIT_ASSERT_EQUAL(list[i], *it);
        }
        CPPUNIT_ASSERT_EQUAL((size_t) 5, res.size());
    }
}

void StringTest::testPrepend()
{
    {
        String str("Test");
        CPPUNIT_ASSERT_EQUAL(String("This is a Test"), str.prepend("This is a "));
    }
    {
        String str("ｱｲｳｴｵｶｷｸｹ");
        CPPUNIT_ASSERT_EQUAL(String("ｾｿﾀﾁﾂﾃｱｲｳｴｵｶｷｸｹ"), str.prepend("ｾｿﾀﾁﾂﾃ"));
    }
    {
        String str("Test");
        CPPUNIT_ASSERT_EQUAL(String("aTest"), str.prepend('a'));
    }
    {
        String str("Test");
        CPPUNIT_ASSERT_EQUAL(String("áTest"), str.prepend(L'á'));
    }
}

void StringTest::testAppend()
{
    {
        String str("This is a ");
        CPPUNIT_ASSERT_EQUAL(String("This is a Test"), str.append("Test"));
    }
    {
        String str("ｾｿﾀﾁﾂﾃ");
        CPPUNIT_ASSERT_EQUAL(String("ｾｿﾀﾁﾂﾃｱｲｳｴｵｶｷｸｹ"), str.append("ｱｲｳｴｵｶｷｸｹ"));
    }
    {
        String str('a');
        CPPUNIT_ASSERT_EQUAL(String("aTest"), str.append("Test"));
    }
    {
        String str(L'á');
        CPPUNIT_ASSERT_EQUAL(String("áTest"), str.append("Test"));
    }
}

void StringTest::miscTests()
{
    String specialChars = returnSpecialChars();
    CPPUNIT_ASSERT_EQUAL((size_t) 10, specialChars.size());
    {
        String specialChars2(specialChars);
        CPPUNIT_ASSERT_EQUAL((size_t) 10, specialChars2.size());
    }
    {
        String specialChars;
        specialChars = returnSpecialChars();
        CPPUNIT_ASSERT_EQUAL((size_t) 10, specialChars.size());
    }
    {
        String specialChars = String('%') + String('3') + String('4');
        CPPUNIT_ASSERT_EQUAL(String("%34"), specialChars);
        CPPUNIT_ASSERT_EQUAL((size_t) 3, specialChars.size());
    }
    {
        String specialChars = String("%34");
        CPPUNIT_ASSERT_EQUAL(String("%34"), specialChars);
        CPPUNIT_ASSERT_EQUAL((size_t) 3, specialChars.size());
    }
    {
        String specialChars("file:///home/user/imágenes/spécial.png");
        CPPUNIT_ASSERT_EQUAL((size_t) 38, specialChars.size());
    }
    {
        String specialChars("šťžľčěďňřůĺ");
        CPPUNIT_ASSERT_EQUAL((size_t) 11, specialChars.size());
    }
    {
        String specialChars("абвгдеёжзийклмно");
        CPPUNIT_ASSERT_EQUAL((size_t) 16, specialChars.size());
    }
    {
        String specialChars("ЂЉЊЋЏђљњћџ");
        CPPUNIT_ASSERT_EQUAL((size_t) 10, specialChars.size());
    }
    {
        String specialChars("ščžćđ");
        CPPUNIT_ASSERT_EQUAL((size_t) 5, specialChars.size());
    }
    {
        String specialChars("ЎўЄєҐґ");
        CPPUNIT_ASSERT_EQUAL((size_t) 6, specialChars.size());
    }
    {
        String specialChars("ሰማይ አይታረስ ንጉሥ አይከሰስ።");
        CPPUNIT_ASSERT_EQUAL((size_t) 20, specialChars.size());
    }
    {
        String specialChars("⡌⠁⠧⠑ ⠼⠁⠒  ⡍⠜⠇⠑⠹⠰⠎ ⡣⠕⠌");
        CPPUNIT_ASSERT_EQUAL((size_t) 21, specialChars.size());
    }
    {
        String specialChars("ᚻᛖ ᚳᚹᚫᚦ ᚦᚫᛏ ᚻᛖ ᛒᚢᛞᛖ ᚩᚾ ᚦᚫᛗ ᛚᚪᚾᛞᛖ ᚾᚩᚱᚦᚹᛖᚪᚱᛞᚢᛗ ᚹᛁᚦ ᚦᚪ ᚹᛖᛥᚫ");
        CPPUNIT_ASSERT_EQUAL((size_t) 56, specialChars.size());
    }
    {
        String specialChars("てすと");
        CPPUNIT_ASSERT_EQUAL((size_t) 3, specialChars.size());
    }
    {
        String specialChars("ｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃ");
        CPPUNIT_ASSERT_EQUAL((size_t) 19, specialChars.size());
    }
}

void StringTest::testOperators()
{
    {
        String str("This is a string");
        const iint32 strInitialSize = str.size();
        String str2;
        str2 = str;
        CPPUNIT_ASSERT_EQUAL(str, str2);
        str += 'a';
        str2 += 'a';
        CPPUNIT_ASSERT_EQUAL(str, str2);
        str = str + 'b';
        str2 = str2 + 'b';
        CPPUNIT_ASSERT_EQUAL(str, str2);
        String add("Hello");
        str += add;
        str2 += add;
        CPPUNIT_ASSERT_EQUAL(str, str2);
        str = str + add;
        str2 = str2 + add;
        CPPUNIT_ASSERT_EQUAL(str, str2);
        const ichar *addStr = "Test";
        str += addStr;
        str2 += addStr;
        CPPUNIT_ASSERT_EQUAL(str, str2);
        str = str + addStr;
        str2 = str2 + addStr;
        CPPUNIT_ASSERT_EQUAL(str, str2);
        ichar c = 'c';
        str += c;
        str2 += c;
        CPPUNIT_ASSERT_EQUAL(str, str2);
        str = str + c;
        str2 = str2 + c;
        CPPUNIT_ASSERT_EQUAL(str, str2);
        CPPUNIT_ASSERT_EQUAL((Char) 'T', str[0]);
        CPPUNIT_ASSERT_EQUAL((Char) 'a', str[strInitialSize]);
        CPPUNIT_ASSERT_EQUAL((Char) 'b', str[strInitialSize + 1]);
        CPPUNIT_ASSERT_EQUAL((Char) 'H', str[strInitialSize + 2]);
        CPPUNIT_ASSERT_EQUAL((Char) 'T', str[strInitialSize + 12]);
    }
    {
        String str("Hello");
        String str2("How are you ?");
        CPPUNIT_ASSERT(str < str2);
    }
    {
        String str("ñ");
        String str2("z");
        CPPUNIT_ASSERT(str < str2);
    }
    {
        String str("é");
        String str2("j");
        CPPUNIT_ASSERT(str < str2);
    }
    {
        String str("This is á test");
        String str2("é");
        CPPUNIT_ASSERT_EQUAL(String("This is á testé"), str + L'é');
        CPPUNIT_ASSERT_EQUAL(String("This is á testé"), str + "é");
        CPPUNIT_ASSERT_EQUAL(String("This is á testéé"), str + str2 + L'é');
        CPPUNIT_ASSERT_EQUAL(String("This is á testéé"), str + str2 + "é");
    }
    {
        String str("/páth/");
        String str2("sómething.txt");
        CPPUNIT_ASSERT_EQUAL(String("/páth/sómething.txt"), str + str2);
    }
    {
        String str("/páth");
        String str2("/sómething.txt");
        CPPUNIT_ASSERT_EQUAL(String("/páth/sómething.txt"), str + str2);
    }
    {
        String str("Test");
        String str2("Test");
        CPPUNIT_ASSERT_EQUAL((size_t) 0, str.find(str2));
    }
    {
        String str("TéstTest");
        String str2("Test");
        CPPUNIT_ASSERT_EQUAL((size_t) 4, str.find(str2));
    }
    {
        String str("TéstTest");
        String str2("Kest");
        CPPUNIT_ASSERT_EQUAL((size_t) -1, str.find(str2));
    }
    {
        String str("Thisisalongtestwithspécialchársinside");
        String str2("spécialchárs");
        CPPUNIT_ASSERT_EQUAL((size_t) 19, str.find(str2));
    }
    {
        String str;
        Char a('a');
        for (iint32 i = 0; i < 10; ++i) {
            str += a;
        }
        CPPUNIT_ASSERT_EQUAL(String("aaaaaaaaaa"), str);
    }
    {
        String str;
        Char a(L'á');
        for (iint32 i = 0; i < 10; ++i) {
            str += a;
        }
        CPPUNIT_ASSERT_EQUAL(String("áááááááááá"), str);
    }
}

void StringTest::testToConversion()
{
    {
        String str;
        bool ok;
        CPPUNIT_ASSERT_EQUAL(0, str.toInt(&ok));
        CPPUNIT_ASSERT_EQUAL(false, ok);
    }
    {
        String str("Cannot convert");
        bool ok;
        CPPUNIT_ASSERT_EQUAL(0, str.toInt(&ok));
        CPPUNIT_ASSERT_EQUAL(false, ok);
    }
    {
        String str("123");
        bool ok;
        CPPUNIT_ASSERT_EQUAL(123, str.toInt(&ok));
        CPPUNIT_ASSERT_EQUAL(true, ok);
    }
    {
        // locale dependent operations
        setlocale(LC_ALL, "C");
        {
            String str("1.55");
            bool ok;
            CPPUNIT_ASSERT_EQUAL((float) 1.55, str.toFloat(&ok));
            CPPUNIT_ASSERT_EQUAL(true, ok);
        }
        {
            String str("1.55");
            bool ok;
            CPPUNIT_ASSERT_EQUAL((double) 1.55, str.toDouble(&ok));
            CPPUNIT_ASSERT_EQUAL(true, ok);
        }
    }
}

void StringTest::testNumber()
{
    CPPUNIT_ASSERT_EQUAL(String("15"), String::number(15));
    CPPUNIT_ASSERT_EQUAL(String("-15"), String::number(-15));
    CPPUNIT_ASSERT_EQUAL(String("50000000000"), String::number((iuint64) 50000000000ULL));
    CPPUNIT_ASSERT_EQUAL(String("-50000000000"), String::number((iint64) -50000000000ULL));
    CPPUNIT_ASSERT_EQUAL(String("10"), String::number(8, 8));
    CPPUNIT_ASSERT_EQUAL(String("12"), String::number(18, 16));
    CPPUNIT_ASSERT_EQUAL(String("16"), String::number(14, 8));
    CPPUNIT_ASSERT_EQUAL(String("100"), String::number(4, 2));
    CPPUNIT_ASSERT_EQUAL(String("1f"), String::number(31, 16));
    {
        // locale dependent operations
        setlocale(LC_ALL, "C");
        CPPUNIT_ASSERT_EQUAL(String("1.57"), String::number((float) 1.57));
        CPPUNIT_ASSERT_EQUAL(String("1.58"), String::number((float) 1.578));
        CPPUNIT_ASSERT_EQUAL(String("1.578"), String::number((float) 1.578, 'g', 4));
    }
}

void StringTest::testMisc()
{
    {
        String str("Test");
        CPPUNIT_ASSERT_EQUAL(String("Test"), str);
        str.clear();
        CPPUNIT_ASSERT_EQUAL(String(), str);
        str = "Test";
        CPPUNIT_ASSERT_EQUAL(String("Test"), str);
        str.prepend("This is a ");
        CPPUNIT_ASSERT_EQUAL(String("This is a Test"), str);
        str.setNumber(150);
        CPPUNIT_ASSERT_EQUAL(String("150"), str);
        str.append(" oranges");
        CPPUNIT_ASSERT_EQUAL(String("150 oranges"), str);
    }
}

String StringTest::returnSpecialChars()
{
    return "áéíóúñ€%32";
}

#include "test.h"
