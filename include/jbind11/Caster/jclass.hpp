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
    // Caster if someone invokes sth like:
    /*
        jclass cls = env->findClass(name);
    

        jobject object2 = cast(cls);
        jclass cls2 = fromJavaObject(object2);
    */
    // NOTE!!!: This just casts jclass to jobject and the other way round.
    // jclass inherits from jobject and is just a pointer.
    // It DOES NOT (!!!) LOOK UP THE CLASS OF THE PASSED OBJECT
    // A similiar case as in the jobject caster. Check it out.

    template<typename T>
    struct Caster<T, typename std::enable_if<std::is_same<T, jclass>::value>::type> 
    {
        // T is jclass.
        public:
            static T fromJavaObject(JNIEnv* env, jobject javaObject)
            {
                return static_cast<jclass>(javaObject);
            }

            static jobject cast(JNIEnv* env, T& value)
            {
                return static_cast<jobject>(value);
            }

            static std::string canonicalTypeName()
            {
                return "java.lang.Class";
            }
            
            static bool isGeneric()
            {
                return false;
            }
    };
}