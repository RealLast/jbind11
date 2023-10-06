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

#include "PrimitiveGetterFunctions.hpp"
#include "PrimitiveSignatures.hpp"
#include "PrimitiveMethods.hpp"
#include "TypeName.hpp"
#include "jbind11_throw.hpp"

namespace jbind11
{
    namespace JNIUtils
    {
        template<typename T>
        T javaPrimitiveToNativePrimitive(JNIEnv* env, jobject javaObject)
        {
            std::string className = JNIUtils::getNameOfClassOfObject(env, javaObject);
            std::string expectedJavaPrimitiveClassName = JNIUtils::getJavaClassNameOfPrimitiveType<T>();

            // Does java object match the primitive?
            if(className != expectedJavaPrimitiveClassName)
            {
                JBIND_THROW("Error, cannot convert javaObject to native C++ primitive " << TypeName<T>::get() << "."
                << "Java object is of type \"" << className << "\", however an object of type \"" << expectedJavaPrimitiveClassName << "\" was expected.");
            }

            // Get the function of the java object that can be used to retrieve the primitive (e.g., integerValue, floatValue, ... as in
            // Integer(42).integerValue() etc.)
            std::string signature = "()" + JNIUtils::getSignatureOfPrimitiveType<T>();
            std::string getterMethodName = JNIUtils::getGetterFunctionNameUsedToRetrievePrimitiveFromJavaObject<T>().c_str();

            jclass objectClass = JNIUtils::getClassOfObject(env, javaObject);
            jmethodID mGetValue = env->GetMethodID(objectClass, getterMethodName.c_str(), signature.c_str());
            env->DeleteLocalRef(objectClass);
            if(mGetValue == NULL)
            {
                JBIND_THROW("Error, cannot convert java object of type \"" << className << "\" to native C++ primitive \"" << TypeName<T>::get() << "\"."
                << "Method \"" << getterMethodName << "\" was not found in object of type \"" << className << "\"");
            } 

            return JNIUtils::callPrimitiveMethod<T>(env, javaObject, mGetValue);
        }
    }    
}