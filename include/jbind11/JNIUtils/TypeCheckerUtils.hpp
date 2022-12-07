#pragma once
#include "BasicUtils.hpp"
#include <jni.h>

namespace jbind11
{
    namespace JNIUtils
    {
        static inline bool isJavaByteObject(JNIEnv* env, jobject data)
        {
            std::string className = getNameOfClassOfObject(env, data);
            return className == "java.lang.Byte";
        }

        static inline bool isJavaShortObject(JNIEnv* env, jobject data)
        {
            std::string className = getNameOfClassOfObject(env, data);
            return className == "java.lang.Short";
        }

        static inline bool isJavaIntegerObject(JNIEnv* env, jobject data)
        {
            std::string className = getNameOfClassOfObject(env, data);
            return className == "java.lang.Integer";
        }

        static inline bool isJavaLongObject(JNIEnv* env, jobject data)
        {
            std::string className = getNameOfClassOfObject(env, data);
            return className == "java.lang.Long";
        }

        static inline bool isJavaFloatObject(JNIEnv* env, jobject data)
        {
            std::string className = getNameOfClassOfObject(env, data);
            return className == "java.lang.Float";
        }

        static inline bool isJavaDoubleObject(JNIEnv* env, jobject data)
        {
            std::string className = getNameOfClassOfObject(env, data);
            return className == "java.lang.Double";
        }

        static inline bool isJavaBooleanObject(JNIEnv* env, jobject data)
        {
            std::string className = getNameOfClassOfObject(env, data);
            return className == "java.lang.Boolean";
        }

        static inline bool isJavaCharObject(JNIEnv* env, jobject data)
        {
            std::string className = getNameOfClassOfObject(env, data);
            return className == "java.lang.Character";
        }

        static inline bool isJavaStringObject(JNIEnv* env, jobject data)
        {
            std::string className = getNameOfClassOfObject(env, data);
            return className == "java.lang.String";
        }

        static inline bool isJavaStringBuilderObject(JNIEnv* env, jobject data)
        {
            std::string className = getNameOfClassOfObject(env, data);
            return className == "java.lang.StringBuilder";
        }
    }
}