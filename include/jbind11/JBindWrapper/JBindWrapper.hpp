#pragma once
#include <jni.h>

#include "JavaHandle/JavaHandle.hpp"
#include "JavaPackage/JavaPackageManager.hpp"
using namespace jbind11;

static AbstractJavaClass* findFirstParentClassThatIsAJBindClass(JNIEnv* env, jclass cls)
{
    // Inheritance graph (names of super classes).
    // Note, that java does not support inheriting from multiple classes.
    // This makes it easier.
    std::vector<std::string> inheritanceList = JNIUtils::getInheritanceList(env, cls);

    AbstractJavaClass* javaClass;

    for(const std::string& name : inheritanceList)
    {
        if(name == "jbind11.JBindWrapper" || name == "java.lang.Object")
        {
            return nullptr;
        }

        javaClass = JavaPackageManager::getInstance()->findClassByCanonicalName(name);

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

        AbstractJavaClass* javaClass = JavaPackageManager::getInstance()->findClassByCanonicalName(canonicalClassName);
        
        if(javaClass == nullptr)
        {
            // Class not registered.
            // Check if any parent class is derived from a jbind11 class.
            // Luckily java does not support multiple inheritance. Otherwise, this would get more complicated.
            JNIEnv* env = JNIUtils::getEnv();
            jclass cls = JNIUtils::getClassOfObject(env, wrappedObject);
            javaClass = findFirstParentClassThatIsAJBindClass(env, cls);
            env->DeleteLocalRef(cls);

            if(javaClass == nullptr)
            {
                // Otherwise, the user must have manually created a class that inherits from jbind11.JBindWrapper,
                // which is not supported.
                // TODO: Get name of super class and check.

                // Why is it sure it inherits from JBindWrapper you ask?
                // Because this function (nativeInit) is called in the constructor of jbind11.JBindWrapper ;)
                JBIND_THROW("Error, failed to initialize java object of type \"" << canonicalClassName << "\".\n"
                << "The class inherits from JBindWrapper, however is not known to jbind11.\n"
                << "In order to create new classes that are supported by jbind11, it is not sufficient to simply create\n"
                << "your own class and inherit from JBindWrapper.\n"
                << "You need to add the class in C++ to a JBIND_MODULE and let jbind11 generate the wrapper code, not the other way round.\n"
                << "Please follow the official jbind11 guidelines.");
            }
        }

        JavaHandle* handle = javaClass->spawnNewHandle();

        // Like global references, weak global references remain valid across native method calls and across different threads.
        // We make sure that other native functions can access the wrapped object as long as it is not garbage collected by using weak global references.
        // If we wouldn't do that, than the wrappedObject ptr stored in the handle will be invalid as soon as we leave the nativeInit function.
        // Any try to access the java object by other native functions via the handle, would therefore fail.
        handle->assignToObject(env, env->NewWeakGlobalRef(wrappedObject));
    }

    JNIEXPORT void JNICALL Java_jbind11_JBindWrapper_nativeFinalize(JNIEnv* env, jobject wrappedObject)
    {
        JavaHandle::removeAndDeleteHandleFromObject(env, wrappedObject);
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

    JNIEXPORT jobject JNICALL Java_jbind11_JBindWrapper_nativeInvokeStatic(JNIEnv* env, jclass jbind11Wrapper, jclass staticClass, jstring jfunctionName, jobject jstack)
    {
        std::string functionName = JNIUtils::toStdString(env, jfunctionName);
        // Please note, that error handling is done appropriately by each of the individual functions
        // of JavaHandle, AbstractJavaClass and AbstractJavaField.
        std::string canonicalClassName = JNIUtils::getClassName(JNIUtils::getEnv(), staticClass);
        
        AbstractJavaClass* javaClass = JavaPackageManager::getInstance()->findClassByCanonicalName(canonicalClassName);
        
        if(javaClass == nullptr)
        {
            // Class not registered.
            // Check if any parent class is derived from a jbind11 class.
            // Luckily java does not support multiple inheritance. Otherwise, this would get more complicated.
            
            javaClass = findFirstParentClassThatIsAJBindClass(JNIUtils::getEnv(), staticClass);

            if(javaClass == nullptr)
            {
                JBIND_THROW("Error, cannot invoke function \"" << functionName << "\" of class \"" << canonicalClassName << "\".\n"
                << "The specified class is neither a class generated by jbind11, nor does it inherit from one.\n"
                << "In order to create new classes that are supported by jbind11, it is not sufficient to simply create\n"
                << "your own class and inherit from JBindWrapper.\n"
                << "You need to add the class in C++ to a JBIND_MODULE and let jbind11 generate the wrapper code, not the other way round.\n"
                << "Please follow the official jbind11 guidelines.");
            }
        }

        // Throws an exception, if the function was not found.
        AbstractJavaFunction* function = javaClass->getFunction(functionName);
        JavaArrayList stack(jstack);

        // Function should be a static function, hence it does not require a handle.
        // We just pass an invalid handle here.
        // We do this so that we do not need to define multiple functions
        // in AbstractJavaFunction, JavaFunction, NonStaticJavaFunction and StaticJavaFunction
        // (i.e., staticExecute vs execute).
        return function->execute(JavaHandle::InvalidHandle(), stack);
    }
}