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
#include "BasicUtils.hpp"
namespace jbind11
{
    namespace JNIUtils
    {
        static jfieldID getFieldOfClassOfObject(JNIEnv* env, jobject javaObject, const std::string& fieldName, const std::string& signature)
        {
            jclass cls = JNIUtils::getClassOfObject(env, javaObject); 
            jfieldID fieldID = env->GetFieldID(cls, fieldName.c_str(), signature.c_str());
            env->DeleteLocalRef(cls);

            return fieldID;
        }

        static bool hasClassOfObjectField(JNIEnv* env, jobject javaObject, const std::string& fieldName, const std::string& signature)
        {
            return getFieldOfClassOfObject(env, javaObject, fieldName, signature) != nullptr;
        }

        

        
    }
}