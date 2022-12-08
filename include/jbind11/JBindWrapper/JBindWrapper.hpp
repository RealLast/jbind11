#pragma once
#include <jni.h>

#include "JavaHandle/JavaHandle.hpp"
#include "JavaPackage/JavaPackageManager.hpp"
using namespace jbind11;

extern "C" 
{
    JNIEXPORT void JNICALL JBind_JBindWrapper_init(JNIEnv* env, jobject wrappedObject)
    {
        std::string canonicalClassName = JNIUtils::getNameOfClassOfObject(env, wrappedObject);

        AbstractJavaClass* javaClass = JavaPackageManager::findClassByCanonicalName(canonicalClassName);

        if(javaClass == nullptr)
        {
            JBIND_THROW("Error, failed to initialize java object of type \"" << canonicalClassName << "\".\n"
            << "The class inherits from JBindWrapper, however is not known to jbind11.\n"
            << "In order to create new classes that are supported by jbind11, it is not sufficient to simply create\n"
            << "your own class and inherit from JBindWrapper.\n"
            << "You need to add the class in C++ to a JBIND_MODULE and let jbind11 generate the wrapper code, not the other way round.\n"
            << "Please follow the official jbind11 guidelines.");
        }

        JavaHandle* handle = javaClass->spawnNewObject();
        handle->assignHandleToObject(env, wrappedObject);
    }

    JNIEXPORT void JNICALL JBind_JBindWrapper_set(JNIEnv* env, jobject wrappedObject, jstring jfieldName, jobject value)
    {
        std::string fieldName = JNIUtils::toStdString(env, jfieldName);
        // Please note, that error handling is done appropriately by each of the individual functions
        // of JavaHandle, AbstractJavaClass and AbstractJavaField.
        JavaHandle* handle = JavaHandle::getHandleFromObject(env, wrappedObject);
        AbstractJavaClass* javaClass = handle->getJavaClass();
        AbstractJavaField* field = javaClass->getField(fieldName);

        field->setValue(env, *handle, value);
    }

    JNIEXPORT jobject JNICALL JBind_JBindWrapper_get(JNIEnv* env, jobject wrappedObject, jstring jfieldName)
    {
        std::string fieldName = JNIUtils::toStdString(env, jfieldName);
        // Please note, that error handling is done appropriately by each of the individual functions
        // of JavaHandle, AbstractJavaClass and AbstractJavaField.
        JavaHandle* handle = JavaHandle::getHandleFromObject(env, wrappedObject);
        AbstractJavaClass* javaClass = handle->getJavaClass();
        AbstractJavaField* field = javaClass->getField(fieldName);

        return field->getValue(env, *handle);
    }
}