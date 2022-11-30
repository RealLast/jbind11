#pragma once

#include <jni.h>
#include "JNIUtils/JNIUtils.hpp"
namespace jbind
{
    namespace JavaNativeClasses
    {
        class JavaArrayList
        {
            private:
                const static std::string JAVA_CLASS_NAME = "java.util.ArrayList";

                jobject javaObject;

                void createNewArrayList(JNIEnv* env, int initialCapacity)
                {
                    // Call constructor ArrayList(Integer initialCapacity).
                    // "I" is the signature of "Integer", we specify it to 
                    // use the correct constructor.
                    this->javaObject = 
                        createObjectFromClassName(env, JAVA_CLASS_NAME, "I", initialCapacity);
                }

                void createNewArrayList(JNIEnv* env)
                {
                    // "" means we use the standard constructor with no parameters. 
                    this->javaObject = 
                        createObjectFromClassName(env, JAVA_CLASS_NAME, "");
                }

            public:
                JavaArrayList()
                {
                    this->createNewArrayList();
                }

                JavaArrayList(int initialCapacity)
                {
                    this->createNewArrayList(initialCapacity);
                }

                JavaArrayList(jobject javaObject) : javaObject(javaObject)
                {

                }


        };
    }
}