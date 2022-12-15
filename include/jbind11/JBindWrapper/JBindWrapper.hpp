#pragma once
#include <jni.h>

#include "JavaHandle/JavaHandle.hpp"
#include "JavaPackage/JavaPackageManager.hpp"
using namespace jbind11;

static AbstractJavaClass* findFirstParentClassThatIsAJBindClass(JNIEnv* env, jobject wrappedObject)
{
    // Inheritance graph (names of super classes).
    // Note, that java does not support inheriting from multiple classes.
    // This makes it easier.
    std::vector<std::string> inheritanceList = JNIUtils::getInheritanceListOfObject(env, wrappedObject);

    AbstractJavaClass* javaClass;

    for(const std::string& name : inheritanceList)
    {
        if(name == "jbind11.JBindWrapper" || name == "java.lang.Object")
        {
            return nullptr;
        }

        javaClass = getPackageManager().findClassByCanonicalName(name);

        if(javaClass != nullptr)
        {
            return javaClass;
        }
    }

    return nullptr;
}

extern "C" 
{
    JNIEXPORT void JNICALL Java_jbind11_JBindWrapper_nativeInit(JNIEnv* env, jobject wrappedObject)
    {
        std::string canonicalClassName = JNIUtils::getNameOfClassOfObject(env, wrappedObject);

        AbstractJavaClass* javaClass = getPackageManager().findClassByCanonicalName(canonicalClassName);
        
        if(javaClass == nullptr)
        {
            // Class not registered.
            // Check if any parent class is derived from a jbind11 class.
            // Luckily java does not support multiple inheritance. Otherwise, this would get more complicated.
            javaClass = findFirstParentClassThatIsAJBindClass(env, wrappedObject);

            if(javaClass == nullptr)
            {
                // Otherwise, the user must have manually created a class that inherits from jbind11.JBindWrapper,
                // which is not supported.
                // TODO: Get name of super class and check.

                JBIND_THROW("Error, failed to initialize java object of type \"" << canonicalClassName << "\".\n"
                << "The class inherits from JBindWrapper, however is not known to jbind11.\n"
                << "In order to create new classes that are supported by jbind11, it is not sufficient to simply create\n"
                << "your own class and inherit from JBindWrapper.\n"
                << "You need to add the class in C++ to a JBIND_MODULE and let jbind11 generate the wrapper code, not the other way round.\n"
                << "Please follow the official jbind11 guidelines.");
            }
        }

        JavaHandle* handle = javaClass->spawnNewHandle();
        handle->assignToObject(env, wrappedObject);
        std::cout << "Init object " << wrappedObject << "\n" << std::flush;
    }

    JNIEXPORT void JNICALL Java_jbind11_JBindWrapper_nativeSet(JNIEnv* env, jobject wrappedObject, jstring jfieldName, jobject value)
    {
        std::string fieldName = JNIUtils::toStdString(env, jfieldName);
        // Please note, that error handling is done appropriately by each of the individual functions
        // of JavaHandle, AbstractJavaClass and AbstractJavaField.
        JavaHandle handle = JavaHandle::getHandleFromObject(env, wrappedObject);
        AbstractJavaClass* javaClass = handle.getJavaClass();
        AbstractJavaField* field = javaClass->getField(fieldName);

        field->setValue(env, handle, value);
    }

    JNIEXPORT jobject JNICALL Java_jbind11_JBindWrapper_nativeGet(JNIEnv* env, jobject wrappedObject, jstring jfieldName)
    {
        std::string fieldName = JNIUtils::toStdString(env, jfieldName);
        // Please note, that error handling is done appropriately by each of the individual functions
        // of JavaHandle, AbstractJavaClass and AbstractJavaField.
        JavaHandle handle = JavaHandle::getHandleFromObject(env, wrappedObject);
        AbstractJavaClass* javaClass = handle.getJavaClass();
        AbstractJavaField* field = javaClass->getField(fieldName);

        return field->getValue(env, handle);
    }

    JNIEXPORT jobject JNICALL Java_jbind11_JBindWrapper_nativeInvoke(JNIEnv* env, jobject wrappedObject, jstring jfunctionName, jobject jstack)
    {
        std::string functionName = JNIUtils::toStdString(env, jfunctionName);
        // Please note, that error handling is done appropriately by each of the individual functions
        // of JavaHandle, AbstractJavaClass and AbstractJavaField.
        JavaHandle handle = JavaHandle::getHandleFromObject(env, wrappedObject);
        AbstractJavaClass* javaClass = handle.getJavaClass();
        AbstractJavaFunction* function = javaClass->getFunction(functionName);

        JavaArrayList stack(jstack);
        return function->execute(handle, stack);
    }
}