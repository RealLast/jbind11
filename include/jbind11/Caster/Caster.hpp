#pragma once

#include <jni.h>
#include <stdexcept>

#include "JavaClass/JavaClass.hpp"
#include "JavaHandle/JavaHandle.hpp"

#include "JNIUtils/JNIUtils.hpp"

// Default caster for wrapped objects
// Primitive types (Integer, Float, ...) and supported native
// standard data types (String, Vector, Map, ...) are handled by different casters.
namespace jbind11
{
    template<typename T, class Enable = void>
    class Caster
    {
        public:
            static T fromJavaObject(JNIEnv* env, jobject javaObject) 
            {
                // Check if object has handle field.
                // If yes, get handle from object and extract native object frm it.
                if(!JavaHandle::hasObjectHandleField(env, javaObject))
                {
                    // the passed javaObject is not supported
                    std::string className = JNIUtils::getNameOfClassOfObject(env, javaObject);
                    JBIND_THROW("Error, cannot connvert java object of type \"" << className << "\" to native C++ type.\n"
                    << "The java object is neither a Wrapper (i.e., does not inherit from JBindWrapper) nor a supported native type (like vector, map, ...).");
                }

                JavaHandle handle = 
                    JavaHandle::getHandleFromObject(env, javaObject);                
                
                // This will create a copy of the variable.
                // For now, we only support return by copy policy.
                return *handle.getNativeData<T>();
            }

            static jobject cast(JNIEnv* env, T& value) 
            {
                // Check if a corresponding JavaClass was registered for type T.
                // If yes, use the JavaClass to create the corresponding java object (wrapper).
                
                // The java object takes ownership of the handle and data on the heap.
                // A copy is necessary at this point, because in generall we do not know
                // about the lifetime of value.

                // TODO: allow cast objects without copy, pass it as shared_ptr or as rvalue reference.
                AbstractJavaClass* javaClass = getPackageManager().findClass<JavaClass<T>>();

                if(javaClass == nullptr)
                {
                    JBIND_THROW("Failed to create wrapped java object for native C++ class \"" << TypeName<T>::get() << "\". No JavaClass wrapper was registered for this native type.");
                }

                // canonical name = package.classname
                std::string javaClassCanonicalName = javaClass->getCanonicalName();

                // This calls the constructor of the wrapper class.
                // That constructor will call JBindWrapper_init().
                jobject javaObject = JNIUtils::createObjectFromClassName(env, javaClassCanonicalName, "");


                // In JBindWrapper_init(), a JavaHandle was created and a new instance of T assigned to it.
                // All that is left to do is to copy value into the data of the handle.
                JavaHandle handle = 
                    JavaHandle::getHandleFromObject(env, javaObject);   
                T* handleObject = handle.getNativeData<T>();
                *handleObject = value;

                return javaObject;
            }

            static std::string canonicalTypeName()
            {
                AbstractJavaClass* javaClass = getPackageManager().findClass<JavaClass<T>>();

                if(javaClass == nullptr)
                {
                    JBIND_THROW("Failed to get canonical type name of JBindWrapper for native C++ class \"" << TypeName<T>::get() << "\". No wrapper was registered for this native type.\n"
                    << "Make sure to add a wrapper for this class in an appropriate JBIND_MODULE declaration."
                    << "This usually means that there is at least one data type that has a wrapper and in that wrapper registered a member of type " << TypeName<T>::get() << "\n."
                    << "In other words: There is one data type that depends on " << TypeName<T>::get() << " having a jbind wrapper.");
                }

                return javaClass->getCanonicalName();
            }

            static bool isGeneric()
            {
                AbstractJavaClass* javaClass = getPackageManager().findClass<JavaClass<T>>();

                if(javaClass == nullptr)
                {
                    JBIND_THROW("Failed to retrieve JBindWrapper clss for native C++ class \"" << TypeName<T>::get() << "\". No wrapper was registered for this native type.\n"
                    << "Make sure to add a wrapper for this class in an appropriate JBIND_MODULE declaration.");
                }

                return javaClass->isGenericClass();
            }
    };
}


