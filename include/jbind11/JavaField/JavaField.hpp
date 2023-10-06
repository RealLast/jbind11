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
#include "JavaHandle/JavaHandle.hpp"

#include "JBindWrapper/GetterSetterGenerator.hpp"
#include "jbind11/cast.hpp"

namespace jbind11
{
    template<typename Class, typename T, class Enable = void>
    class JavaField : public AbstractJavaField 
    {
        private:
            T Class::*ptr;

        public:
            JavaField() : AbstractJavaField("")
            {

            }

            JavaField(const std::string& fieldName, T Class::*ptr) : AbstractJavaField(fieldName), ptr(ptr)
            {

            }

            // Exctracts the C++ instance from the handle, and uses the pointer to member to 
            // convert the member to a Java object.
            virtual jobject getValue(JNIEnv* env, JavaHandle javaHandle)
            {
                Class* instance = javaHandle.getNativeData<Class>();
                return get(env, *instance);
            }
     
            virtual jobject get(JNIEnv* env, Class& instance)
            {
                T& value = instance.*ptr;
                return Caster<T>::cast(env, value);
            }

            virtual void setValue(JNIEnv* env, JavaHandle javaHandle, jobject object)
            {
                Class* instance = javaHandle.getNativeData<Class>();
                set(env, *instance, object);
            }

            virtual void set(JNIEnv* env, Class& instance, jobject object)
            {
                T& value = instance.*ptr;
                value = Caster<T>::fromJavaObject(env, object);
            }

            virtual std::string getFieldDeclaration()
            {
                return this->fieldName;
            }

            virtual std::string getGetterDeclaration()
            {
                return GetterGenerator<T>::generate(this->fieldName);
            }

            virtual std::string getSetterDeclaration()
            {
                return SetterGenerator<T>::generate(this->fieldName);
            }


         
    };
}