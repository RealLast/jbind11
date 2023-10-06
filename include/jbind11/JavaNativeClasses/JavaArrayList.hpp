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
#include "JNIUtils/JNIUtils.hpp"
namespace jbind11
{
    // Good reference: https://stackoverflow.com/questions/33406527/returning-an-arraylist-of-string-from-native-java-to-jni

    class JavaArrayList
    {
        private:
            const std::string JAVA_CLASS_NAME = "java.util.ArrayList";

            jobject javaObject;

            void createNewArrayList(size_t initialCapacity)
            {
                // Call constructor ArrayList(Integer initialCapacity).
                // "I" is the signature of "Integer", we specify it to 
                // use the correct constructor.
                this->javaObject = 
                    JNIUtils::createObjectFromClassName(JNIUtils::getEnv(), JAVA_CLASS_NAME, "I", initialCapacity);
            }

            void createNewArrayList()
            {
                // "" means we use the standard constructor with no parameters. 
                this->javaObject = 
                    JNIUtils::createObjectFromClassName(JNIUtils::getEnv(), JAVA_CLASS_NAME, "");
            }

        public:
            JavaArrayList()
            {
                this->createNewArrayList();
            }

            JavaArrayList(size_t initialCapacity)
            {
                this->createNewArrayList(initialCapacity);
            }

            JavaArrayList(jobject javaObject) 
            {
                std::string className = JNIUtils::getNameOfClassOfObject(JNIUtils::getEnv(), javaObject);

                if(className != JAVA_CLASS_NAME)
                {
                    JBIND_THROW("Cannot create native JavaArrayList object from passed java object of type \"" << className << "\"."
                    << "Please make sure the jobject passed to JavaArrayList is of type \"" << JAVA_CLASS_NAME << "\"");
                }

                this->javaObject = javaObject;
            }

            void add(jobject objectToAdd)
            {
                JNIEnv* env = JNIUtils::getEnv();
                jmethodID arrayListAddMethodID = JNIUtils::getMethodID(env, this->javaObject, "add", "(Ljava/lang/Object;)Z");

                if(arrayListAddMethodID == nullptr)
                {
                    JBIND_THROW("Error, JavaArrayList.add(object) failed. Cannot find \"add\" function in Java object.\n"
                    "Is the internal java object really of type java.util.ArrayList?");
                }

                // ArrayList.add() always returns true, according to specification.
                // So we just ignore it.
                env->CallBooleanMethod(this->javaObject, arrayListAddMethodID, objectToAdd);
            }

            size_t size()
            {
                JNIEnv* env = JNIUtils::getEnv();
                jmethodID arrayListSizeMethodID = JNIUtils::getMethodID(env, this->javaObject, "size", "()I");

                if(arrayListSizeMethodID == nullptr)
                {
                    JBIND_THROW("Error, JavaArrayList.size() failed. Cannot find \"size\" function in Java object.\n"
                    "Is the internal java object really of type java.util.ArrayList?");
                }

                return env->CallIntMethod(this->javaObject, arrayListSizeMethodID);
            }

            jobject get(size_t index)
            {
                JNIEnv* env = JNIUtils::getEnv();
                jmethodID arrayListGetMethod = JNIUtils::getMethodID(env, this->javaObject, "get", "(I)Ljava/lang/Object;");

                if(arrayListGetMethod == nullptr)
                {
                    JBIND_THROW("Error, JavaArrayList.get(index) failed. Cannot find \"get\" function in Java object.\n"
                    "Is the internal java object really of type java.util.ArrayList?");
                }

                return env->CallObjectMethod(this->javaObject, arrayListGetMethod, (jint)index);
            }

            jobject getJavaObject()
            {
                return this->javaObject;
            }

    };

}