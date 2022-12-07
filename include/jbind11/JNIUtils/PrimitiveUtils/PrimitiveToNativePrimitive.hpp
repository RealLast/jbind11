#pragma once

#include "PrimitiveGetterFunctions.hpp"
#include "PrimitiveSignatures.hpp"
#include "PrimitiveMethods.hpp"

namespace jbind11
{
    namespace JNIUtils
    {
        template<typename T>
        T javaPrimitiveToNativePrimitive(JNIEnv* env, jobject javaObject)
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
            std::string signature = "()" + JNIUtils::getSignatureOfPrimitiveType<T>();
            std::string getterMethodName = JNIUtils::getGetterFunctionNameUsedToRetrievePrimitiveFromJavaObject<T>().c_str();

            jclass objectClass = JNIUtils::getClassOfObject(env, javaObject);
            jmethodID mGetValue = env->GetMethodID(objectClass, getterMethodName.c_str(), signature.c_str());
            env->DeleteLocalRef(objectClass);
            if(mGetValue == NULL)
            {
                JBIND_THROW("Error, cannot convert java object of type \"" << className << "\" to native C++ primitive \"" << TypeName<T>::get() << "\"."
                << "Method \"" << getterMethodName << "\" was not found in object of type \"" << className << "\"");
            } 

            return JNIUtils::callPrimitiveMethod<T>(env, javaObject, mGetValue);
        }
    }    
}