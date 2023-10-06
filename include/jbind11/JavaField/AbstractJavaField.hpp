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
    struct JavaHandle;
    // A JavaField stores the name of a field of a java class as well as
    // the pointer to member of the wrapped class (see JavaPrimitiveField and JavaObjectField).
    // It allows to access the member value of a wrapped JavaClass, if a JavaHandle to the associated class object is provided.
    // E.g.:
    /*
        struct TestStruct
        {
            int val;
        };

        class JavaClass<TestStruct>
        {
            JavaClass()
            {
                def_readwrite("val", &TestStruct::val);
            }
        }

        TestStruct instance;
        JavaHandle handle;
        
        // Automatically looks up JavaClass<TestStruct>
        handle.set(instance);

        JavaClass<TestStruct> javaClass = handle.getClass();
        AbstractJavaField field = javaClass.getField("val");

        // Get's instance.val and converts it to java.lang.Integer
        jobject value = field.getValueFromHandle(handle);        

    */
    class AbstractJavaField
    {  
        protected:
            std::string fieldName;

        public:
            
            AbstractJavaField()
            {
                
            }

            AbstractJavaField(const std::string& fieldName) : fieldName(fieldName)
            {

            }

            virtual ~AbstractJavaField()
            {

            }

            // Exctracts the C++ instance from the handle, uses the pointer to member to 
            // access the corresponding native variable and uses Caster<Class> to convert it to a java object.
            virtual jobject getValue(JNIEnv* env, JavaHandle javaHandle) = 0;

            // Exctracts the C++ instance from the handle, uses the pointer to member to 
            // access the corresponding native variable and uses Caster<Class> to assign the value of 
            // the passed java object to it.
            virtual void setValue(JNIEnv* env, JavaHandle handle, jobject object) = 0;

          //  virtual void* getPointerToMember(JavaHandle* handle);

            virtual std::string getGetterDeclaration() = 0;
            virtual std::string getSetterDeclaration() = 0;
        
    };
}