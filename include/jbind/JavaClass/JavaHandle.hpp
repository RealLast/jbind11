#pragma once

#include "AbstractJavaClass.hpp"
#include "jbind/JavaPackage/JavaPackageManager.hpp"
namespace jbind
{
    // Wrapper around a plain C++ object.
    // Stores the raw pointer to the object as well as it's associated java class.
    class JavaHandle
    {
        private:
            void* rawPtr;
            AbstractJavaClass* javaClass;

        public:
            
            template<typename T>
            void set(T& value)
            {
                this->rawPtr = static_cast<void*>(&value);
                this->javaClass = JavaPackageManager::findClass<T>();
            }

            template<typename T>
            T& get()
            {
                return *static_cast<T*>(this->rawPtr);    
            }

    };
}