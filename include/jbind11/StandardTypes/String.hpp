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

#pragma once

#include <jni.h>
#include "Caster/Caster.hpp"
#include "Traits/jbind11_is_specialization_of.hpp"
#include <string>


namespace jbind11
{
    template<class T>
    struct Caster<T, typename std::enable_if<std::is_same<T, std::string>::value>::type> 
    {        
        public:

            static std::string fromJavaObject(JNIEnv* env, jobject javaObject)
            {
                return JNIUtils::toStdString(env, static_cast<jstring>(javaObject));
            }

            static jobject cast(JNIEnv* env, std::string& string)
            {
                return JNIUtils::toJString(env, string);
            }

            static std::string canonicalTypeName()
            {
                return "java.lang.String";
            }
    };
}   

