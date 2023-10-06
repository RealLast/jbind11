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
#include "ClassLoader/ClassLoader.hpp"
#include "StringUtils.hpp"
namespace jbind11
{
    namespace JNIUtils
    {
        template<typename... Parameters>
        static jobject createObjectFromClassName(JNIEnv* env, std::string className, std::string constructorParameters, Parameters... parameters)
        {  
            std::string javaClassName = className;
            stringReplaceAll(javaClassName, ".", "/");
            jclass cls = getClassLoader().findClass(env, javaClassName.c_str());

            if(cls == nullptr)
            {
                JBIND_THROW("Cannot create java object, failed to lookup class " << className);
            }

            std::string constructorSignature = std::string("(") + constructorParameters + std::string(")V");
            jmethodID constructor = env->GetMethodID(cls, "<init>", constructorSignature.c_str());

            if(constructor == nullptr)
            {
                JBIND_THROW("Cannot create java object, failed to lookup constructor for class " << className);
            }

            jobject object = env->NewObject(cls, constructor, parameters...);

            if(object == nullptr)
            {
                JBIND_THROW("Cannot create java object, object with class signature " << className << " was found, but could not be created.");
            }

            env->DeleteLocalRef(cls);

            return object;
        }
    }
}