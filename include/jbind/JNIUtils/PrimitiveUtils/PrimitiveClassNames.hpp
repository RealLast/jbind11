#pragma once
#include <type_traits>
#include <cstdint>
#include <string>

namespace jbind
{
    namespace JNIUtils
    {
        template<typename T>
        static inline typename std::enable_if<std::is_same<T, signed char>::value || std::is_same<T, unsigned char>::value, std::string>::type
        getJavaClassNameOfPrimitiveType()
        {
            return "java.lang.Byte";
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, int16_t>::value || std::is_same<T, uint16_t>::value, std::string>::type
        getJavaClassNameOfPrimitiveType()
        {
            return "java.lang.Short";
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, int32_t>::value || std::is_same<T, uint32_t>::value, std::string>::type
        getJavaClassNameOfPrimitiveType()
        {
            return "java.lang.Integer";
        }

        template<typename T>
        static inline typename std::enable_if<(std::is_same<T, long>::value || std::is_same<T, unsigned long>::value) && !std::is_same<long, int64_t>::value, std::string>::type
        getJavaClassNameOfPrimitiveType()
        {
            // Yes, it's "J", because "L" is used for clases.
            return "java.lang.Long";
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, int64_t>::value || std::is_same<T, uint64_t>::value, std::string>::type
        getJavaClassNameOfPrimitiveType()
        {
            // Yes, it's "J", because "L" is used for clases.
            return "java.lang.Long";
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, float>::value, std::string>::type
        getJavaClassNameOfPrimitiveType()
        {
            return "java.lang.Float";
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, double>::value, std::string>::type
        getJavaClassNameOfPrimitiveType()
        {
            return "java.lang.Double";
        }

        // Note that Java's Character class is 16 bit. 
        // Further note: char16_t can neither be signed nor unsigned.
        template<typename T>
        static inline typename std::enable_if<std::is_same<T, char16_t>::value, std::string>::type
        getJavaClassNameOfPrimitiveType()
        {
            return "java.lang.Character";
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, bool>::value, std::string>::type
        getJavaClassNameOfPrimitiveType()
        {
            return "java.lang.Boolean";
        }
    }
}