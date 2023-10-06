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
#include "BasicUtils.hpp"
#include <jni.h>

namespace jbind11
{
    namespace JNIUtils
    {
        static inline bool isJavaByteObject(JNIEnv* env, jobject data)
        {
            std::string className = getNameOfClassOfObject(env, data);
            return className == "java.lang.Byte";
        }

        static inline bool isJavaShortObject(JNIEnv* env, jobject data)
        {
            std::string className = getNameOfClassOfObject(env, data);
            return className == "java.lang.Short";
        }

        static inline bool isJavaIntegerObject(JNIEnv* env, jobject data)
        {
            std::string className = getNameOfClassOfObject(env, data);
            return className == "java.lang.Integer";
        }

        static inline bool isJavaLongObject(JNIEnv* env, jobject data)
        {
            std::string className = getNameOfClassOfObject(env, data);
            return className == "java.lang.Long";
        }

        static inline bool isJavaFloatObject(JNIEnv* env, jobject data)
        {
            std::string className = getNameOfClassOfObject(env, data);
            return className == "java.lang.Float";
        }

        static inline bool isJavaDoubleObject(JNIEnv* env, jobject data)
        {
            std::string className = getNameOfClassOfObject(env, data);
            return className == "java.lang.Double";
        }

        static inline bool isJavaBooleanObject(JNIEnv* env, jobject data)
        {
            std::string className = getNameOfClassOfObject(env, data);
            return className == "java.lang.Boolean";
        }

        static inline bool isJavaCharObject(JNIEnv* env, jobject data)
        {
            std::string className = getNameOfClassOfObject(env, data);
            return className == "java.lang.Character";
        }

        static inline bool isJavaStringObject(JNIEnv* env, jobject data)
        {
            std::string className = getNameOfClassOfObject(env, data);
            return className == "java.lang.String";
        }

        static inline bool isJavaStringBuilderObject(JNIEnv* env, jobject data)
        {
            std::string className = getNameOfClassOfObject(env, data);
            return className == "java.lang.StringBuilder";
        }
    }
}