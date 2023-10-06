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
        jobject object = ...;

        jobject object2 = cast(object);
        jobject object2 = fromJavaObject(object);
    */
    // It's kinda non-sensical, but can happen for example in function definitions.
    // If the user wants to define define a function that returns a jobject,
    // which is then not casted to a specific C++ type.
    // Example:
    /*
        struct TestStruct
        {
            // Here, we directly return a jobject.
            jobject generateObject(...)
            {
                jobject object = createObjectFromClassName(...);
                ...
                return 
            }
        };

        JBIND11_PACKAGE(TestPackage, p)
        {
            JavaClass<TestStruct> cls(p, "TestStructr");
            cls.def("generateObject", &TestStruct::generateObject)
        }
    */
    template<typename T>
    struct Caster<T, typename std::enable_if<std::is_same<T, jobject>::value>::type> 
    {
        // T is jobject.
        public:
            static T fromJavaObject(JNIEnv* env, jobject javaObject)
            {
                return javaObject;
            }

            static jobject cast(JNIEnv* env, T& value)
            {
                return value;
            }

            static std::string canonicalTypeName()
            {
                return "java.lang.Object";
            }
            
            static bool isGeneric()
            {
                return false;
            }
    };
}