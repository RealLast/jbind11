/***************************************************************************
* Copyright (C) 2023 ETH Zurich
* Core AI & Digital Biomarker, Acoustic and Inflammatory Biomarkers (ADAMMA)
* Centre for Digital Health Interventions (c4dhi.org)
* 
* Authors: Patrick Langer
* 
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
*         http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
***************************************************************************/

#pragma once

namespace jbind11
{
    template<typename T> struct is_integer_no_bool : public std::false_type {};
}

#define JBIND_IS_INTEGER_NO_BOOL(type) \
namespace jbind11\
{\
    template<>\
    struct is_integer_no_bool<type> : public std::true_type {};\
}

JBIND_IS_INTEGER_NO_BOOL(unsigned char)
JBIND_IS_INTEGER_NO_BOOL(signed char)

JBIND_IS_INTEGER_NO_BOOL(signed short)
JBIND_IS_INTEGER_NO_BOOL(unsigned short)

JBIND_IS_INTEGER_NO_BOOL(signed int)
JBIND_IS_INTEGER_NO_BOOL(unsigned int)

JBIND_IS_INTEGER_NO_BOOL(signed long)
JBIND_IS_INTEGER_NO_BOOL(unsigned long)

JBIND_IS_INTEGER_NO_BOOL(signed long long)
JBIND_IS_INTEGER_NO_BOOL(unsigned long long)


