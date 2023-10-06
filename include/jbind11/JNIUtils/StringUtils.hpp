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

#include <string>
#include <jni.h>

namespace jbind11
{
    namespace JNIUtils
    {
        static std::string toStdString(JNIEnv *env, jobject jStr)
        {
            return toStdString(env, static_cast<jstring>(jStr));
        }

        static std::string toStdString(JNIEnv *env, jstring jStr)
        {
            if (!jStr)
                    return "";

            const jclass stringClass = env->GetObjectClass(jStr);
            const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
            jstring characterSetString =  env->NewStringUTF("UTF-8");
            const jbyteArray stringJbytes = (jbyteArray) env->CallObjectMethod(jStr, getBytes, characterSetString);

            size_t length = (size_t) env->GetArrayLength(stringJbytes);
            jbyte* pBytes = env->GetByteArrayElements(stringJbytes, NULL);

            std::string ret = std::string((char*) pBytes, length);
            env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

            env->DeleteLocalRef(stringJbytes);
            env->DeleteLocalRef(stringClass);
            env->DeleteLocalRef(characterSetString);
            return ret;
        }

        static jstring toJString(JNIEnv* env, const std::string& nativeString)
        {
            return env->NewStringUTF(nativeString.c_str());
        }

        static void stringReplaceAll(std::string& str, const std::string from, const std::string to)
        {
            size_t start_pos = 0;
            while((start_pos = str.find(from, start_pos)) != std::string::npos) 
            {
                str.replace(start_pos, from.length(), to);
                start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
            }
        }

    }
}