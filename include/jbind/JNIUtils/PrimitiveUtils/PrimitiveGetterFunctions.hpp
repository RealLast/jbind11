#pragma once
#include <type_traits>
#include <cstdint>
#include <string>

namespace jbind
{
    namespace JNIUtils
    {   
        // Utils to retrieve the name of the function that can be used to retrieve a primitive
        // object from the corresponding java class.
        // e.g.:
        /*
            == JAVA ==
            Integer i = new Integer(42);
            int value = i.intValue();
        */

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, int8_t>::value || std::is_same<T, uint8_t>::value, std::string>::type
        getGetterFunctionNameUsedToRetrievePrimitiveFromJavaObject()
        {
            return "byteValue";
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, int16_t>::value || std::is_same<T, uint16_t>::value, std::string>::type
        getGetterFunctionNameUsedToRetrievePrimitiveFromJavaObject()
        {
            return "shortValue";
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, int32_t>::value || std::is_same<T, uint32_t>::value, std::string>::type
        getGetterFunctionNameUsedToRetrievePrimitiveFromJavaObject()
        {
            return "intValue";
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, int64_t>::value || std::is_same<T, uint64_t>::value, std::string>::type
        getGetterFunctionNameUsedToRetrievePrimitiveFromJavaObject()
        {
            return "longValue";
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, float>::value, std::string>::type
        getGetterFunctionNameUsedToRetrievePrimitiveFromJavaObject()
        {
            return "floatValue";
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, double>::value, std::string>::type
        getGetterFunctionNameUsedToRetrievePrimitiveFromJavaObject()
        {
            return "doubleValue";
        }

        // Note that Java's Character class is 16 bit. 
        // Further note: char16_t can neither be signed nor unsigned.
        template<typename T>
        static inline typename std::enable_if<std::is_same<T, char16_t>::value, std::string>::type
        getGetterFunctionNameUsedToRetrievePrimitiveFromJavaObject()
        {
            return "charValue";
        }

        template<typename T>
        static inline typename std::enable_if<std::is_same<T, bool>::value, std::string>::type
        getGetterFunctionNameUsedToRetrievePrimitiveFromJavaObject()
        {
            return "booleanValue";
        }
    }
}