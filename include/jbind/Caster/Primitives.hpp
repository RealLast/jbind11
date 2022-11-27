#pragma once

#include <jni.h>
#include "Caster.hpp"


/*
References: https://msoe.us/taylor/tutorial/ce2810/csimilar
https://www.geeksforgeeks.org/data-types-in-java/

Be aware, that in C++, we can only really rely on types like uint8_t, int8_t to have a certain number of bits.
Normal integer types like short, int or long can be different, depening on System and compiler (e.g., Linux vs. Unix)
See here: https://en.cppreference.com/w/cpp/language/types
Since uint8_t, uint16_t.. and others are just typdefs of char, int, long etc. depending on the architecture,
we use them (uint8_t, uint16_t, ..) in the template specializations to automatically choose the correct underlying type (char, short, int)..
Note, that Java's Character data type does NOT corresponds to C++'s char type, since it has 16 bits in Java.
The char type in C++ has 8 bits, thus corresponds to Java's byte type.
Java's character type, however, can be matched to C++'s char_16t.



Java Primitive Data Types:

Type        Content             Memory                      Min Value                                   Max Value       Matching C++ type
byte 	    integer 	        8 bits 	                         -128 	                                      127       signed/unsigned char
short 	    integer 	        16 bits 	                  -32,768 	                                   22,767       int16_t/uint16_t
int 	    integer 	        32 bits 	           -2,147,483,648 	                            2,147,483,647       int32_t/uint32_t
long 	    integer 	        64 bits    -9,223,372,036,854,775,808 	                9,223,372,036,854,775,808       int64_t/uint64_t
float 	    real 	            32 bits 	                  -3.4E38 	                                   3.4E38       float
double 	    real 	            64 bits 	                 -1.8E308 	                                  1.8E308       double

Character   unicode characters  16 bits(does not match C++'s char type!)                                                char16_t
Boolean     true, false                                                                                                 bool

*/

#include "JNIUtils/JNIUtils.hpp"

namespace jbind
{
    template<class T>
    struct Caster<T, typename std::enable_if<std::is_arithmetic<T>::value>::type> 
    {
        public:
            static T cast(JNIEnv* env, jobject& javaObject)
            {
                std::string className = JNIUtils::getNameOfClassOfObject(env, javaObject);
                std::string expectedJavaPrimitiveClassName = JNIUtils::getJavaClassNameOfPrimitiveType<T>();

                // Does java object match the primitive?
                if(className != expectedJavaPrimitiveClassName)
                {
                    JBIND_THROW("Error, cannot convert javaObject to native C++ primitive " << TypeName<T>::get() << "."
                    << "Java object is of type \"" << className << "\", however an object of type \"" << expectedJavaPrimitiveClassName << "\" was expected.");
                }

                // Get the function of the java object that can be used to retrieve the primitive (e.g., integerValue, floatValue, ... as in
                // Integer(42).integerValue() etc.)
                std::string signature = "()" + Signatures::Primitive::getSignatureOfPrimitiveType<T>();
                std::string getterMethodName = getGetterFunctionNameUsedToRetrievePrimitiveFromJavaObject<T>().c_str();

                jclass objectClass = getClassOfObject(env, javaObject);
                jmethodID mGetValue = env->GetMethodID(objectClass, getterMethodName.c_str(), signature.c_str());
                env->DeleteLocalRef(objectClass);
                if(mGetValue == NULL)
                {
                    CLAID_THROW(Exception, "Error, cannot convert java object of type \"" << className << "\" to native C++ primitive \"" << TypeChecking::getCompilerSpecificCompileTypeNameOfClass<T>() << "\"."
                    << "Method \"" << getterMethodName << "\" was not found in object of type \"" << className << "\"");
                } 

                nativePrimitive = callPrimitiveMethod<T>(env, javaObject, mGetValue);
            }
    };
}