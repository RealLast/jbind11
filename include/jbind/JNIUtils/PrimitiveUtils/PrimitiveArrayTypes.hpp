#pragma once
#include <jni.h>
#include <string>

namespace jbind
{
    namespace JNIUtils
    {
        template<typename T>
        static inline typename std::enable_if<std::is_same<T, int8_t>::value || std::is_same<T, uint8_t>::value, std::string>::type
        getPrimitiveArrayTypeDeclaration(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            return "byte[]";
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, int16_t>::value || std::is_same<T, uint16_t>::value, std::string>::type
        getPrimitiveArrayTypeDeclaration(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            return "short[]";
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, int32_t>::value || std::is_same<T, uint32_t>::value, std::string>::type
        getPrimitiveArrayTypeDeclaration(JNIEnv* env, jarray object, size_t start, size_t length, T* destination)
        {
            return "int[]";
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, int64_t>::value || std::is_same<T, uint64_t>::value, std::string>::type
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