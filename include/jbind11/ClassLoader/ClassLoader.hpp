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
#include "JavaPackage/JavaPackageManager.hpp"
namespace jbind11
{
    class ClassLoader
    {
        // Why use custom findClass?
        // See https://stackoverflow.com/questions/13263340/findclass-from-any-thread-in-android-jni/16302771#16302771

        private:
            JavaVM* jvm = nullptr;
            jobject gClassLoader;
            jmethodID gFindClassMethod;
            bool onLoadCalled = false;

        public:
            // See https://stackoverflow.com/questions/13263340/findclass-from-any-thread-in-android-jni/16302771#16302771
            void onLoad(JavaVM* javaVM)
            {
                this->jvm = javaVM;

                JNIEnv* env = getEnv();

                // ONLY use NON-SYSTEM class!!
                // **** !!!! READ COMMENTS OF STACKOVERFLOW ANSWER CAREFULLY !!!! ****
                /*
                
                    "Also, for people are implementing this, be aware that the ClassLoader you extract (the one that knows about your classes) 
                    does NOT know about certain Android classes like android.app.Activity. 
                    So you really have to try both in any general-purpose findClass routine. 
                    SIGH---could they make JNI any harder to use?" 
                    (Quote from comments below the stackoverflow answer).

                */
                std::string className = "jbind11/JBindWrapper";
                auto randomClass = env->FindClass(className.c_str());
                if(randomClass == nullptr)
                {
                    JBIND_THROW("Error in jbind11::ClassLoader::onLoad. Cannot find class \"" << className << "\" therefore cannot retrieve ClassLoader.")
                }

                jclass classClass = env->GetObjectClass(randomClass);

                auto classLoaderClass = env->FindClass("java/lang/ClassLoader");
                auto getClassLoaderMethod = env->GetMethodID(classClass, "getClassLoader",
                                                        "()Ljava/lang/ClassLoader;");

                jobject tmpClassLoader = env->CallObjectMethod(randomClass, getClassLoaderMethod);
                gClassLoader = env->NewGlobalRef(tmpClassLoader);
                gFindClassMethod = env->GetMethodID(classLoaderClass, "findClass",
                                                "(Ljava/lang/String;)Ljava/lang/Class;");
                onLoadCalled = true;

            }

            jclass findClass(JNIEnv* env, const char* name) 
            {
                if(!onLoadCalled)
                {
                    JBIND_THROW("Error, JNIUtils::findClass was called without prior call to onLoad. Please make sure that jbind11::ClassLoader::onLoad "
                    << "was called in JNI_OnLoad in native code.");
                }


                if(strlen(name) == 0)
                {
                    JBIND_THROW("Error, cannot find class, string is empty (provide a non-empty class name");
                }
                // This would cause FindClass to throw an unrecoverable exception..
                if(!std::isalnum(name[strlen(name) - 1]))
                {
                    JBIND_THROW("Error, cannot find class \"" << name << "\". The specified class ends with non alpha-numeric charachter '"
                    << name[strlen(name) - 1] << "', which is not allowed");
                }



                jstring str = env->NewStringUTF(name);


                jclass cls;
                
                try{
                    cls = static_cast<jclass>(env->CallObjectMethod(gClassLoader, gFindClassMethod, str));
                }
                catch(std::exception& e)
                {
                }
                env->DeleteLocalRef(str);

                if(cls == nullptr)
                {
                    // If failed, try conventional method.
                    env->ExceptionClear();


                    cls = env->FindClass(name);
                }

          
                
 
                return cls;
            }

            JNIEnv* getEnv() 
            {
                if(jvm == nullptr)
                {
                    JBIND_THROW("Error, ClassLoader::getEnv() was called without prior call to onLoad. Please make sure that jbind11::ClassLoader::onLoad "
                    << "was called in JNI_OnLoad in native code. To do so, please include \"jbind11/jbind11_main.hpp\" in your main source file.");
                }
                JNIEnv *env;
                int status = jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
                if(status < 0) {    
                    #ifdef __ANDROID__
                    status = jvm->AttachCurrentThread(&env, NULL);
                    #else
                    status = jvm->AttachCurrentThread((void**)&env, NULL);
                    #endif
                    if(status < 0) {        
                        return nullptr;
                    }
                }
                return env;
            }

    };

    inline ClassLoader& getClassLoader()
    {
        static ClassLoader* classLoader = new ClassLoader();
        return *classLoader;
    }
}

