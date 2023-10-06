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
    class Consumer
    {
        private:
            std::string JAVA_CLASS_NAME = "java.util.function.Consumer";


            struct ReferenceHolder
            {
                private:
                    jobject javaObjectReference;

                
                public:
                    ReferenceHolder(jobject javaObject)
                    {
                        this->javaObjectReference = JNIUtils::getEnv()->NewGlobalRef(javaObject);
                    }

                    ~ReferenceHolder()
                    {
                        JNIUtils::getEnv()->DeleteGlobalRef(javaObjectReference);
                    }

                    jobject getJavaObject()
                    {
                        return this->javaObjectReference;
                    }
            };

            std::shared_ptr<ReferenceHolder> referenceHolder;


            

        public:
            Consumer()
            {
            }

         
            Consumer(jobject javaObject) 
            {
                std::string className = JNIUtils::getNameOfClassOfObject(JNIUtils::getEnv(), javaObject);

                // if(className != JAVA_CLASS_NAME)
                // {
                //     JBIND_THROW("Cannot create native Consumer object from passed java object of type \"" << className << "\"."
                //     << "Please make sure the jobject passed to JavaArrayList is of type \"" << JAVA_CLASS_NAME << "\"");
                // }

                this->referenceHolder = std::make_shared<ReferenceHolder>(javaObject);
            }

            void accept(jobject obj)
            {
                jobject javaObject = this->referenceHolder->getJavaObject();
                std::string className = JNIUtils::getNameOfClassOfObject(JNIUtils::getEnv(), javaObject);

                JNIEnv* env = JNIUtils::getEnv();
                jmethodID acceptMethodID = JNIUtils::getMethodID(env, javaObject, "accept", "(Ljava/lang/Object;)V");

                if(acceptMethodID == nullptr)
                {
                    JBIND_THROW("Cannot call accept function of Consumer. Java object of class \"" << className << "\"\n"
                    << "Does not have a function \"void accept(T t)\".")
                }
                std::cout << "Calling consumer function\n";
                env->CallVoidMethod(javaObject, acceptMethodID, obj);
            }

            jobject getJavaObject()
            {
                return this->referenceHolder->getJavaObject();
            }

    };

}