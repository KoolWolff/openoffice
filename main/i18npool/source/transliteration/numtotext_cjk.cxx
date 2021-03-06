/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_i18npool.hxx"

#define TRANSLITERATION_ALL
#include <numtotext_cjk.hxx>
#include <bullet.h>
#include <com/sun/star/i18n/NativeNumberMode.hpp>

namespace com { namespace sun { namespace star { namespace i18n {

#define TRANSLITERATION_NUMTOTEXT( name, number ) \
NumToText##name::NumToText##name() \
{ \
        nNativeNumberMode = number; \
        tableSize = 0; \
        transliterationName = "NumToText"#name; \
        implementationName = "com.sun.star.i18n.Transliteration.NumToText"#name; \
}
using namespace com::sun::star::i18n::NativeNumberMode;

TRANSLITERATION_NUMTOTEXT( Lower_zh_CN, NATNUM4)
TRANSLITERATION_NUMTOTEXT( Upper_zh_CN, NATNUM5)
TRANSLITERATION_NUMTOTEXT( Lower_zh_TW, NATNUM4)
TRANSLITERATION_NUMTOTEXT( Upper_zh_TW, NATNUM5)
TRANSLITERATION_NUMTOTEXT( Fullwidth_zh_CN, NATNUM6)
TRANSLITERATION_NUMTOTEXT( Fullwidth_zh_TW, NATNUM6)
TRANSLITERATION_NUMTOTEXT( Fullwidth_ja_JP, NATNUM6)
TRANSLITERATION_NUMTOTEXT( Fullwidth_ko, NATNUM6)
TRANSLITERATION_NUMTOTEXT( FormalLower_ko, NATNUM4)
TRANSLITERATION_NUMTOTEXT( FormalUpper_ko, NATNUM5)
TRANSLITERATION_NUMTOTEXT( FormalHangul_ko, NATNUM10)
TRANSLITERATION_NUMTOTEXT( InformalLower_ko, NATNUM7)
TRANSLITERATION_NUMTOTEXT( InformalUpper_ko, NATNUM8)
TRANSLITERATION_NUMTOTEXT( InformalHangul_ko, NATNUM11)
TRANSLITERATION_NUMTOTEXT( KanjiLongTraditional_ja_JP, NATNUM5)
TRANSLITERATION_NUMTOTEXT( KanjiLongModern_ja_JP, NATNUM4)
TRANSLITERATION_NUMTOTEXT( Date_zh, NATNUM7)
TRANSLITERATION_NUMTOTEXT( KanjiShortTraditional_ja_JP, NATNUM8)
TRANSLITERATION_NUMTOTEXT( KanjiShortModern_ja_JP, NATNUM7)
#undef TRANSLITERATION_NUMTOTEXT

#define TRANSLITERATION_NUMTOTEXT( name, _table, recycle ) \
NumToText##name::NumToText##name() \
{ \
        table = _table;\
        tableSize = sizeof(_table) / sizeof(sal_Unicode); \
        recycleSymbol = recycle; \
        transliterationName = "NumToText"#name; \
        implementationName = "com.sun.star.i18n.Transliteration.NumToText"#name; \
}

TRANSLITERATION_NUMTOTEXT ( AIUFullWidth_ja_JP, table_AIUFullWidth_ja_JP, sal_True)
TRANSLITERATION_NUMTOTEXT ( AIUHalfWidth_ja_JP, table_AIUHalfWidth_ja_JP, sal_True)
TRANSLITERATION_NUMTOTEXT ( IROHAFullWidth_ja_JP, table_IROHAFullWidth_ja_JP, sal_True)
TRANSLITERATION_NUMTOTEXT ( IROHAHalfWidth_ja_JP, table_IROHAHalfWidth_ja_JP, sal_True)
TRANSLITERATION_NUMTOTEXT ( CircledNumber, table_CircledNumber, sal_False)
TRANSLITERATION_NUMTOTEXT ( TianGan_zh, table_TianGan_zh, sal_False)
TRANSLITERATION_NUMTOTEXT ( DiZi_zh, table_DiZi_zh, sal_False)
TRANSLITERATION_NUMTOTEXT ( HangulJamo_ko, table_HangulJamo_ko, sal_True)
TRANSLITERATION_NUMTOTEXT ( HangulSyllable_ko, table_HangulSyllable_ko, sal_True)
TRANSLITERATION_NUMTOTEXT ( HangulCircledJamo_ko, table_HangulCircledJamo_ko, sal_True)
TRANSLITERATION_NUMTOTEXT ( HangulCircledSyllable_ko, table_HangulCircledSyllable_ko, sal_True)
#undef TRANSLITERATION_NUMTOTEXT

} } } }
