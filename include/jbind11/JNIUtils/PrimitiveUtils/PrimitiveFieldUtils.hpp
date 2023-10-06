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
#include "JNIUtils/BasicUtils.hpp"
#include "JNIUtils/PrimitiveUtils/PrimitiveSignatures.hpp"
namespace jbind11
{
    namespace JNIUtils
    {
        // === GETTERS
        template<typename T>
        static inline typename std::enable_if<is_integer_no_bool<T>::value && sizeof(T) == 1, T>::type // 8 bit, i.e. int8_t, uint8_t (char_8t)
        getPrimitiveField(JNIEnv* env, jobject& object, jfieldID fieldID)
        {
            return env->GetByteField(object, fieldID);
        }

        template<typename T>
        static inline typename std::enable_if<is_integer_no_bool<T>::value && sizeof(T) == 2, T>::type // 16 bit, i.e. int16_t, uint16_t
        getPrimitiveField(JNIEnv* env, jobject& object, jfieldID fieldID)
        {
            return env->GetShortField(object, fieldID);
        }

        template<typename T>
        static inline typename std::enable_if<is_integer_no_bool<T>::value && sizeof(T) == 4, T>::type // 32 bit, i.e. int32_t, uint32_t
        getPrimitiveField(JNIEnv* env, jobject& object, jfieldID fieldID)
        {
            return env->GetIntField(object, fieldID);
        }

        template<typename T>
        static inline typename std::enable_if<is_integer_no_bool<T>::value && sizeof(T) == 8, T>::type // 64 bit, i.e. int64_t, uint64_t
        getPrimitiveField(JNIEnv* env, jobject& object, jfieldID fieldID)
        {
            return env->GetLongField(object, fieldID);
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, float>::value, T>::type
        getPrimitiveField(JNIEnv* env, jobject& object, jfieldID fieldID)
        {
            return env->GetFloatField(object, fieldID);
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, double>::value, T>::type
        getPrimitiveField(JNIEnv* env, jobject& object, jfieldID fieldID)
        {
            return env->GetDoubleField(object, fieldID);
        }

        // Note that Java's Character class is 16 bit. 
        // Further note: char16_t can neither be signed nor unsigned.
        template<typename T>
        static inline typename std::enable_if<std::is_same<T, char16_t>::value, T>::type
        getPrimitiveField(JNIEnv* env, jobject& object, jfieldID fieldID)
        {
            return env->GetFloatField(object, fieldID);
        }
        template<typename T>
        static inline typename std::enable_if<std::is_same<T, bool>::value, T>::type
        getPrimitiveField(JNIEnv* env, jobject& object, jfieldID fieldID)
        {
            return env->GetBooleanField(object, fieldID);
        }


        // === SETTERS
        template<typename T>
        static inline typename std::enable_if<is_integer_no_bool<T>::value && sizeof(T) == 1>::type // 8 bit, i.e. int8_t, uint8_t (char_8t)
        setPrimitiveField(JNIEnv* env, jobject& object, jfieldID fieldID, T value)
        {
            return env->SetByteField(object, fieldID, value);
        }

        template<typename T>
        static inline typename std::enable_if<is_integer_no_bool<T>::value && sizeof(T) == 2>::type // 16 bit, i.e. int16_t, uint16_t
        setPrimitiveField(JNIEnv* env, jobject& object, jfieldID fieldID, T value)
        {
            return env->SetShortField(object, fieldID, value);
        }

        template<typename T>
        static inline typename std::enable_if<is_integer_no_bool<T>::value && sizeof(T) == 4>::type // 32 bit, i.e. int32_t, uint32_t
        setPrimitiveField(JNIEnv* env, jobject& object, jfieldID fieldID, T value)
        {
            return env->SetIntField(object, fieldID, value);
        }

        template<typename T>
        static inline typename std::enable_if<is_integer_no_bool<T>::value && sizeof(T) == 8>::type // 64 bit, i.e. int64_t, uint64_t
        setPrimitiveField(JNIEnv* env, jobject& object, jfieldID fieldID, T value)
        {
            return env->SetLongField(object, fieldID, value);
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, float>::value>::type
        setPrimitiveField(JNIEnv* env, jobject& object, jfieldID fieldID, T value)
        {
            return env->SetFloatField(object, fieldID, value);
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, double>::value>::type
        setPrimitiveField(JNIEnv* env, jobject& object, jfieldID fieldID, T value)
        {
            return env->SetDoubleField(object, fieldID, value);
        }

        // Note that Java's Character class is 16 bit. 
        // Further note: char16_t can neither be signed nor unsigned.
        template<typename T>
        static inline typename std::enable_if<std::is_same<T, char16_t>::value>::type
        setPrimitiveField(JNIEnv* env, jobject& object, jfieldID fieldID, T value)
        {
            return env->SetFloatField(object, fieldID, value);
        }
        template<typename T>
        static inline typename std::enable_if<std::is_same<T, bool>::value>::type
        setPrimitiveField(JNIEnv* env, jobject& object, jfieldID fieldID, T value)
        {
            return env->SetBooleanField(object, fieldID, value);
        }


    }
}