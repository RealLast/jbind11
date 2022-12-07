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

                JavaHandle* handle = 
                    JavaHandle::getHandleFromObject(env, javaObject);                
                
                // This will create a copy of the variable.
                // For now, we only support return by copy policy.
                return *handle->get<T>();
            }

            static jobject cast(JNIEnv* env, T& value)
            {
                // Check if a corresponding JavaClass was registered for type T.
                // If yes, use the JavaClass to create the corresponding java object (wrapper).
                // Copy T to a new instance on heap, store the pointer in a new JavaHandle and assign
                // the java handle to the java wrapper object.
                // The wrapper object takes ownership of the handle and data on the heap.
                // A copy is necessary at this point, because in generall we do not know
                // about the lifetime of value.

                // TODO: If you want to cast objects without copy, pass it as shared_ptr or as rvalue reference.
                AbstractJavaClass* javaClass = JavaPackageManager::findClass<JavaClass<T>>();

                if(javaClass == nullptr)
                {
                    JBIND_THROW("Failed to create wrapped java object for native C++ class \"" << TypeName<T>::get() << "\". No JavaClass wrapper was registered for this native type.");
                }

                // canonical name = package.classname
                std::string javaClassCanonicalName = javaClass->getCanonicalName();

                T* copy = new T(value);
                JavaHandle* handle = new JavaHandle();
                handle->set(copy);


                intptr_t handleAddress = reinterpret_cast<intptr_t>(handle);

                // This calls the private constructor of the wrapper class.
                // Will throw exception on failure.
                jobject object = JNIUtils::createObjectFromClassName(env, javaClassCanonicalName, "L", handleAddress);
                return object;
            }

            static std::string canonicalTypeName()
            {
                AbstractJavaClass* javaClass = JavaPackageManager::findClass<JavaClass<T>>();

                if(javaClass == nullptr)
                {
                    JBIND_THROW("Failed to get canonical type name of JBindWrapper for native C++ class \"" << TypeName<T>::get() << "\". No wrapper was registered for this native type.\n"
                    << "Make sure to add a wrapper for this class in an appropriate JBIND_MODULE declaration.");
                }

                return javaClass->getCanonicalName();
            }
    };
}