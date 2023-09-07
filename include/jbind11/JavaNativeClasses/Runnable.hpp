#pragma once

#include <jni.h>
#include "JNIUtils/JNIUtils.hpp"
namespace jbind11
{
    class Runnable
    {
        private:
            std::string JAVA_CLASS_NAME = "java.lang.Runnable";


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
            Runnable()
            {
            }

         
            Runnable(jobject javaObject) 
            {
                std::string className = JNIUtils::getNameOfClassOfObject(JNIUtils::getEnv(), javaObject);

                // if(className != JAVA_CLASS_NAME)
                // {
                //     JBIND_THROW("Cannot create native Runnable object from passed java object of type \"" << className << "\"."
                //     << "Please make sure the jobject passed to JavaArrayList is of type \"" << JAVA_CLASS_NAME << "\"");
                // }

                this->referenceHolder = std::make_shared<ReferenceHolder>(javaObject);
            }

            void run()
            {
                jobject javaObject = this->referenceHolder->getJavaObject();
                std::string className = JNIUtils::getNameOfClassOfObject(JNIUtils::getEnv(), javaObject);

                JNIEnv* env = JNIUtils::getEnv();
                jmethodID runMethodID = JNIUtils::getMethodID(env, javaObject, "run", "()V");

                if(runMethodID == nullptr)
                {
                    JBIND_THROW("Cannot call run function of Runnable. Java object of class \"" << className << "\"\n"
                    << "Does not have a function \"void run()\".")
                }
                std::cout << "Calling Runnable function\n";
                env->CallVoidMethod(javaObject, runMethodID);
            }

            jobject getJavaObject()
            {
                return this->referenceHolder->getJavaObject();
            }

    };

}