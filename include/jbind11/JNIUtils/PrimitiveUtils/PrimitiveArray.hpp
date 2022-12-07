#pragma once
#include <jni.h>
#include <string>

namespace jbind11
{
    namespace JNIUtils
    {
        template<typename T>
        static inline typename std::enable_if<std::is_same<T, int8_t>::value || std::is_same<T, uint8_t>::value>::type
        getPrimitiveArrayRegion(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            env->GetByteArrayRegion(static_cast<jbyteArray>(object), start, length, destination);
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, int16_t>::value || std::is_same<T, uint16_t>::value>::type
        getPrimitiveArrayRegion(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            env->GetShortArrayRegion(static_cast<jshortArray>(object), start, length, destination);
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, int32_t>::value || std::is_same<T, uint32_t>::value>::type
        getPrimitiveArrayRegion(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            env->GetIntArrayRegion(static_cast<jintArray>(object), start, length, destination);
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, int64_t>::value || std::is_same<T, uint64_t>::value>::type
        getPrimitiveArrayRegion(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            env->GetLongArrayRegion(static_cast<jlongArray>(object), start, length, destination);
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, float>::value>::type
        getPrimitiveArrayRegion(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            env->GetFloatArrayRegion(static_cast<jfloatArray>(object), start, length, destination);
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, double>::value>::type
        getPrimitiveArrayRegion(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            env->GetDoubleArrayRegion(static_cast<jdoubleArray>(object), start, length, destination);
        }

        // Note that Java's Character class is 16 bit. 
        // Further note: char16_t can neither be signed nor unsigned.
        template<typename T>
        static inline typename std::enable_if<std::is_same<T, char16_t>::value>::type
        getPrimitiveArrayRegion(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            env->GetCharArrayRegion(static_cast<jcharArray>(object), start, length, destination);
        }
        template<typename T>
        static inline typename std::enable_if<std::is_same<T, bool>::value>::type
        getPrimitiveArrayRegion(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            env->GetBooleanArrayRegion(static_cast<jbooleanArray>(object), start, length, destination);
        }
    }
}