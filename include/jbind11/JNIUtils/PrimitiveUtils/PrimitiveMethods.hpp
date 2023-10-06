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

namespace jbind11
{
    namespace JNIUtils
    {
        template<typename T>
        static inline typename std::enable_if<is_integer_no_bool<T>::value && sizeof(T) == 1, T>::type // 8 bit, i.e. int8_t, uint8_t (char_8t)
        callPrimitiveMethod(JNIEnv* env, jobject& object, jmethodID methodID)
        {
            return env->CallByteMethod(object, methodID);;
        }

        template<typename T>
        static inline typename std::enable_if<is_integer_no_bool<T>::value && sizeof(T) == 2, T>::type // 16 bit, i.e. int16_t, uint16_t
        callPrimitiveMethod(JNIEnv* env, jobject& object, jmethodID methodID)
        {
            return env->CallShortMethod(object, methodID);
        }

        template<typename T>
        static inline typename std::enable_if<is_integer_no_bool<T>::value && sizeof(T) == 4, T>::type // 32 bit, i.e. int32_t, uint32_t
        callPrimitiveMethod(JNIEnv* env, jobject& object, jmethodID methodID)
        {
            return env->CallIntMethod(object, methodID);
        }

        template<typename T>
        static inline typename std::enable_if<is_integer_no_bool<T>::value && sizeof(T) == 8, T>::type // 64 bit, i.e. int64_t, uint64_t
        callPrimitiveMethod(JNIEnv* env, jobject& object, jmethodID methodID)
        {
            return env->CallLongMethod(object, methodID);
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, float>::value, T>::type
        callPrimitiveMethod(JNIEnv* env, jobject& object, jmethodID methodID)
        {
            return env->CallFloatMethod(object, methodID);
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, double>::value, T>::type
        callPrimitiveMethod(JNIEnv* env, jobject& object, jmethodID methodID)
        {
            return env->CallDoubleMethod(object, methodID);
        }

        // Note that Java's Character class is 16 bit. 
        // Further note: char16_t can neither be signed nor unsigned.
        template<typename T>
        static inline typename std::enable_if<std::is_same<T, char16_t>::value, T>::type
        callPrimitiveMethod(JNIEnv* env, jobject& object, jmethodID methodID)
        {
            return env->CallCharMethod(object, methodID);
        }
        template<typename T>
        static inline typename std::enable_if<std::is_same<T, bool>::value, T>::type
        callPrimitiveMethod(JNIEnv* env, jobject& object, jmethodID methodID)
        {
            return env->CallBooleanMethod(object, methodID);
        }
    }
}