#pragma once

#include "JavaClass/AbstractJavaClass.hpp"
#include "JavaPackage/JavaPackageManager.hpp"
namespace jbind
{
    template <typename T>
    class JavaClass;
}

#include "JavaClass/JavaClass.hpp"

#include "jbind_throw.hpp"
namespace jbind
{
    // Wrapper around a plain C++ object.
    // Stores the raw pointer to the object as well as it's associated java class.
    class JavaHandle
    {
        private:
            void* rawPtr;
            AbstractJavaClass* javaClass;

            bool initialized = false;

        public:
            
            template<typename T>
            void set(T& value)
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
            T& get()
            {
                if(!this->initialized)
                {
                    JBIND_THROW("Failed to get object reference from JavaHandle. Handle has not been initialized using set() before!");
                }
                return *static_cast<T*>(this->rawPtr);    
            }

            AbstractJavaClass* getJavaClass()
            {
                if(!this->initialized)
                {
                    JBIND_THROW("Failed to get JavaClass from JavaHandle. Handle has not been initialized using set() before!");
                }
                return this->javaClass;
            }

    };
}