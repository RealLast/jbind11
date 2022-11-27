#pragma once
#include <type_traits>
#include <cstdint>
#include <string>
namespace jbind
{
    namespace JNIUtils
    {
        template<typename T>
        static inline typename std::enable_if<std::is_same<T, int8_t>::value || std::is_same<T, uint8_t>::value, std::string>::type
        getSignatureOfPrimitiveType()
        {
            return "B";
        }

        // int16_t should be typedef'd as short.
        template<typename T>
        static inline typename std::enable_if<std::is_same<T, int16_t>::value || std::is_same<T, uint16_t>::value, std::string>::type
        getSignatureOfPrimitiveType()
        {
            return "S";
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, int32_t>::value || std::is_same<T, uint32_t>::value, std::string>::type
        getSignatureOfPrimitiveType()
        {
            return "I";
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, int64_t>::value || std::is_same<T, uint64_t>::value, std::string>::type
        getSignatureOfPrimitiveType()
        {
            // Yes, it's "J", because "L" is used for clases.
            return "J";
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, float>::value, std::string>::type
        getSignatureOfPrimitiveType()
        {
            return "F";
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, double>::value, std::string>::type
        getSignatureOfPrimitiveType()
        {
            return "D";
        }

        // Note that Java's Character class is 16 bit. 
        // Further note: char16_t can neither be signed nor unsigned.
        template<typename T>
        static inline typename std::enable_if<std::is_same<T, char16_t>::value, std::string>::type
        getSignatureOfPrimitiveType()
        {
            return "C";
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, bool>::value, std::string>::type
        getSignatureOfPrimitiveType()
        {
            return "Z";
        }
    }
}