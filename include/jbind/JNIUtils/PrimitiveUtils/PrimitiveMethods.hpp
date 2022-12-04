#pragma once
#include <jni.h>
#include <string>

namespace jbind
{
    namespace JNIUtils
    {
        template<typename T>
        static inline typename std::enable_if<std::is_same<T, int8_t>::value || std::is_same<T, uint8_t>::value, T>::type
        callPrimitiveMethod(JNIEnv* env, jobject& object, jmethodID methodID)
        {
            return env->CallByteMethod(object, methodID);;
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, int16_t>::value || std::is_same<T, uint16_t>::value, T>::type
        callPrimitiveMethod(JNIEnv* env, jobject& object, jmethodID methodID)
        {
            return env->CallShortMethod(object, methodID);
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, int32_t>::value || std::is_same<T, uint32_t>::value, T>::type
        callPrimitiveMethod(JNIEnv* env, jobject& object, jmethodID methodID)
        {
            return env->CallIntMethod(object, methodID);
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, int64_t>::value || std::is_same<T, uint64_t>::value, T>::type
        callPrimitiveMethod(JNIEnv* env, jobject& object, jmethodID methodID)
        {
            return env->CallLongMethod(object, methodID);
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, float>::value, T>::type
        callPrimitiveMethod(JNIEnv* env, jobject& object, jmethodID methodID)
        {
            return env->CallFloatMethod(object, methodID);
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, double>::value, T>::type
        callPrimitiveMethod(JNIEnv* env, jobject& object, jmethodID methodID)
        {
            return env->CallDoubleMethod(object, methodID);
        }

        // Note that Java's Character class is 16 bit. 
        // Further note: char16_t can neither be signed nor unsigned.
        template<typename T>
        static inline typename std::enable_if<std::is_same<T, char16_t>::value, T>::type
        callPrimitiveMethod(JNIEnv* env, jobject& object, jmethodID methodID)
        {
            return env->CallCharMethod(object, methodID);
        }
        template<typename T>
        static inline typename std::enable_if<std::is_same<T, bool>::value, T>::type
        callPrimitiveMethod(JNIEnv* env, jobject& object, jmethodID methodID)
        {
            return env->CallBooleanMethod(object, methodID);
        }
    }
}