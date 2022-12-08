#pragma once
#include <jni.h>
#include <string>

namespace jbind11
{
    namespace JNIUtils
    {
        template<typename T>
        static inline typename std::enable_if<is_integer_no_bool<T>::value && sizeof(T) == 1, std::string>::type // 8 bit, i.e. int8_t, uint8_t (char_8t)
        getPrimitiveArrayTypeDeclaration(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            return "byte[]";
        }

        template<typename T>
        static inline typename std::enable_if<is_integer_no_bool<T>::value && sizeof(T) == 2, std::string>::type // 16 bit, i.e. int16_t, uint16_t
        getPrimitiveArrayTypeDeclaration(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            return "short[]";
        }

        template<typename T>
        static inline typename std::enable_if<is_integer_no_bool<T>::value && sizeof(T) == 4, std::string>::type // 32 bit, i.e. int32_t, uint32_t
        getPrimitiveArrayTypeDeclaration(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            return "int[]";
        }

        template<typename T>
        static inline typename std::enable_if<is_integer_no_bool<T>::value && sizeof(T) == 8, std::string>::type // 64 bit, i.e. int64_t, uint64_t
        getPrimitiveArrayTypeDeclaration(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            return "long[]";
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, float>::value, std::string>::type
        getPrimitiveArrayTypeDeclaration(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            return "float[]";
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, double>::value, std::string>::type
        getPrimitiveArrayTypeDeclaration(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            return "double[]";
        }

        // Note that Java's Character class is 16 bit. 
        // Further note: char16_t can neither be signed nor unsigned.
        template<typename T>
        static inline typename std::enable_if<std::is_same<T, char16_t>::value, std::string>::type
        getPrimitiveArrayTypeDeclaration(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            return "char[]";
        }
        template<typename T>
        static inline typename std::enable_if<std::is_same<T, bool>::value, std::string>::type
        getPrimitiveArrayTypeDeclaration(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            return "boolean[]";
        }
    }
}