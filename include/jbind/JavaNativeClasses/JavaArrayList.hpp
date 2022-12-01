#pragma once

#include <jni.h>
#include "JNIUtils/JNIUtils.hpp"
namespace jbind
{
    // Good reference: https://stackoverflow.com/questions/33406527/returning-an-arraylist-of-string-from-native-java-to-jni
    namespace JavaNativeClasses
    {
        class JavaArrayList
        {
            private:
                const std::string JAVA_CLASS_NAME = "java.util.ArrayList";

                jobject javaObject;

                void createNewArrayList(JNIEnv* env, size_t initialCapacity)
                {
                    // Call constructor ArrayList(Integer initialCapacity).
                    // "I" is the signature of "Integer", we specify it to 
                    // use the correct constructor.
                    this->javaObject = 
                        JNIUtils::createObjectFromClassName(env, JAVA_CLASS_NAME, "I", initialCapacity);
                }

                void createNewArrayList(JNIEnv* env)
                {
                    // "" means we use the standard constructor with no parameters. 
                    this->javaObject = 
                        JNIUtils::createObjectFromClassName(env, JAVA_CLASS_NAME, "");
                }

            public:
                JavaArrayList(JNIEnv* env)
                {
                    this->createNewArrayList(env);
                }

                JavaArrayList(JNIEnv* env, size_t initialCapacity)
                {
                    this->createNewArrayList(env, initialCapacity);
                }

                JavaArrayList(JNIEnv* env, jobject javaObject) 
                {
                    std::string className = JNIUtils::getNameOfClassOfObject(env, javaObject);

                    if(className != JAVA_CLASS_NAME)
                    {
                        JBIND_THROW("Cannot create native JavaArrayList object from passed java object of type \"" << className << "\"."
                        << "Please make sure the jobject passed to JavaArrayList is of type \"" << JAVA_CLASS_NAME << "\"");
                    }

                    this->javaObject = javaObject;
                }

                void add(JNIEnv* env, jobject objectToAdd)
                {
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

                size_t size(JNIEnv* env)
                {
                    jmethodID arrayListSizeMethodID = JNIUtils::getMethodID(env, this->javaObject, "size", "()I");

                    if(arrayListSizeMethodID == nullptr)
                    {
                        JBIND_THROW("Error, JavaArrayList.size() failed. Cannot find \"size\" function in Java object.\n"
                        "Is the internal java object really of type java.util.ArrayList?");
                    }

                    return env->CallIntMethod(this->javaObject, arrayListSizeMethodID);
                }

                jobject get(JNIEnv* env, size_t index)
                {
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
}