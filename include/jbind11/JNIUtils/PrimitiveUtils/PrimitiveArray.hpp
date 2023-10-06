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
#include <string>
#include "Traits/is_integer_no_bool.hpp"

namespace jbind11
{
    namespace JNIUtils
    {
        template<typename T>
        static inline typename std::enable_if<is_integer_no_bool<T>::value && sizeof(T) == 1>::type // 8 bit, i.e. int8_t, uint8_t (char_8t)
        getPrimitiveArrayRegion(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            env->GetByteArrayRegion(static_cast<jbyteArray>(object), start, length, reinterpret_cast<jbyte*>(destination));
        }

        template<typename T>
        static inline typename std::enable_if<is_integer_no_bool<T>::value && sizeof(T) == 2>::type // 16 bit, i.e. int16_t, uint16_t
        getPrimitiveArrayRegion(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            env->GetShortArrayRegion(static_cast<jshortArray>(object), start, length, reinterpret_cast<jshort*>(destination));
        }

        template<typename T>
        static inline typename std::enable_if<is_integer_no_bool<T>::value && sizeof(T) == 4>::type // 32 bit, i.e. int32_t, uint32_t
        getPrimitiveArrayRegion(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            env->GetIntArrayRegion(static_cast<jintArray>(object), start, length, reinterpret_cast<jint*>(destination));
        }

        template<typename T>
        static inline typename std::enable_if<is_integer_no_bool<T>::value && sizeof(T) == 8>::type // 64 bit, i.e. int64_t, uint64_t
        getPrimitiveArrayRegion(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            env->GetLongArrayRegion(static_cast<jlongArray>(object), start, length, reinterpret_cast<jlong*>(destination));
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, float>::value>::type
        getPrimitiveArrayRegion(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            env->GetFloatArrayRegion(static_cast<jfloatArray>(object), start, length, destination);
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, double>::value>::type
        getPrimitiveArrayRegion(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            env->GetDoubleArrayRegion(static_cast<jdoubleArray>(object), start, length, destination);
        }

        // Note that Java's Character class is 16 bit. 
        // Further note: char16_t can neither be signed nor unsigned.
        template<typename T>
        static inline typename std::enable_if<std::is_same<T, char16_t>::value>::type
        getPrimitiveArrayRegion(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            env->GetCharArrayRegion(static_cast<jcharArray>(object), start, length, destination);
        }
        template<typename T>
        static inline typename std::enable_if<std::is_same<T, bool>::value>::type
        getPrimitiveArrayRegion(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            env->GetBooleanArrayRegion(static_cast<jbooleanArray>(object), start, length, destination);
        }
    }
}
// case 1: // 8 bit, i.e. int8_t, uint8_t (char_8t)
// {
//     env->GetByteArrayRegion(static_cast<jbyteArray>(object), start, length, destination);
// }
// break;
// case 2: // 16 bit, i.e. int16_t, uint16_t
// {
//     env->GetShortArrayRegion(static_cast<jshortArray>(object), start, length, destination);
// }
// break;
// case 4: // 32 bit, i.e. int32_t, uint32_t
// {
//     env->GetIntArrayRegion(static_cast<jintArray>(object), start, length, destination);
// }
// break;
// case 8: // 64 bit, i.e. int64_t, uint64_t
// {
//     env->GetLongArrayRegion(static_cast<jlongArray>(object), start, length, destination);
// }
// break;