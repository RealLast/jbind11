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
#include <string>
#include "StringUtils.hpp"
namespace jbind11
{
    namespace JNIUtils
    {
        // Returns the class name of a given object of type jclass.
        // Not, the object needs to be a Class object in Java!
        // E.g. String.class would work. Its NOT an instance of String in that case.
        // If it was a native function (called from java), you'd call it like getClassName(String.class).
        // If you want to get the class name of an object, make sure to get it's class first (i.e.: env->GetObjectClass(env, object)),
        // do not pass the object to this function
        static inline std::string getClassName(JNIEnv* env, jclass dataType)
        {
            // Find base class Class to call it's getName function.

            std::string className = "java/lang/Class";
            jclass ccls = env->FindClass("java/lang/Class");
        
            jmethodID mid_getName = env->GetMethodID(ccls, "getName", "()Ljava/lang/String;");
            jstring strObj = (jstring)env->CallObjectMethod(dataType, mid_getName);

            std::string stdString = JNIUtils::toStdString(env, strObj);

            env->DeleteLocalRef(ccls);
            env->DeleteLocalRef(strObj);

            return stdString;
        }

        static inline jclass getClassOfObject(JNIEnv* env, jobject object)
        {
            return env->GetObjectClass(object);
        }

        static inline std::string getNameOfClassOfObject(JNIEnv* env, jobject object)
        {
            jclass cls = getClassOfObject(env, object);
            std::string className = getClassName(env, cls);
            env->DeleteLocalRef(cls);

            return className;
        }

        static inline jmethodID getMethodID(JNIEnv* env, jobject object, const char* methodName, const char* signature)
        {
            jclass cls = getClassOfObject(env, object);
            jmethodID methodID = env->GetMethodID(cls, methodName, signature);
            env->DeleteLocalRef(cls);
            return methodID;
        }

        static inline jclass getSuperClass(JNIEnv* env, jclass cls)
        {
            jclass superClass = env->GetSuperclass(cls);

            return superClass;
        }

        static inline std::string getNameOfSuperClass(JNIEnv* env, jclass cls)
        {
            jclass superClass = getSuperClass(env, cls);
            std::string className = getClassName(env, superClass);
            env->DeleteLocalRef(superClass);

            return className;
        }

        static inline std::string getNameOfSuperClassOfObject(JNIEnv* env, jobject object)
        {
            jclass cls = getClassOfObject(env, object);

            std::string className = getNameOfSuperClass(env, cls);
            env->DeleteLocalRef(cls);

            return className;
        }

        static inline std::vector<std::string> getInheritanceList(JNIEnv* env, jclass cls)
        {
            std::vector<std::string> inheritanceList;

            std::string className = getClassName(env, cls);
            inheritanceList.push_back(className);


            jclass superClass = getSuperClass(env, cls);
            jclass superSuperClass;

            while(className != "java.lang.Object")
            {
                className = getClassName(env, superClass);
                inheritanceList.push_back(className);

                jclass superSuperClass = getSuperClass(env, superClass);
                env->DeleteLocalRef(superClass);

                superClass = superSuperClass;
            }

            env->DeleteLocalRef(superClass);

            return inheritanceList;
        }

        static inline std::vector<std::string> getInheritanceListOfObject(JNIEnv* env, jobject object)
        {
            std::vector<std::string> inheritanceList;
            jclass cls = getClassOfObject(env, object);
            inheritanceList = getInheritanceList(env, cls);
            env->DeleteLocalRef(cls);

            return inheritanceList;
        }

        
    }
}