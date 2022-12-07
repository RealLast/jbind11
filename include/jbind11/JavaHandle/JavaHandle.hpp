#pragma once

#include "JavaClass/AbstractJavaClass.hpp"
#include "JavaPackage/JavaPackageManager.hpp"
#include "JNIUtils/JNIUtils.hpp"
namespace jbind11
{
    template <typename T>
    class JavaClass;
}

#include "JavaClass/JavaClass.hpp"

#include "jbind11_throw.hpp"
namespace jbind11
{
    // Wrapper around a plain C++ object.
    // Stores the raw pointer to the object as well as it's associated java class.
    class JavaHandle 
    {
        private:
            void* rawPtr;
            AbstractJavaClass* javaClass;

            bool initialized = false;

            static jfieldID getHandleField(JNIEnv* env, jobject obj)
            {
                jclass c = env->GetObjectClass(obj);
                // J is the type signature for long:
                jfieldID fieldID = env->GetFieldID(c, "nativeJavaHandle", "J");
                env->DeleteLocalRef(c);

                return fieldID;
            }

        public:
            
            template<typename T>
            void set(T* value)
            {
                this->rawPtr = static_cast<void*>(&value);
                this->javaClass = JavaPackageManager::findClass<JavaClass<T>>();

                if(this->javaClass == nullptr)
                {
                    JBIND_THROW("Failed to create JavaHandle for Native C++ class \"" << TypeName<T>::get() << "\". No JavaClass wrapper was registered for this native type.");
                }

                this->initialized = true;
            }

            template<typename T>
            T* get()
            {
                if(!this->initialized)
                {
                    JBIND_THROW("Failed to get object reference from JavaHandle. Handle has not been initialized using set() before!");
                }
                return static_cast<T*>(this->rawPtr);    
            }

            AbstractJavaClass* getJavaClass()
            {
                if(!this->initialized)
                {
                    JBIND_THROW("Failed to get JavaClass from JavaHandle. Handle has not been initialized using set() before!");
                }
                return this->javaClass;
            }

            static bool hasObjectHandleField(JNIEnv* env, jobject javaObject)
            {
                jfieldID handleField = getHandleField(env, javaObject);
                return handleField != nullptr;
            }

            static JavaHandle* getHandleFromObject(JNIEnv* env, jobject javaObject)
            {
                jfieldID handleField = getHandleField(env, javaObject);

                if(handleField == nullptr)
                {
                    std::string className = JNIUtils::getNameOfClassOfObject(env, javaObject);
                    JBIND_THROW("Error, cannot get JavaHandle from object of class \"" << className << "\".\n"
                    << "Class \"" << className << "\" does not have a field called \"nativeJavaHandle\"\n."
                    << "Apparently, this class is not a wrapped class, i.e., it does not inherit from JBindWrapper.");
                }

                jlong handle = env->GetLongField(javaObject, handleField);
                return reinterpret_cast<JavaHandle*>(handle);
            }

            void assignHandleToObject(JNIEnv* env, jobject javaObject)
            {
                jfieldID handleField = getHandleField(env, javaObject);

                if(handleField == nullptr)
                {
                    std::string className = JNIUtils::getNameOfClassOfObject(env, javaObject);
                    JBIND_THROW("Error, assign handle to object of class \"" << className << "\".\n"
                    << "Class \"" << className << "\" does not have a field called \"nativeJavaHandle\"\n."
                    << "Apparently, this class is not a wrapped class, i.e., it does not inherit from JBindWrapper.");
                }

                jlong handle = reinterpret_cast<jlong>(this);
                env->SetLongField(javaObject, handleField, handle);
            }

    };
}