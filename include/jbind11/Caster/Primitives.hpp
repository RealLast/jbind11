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

#include <jni.h>
#include "Caster.hpp"


/*
References: https://msoe.us/taylor/tutorial/ce2810/csimilar
https://www.geeksforgeeks.org/data-types-in-java/

Be aware, that in C++, we can only really rely on types like uint8_t, int8_t to have a certain number of bits.
Normal integer types like short, int or long can be different, depening on System and compiler (e.g., Linux vs. Unix)
See here: https://en.cppreference.com/w/cpp/language/types
Since uint8_t, uint16_t.. and others are just typdefs of char, int, long etc. depending on the architecture,
we use them (uint8_t, uint16_t, ..) in the template specializations to automatically choose the correct underlying type (char, short, int)..
Note, that Java's Character data type does NOT corresponds to C++'s char type, since it has 16 bits in Java.
The char type in C++ has 8 bits, thus corresponds to Java's byte type.
Java's character type, however, can be matched to C++'s char_16t.



Java Primitive Data Types:

Type        Content             Memory                      Min Value                                   Max Value       Matching C++ type
byte 	    integer 	        8 bits 	                         -128 	                                      127       signed/unsigned char
short 	    integer 	        16 bits 	                  -32,768 	                                   22,767       int16_t/uint16_t
int 	    integer 	        32 bits 	           -2,147,483,648 	                            2,147,483,647       int32_t/uint32_t
long 	    integer 	        64 bits    -9,223,372,036,854,775,808 	                9,223,372,036,854,775,808       int64_t/uint64_t
float 	    real 	            32 bits 	                  -3.4E38 	                                   3.4E38       float
double 	    real 	            64 bits 	                 -1.8E308 	                                  1.8E308       double

Character   unicode characters  16 bits(does not match C++'s char type!)                                                char16_t
Boolean     true, false                                                                                                 bool

*/

#include "JNIUtils/JNIUtils.hpp"
#include "TypeName.hpp"
#include "jbind11_throw.hpp"

namespace jbind11
{
    template<typename T>
    struct Caster<T, typename std::enable_if<std::is_arithmetic<T>::value>::type> 
    {
        public:
            static T fromJavaObject(JNIEnv* env, jobject javaObject)
            {
                return JNIUtils::javaPrimitiveToNativePrimitive<T>(env, javaObject);
            }

            static jobject cast(JNIEnv* env, T& value)
            {
                // Converts C++ int to java.lang.Integer, C++ float to java.lang.Float and so on.
                std::string primitiveClassName  =   JNIUtils::getJavaClassNameOfPrimitiveType<T>();
                std::string signature           =   JNIUtils::getSignatureOfPrimitiveType<T>();

                jobject javaObject = 
                    JNIUtils::createObjectFromClassName(env, primitiveClassName, signature, value);
                return javaObject;
            }

            static std::string canonicalTypeName()
            {
                return JNIUtils::getJavaClassNameOfPrimitiveType<T>();
            }

            static bool isGeneric()
            {
                return false;
            }
    };
}