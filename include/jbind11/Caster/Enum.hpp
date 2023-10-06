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


#include "JNIUtils/JNIUtils.hpp"
#include "TypeName.hpp"
#include "jbind11_throw.hpp"

namespace jbind11
{
    template<typename T>
    struct Caster<T, typename std::enable_if<std::is_enum<T>::value>::type> 
    {
        private:
            typedef int32_t IntegerType;
        public:
            static T fromJavaObject(JNIEnv* env, jobject javaObject)
            {
                // Treat enum as integer.
                return static_cast<T>(Caster<IntegerType>::fromJavaObject(env, javaObject));
            }

            static jobject cast(JNIEnv* env, T& value)
            {
                IntegerType copy = static_cast<IntegerType>(value);
                // Treat enum as integer.
                return Caster<IntegerType>::cast(env, copy);
            }

            static std::string canonicalTypeName()
            {
                return Caster<IntegerType>::canonicalTypeName();
            }

            static bool isGeneric()
            {
                return false;
            }
    };
}