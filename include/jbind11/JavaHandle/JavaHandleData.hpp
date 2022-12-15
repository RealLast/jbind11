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

#include "AbstractJavaHandleManager.hpp"

namespace jbind11
{

    
    class JavaHandleData
    {
        private:
            AbstractJavaClass* javaClass;

            void* rawPtr = nullptr;
            jobject javaObject = nullptr;

            bool nativeDataAssigned = false;
            bool javaObjectReferenceAssigned = false;

            AbstractJavaHandleManager* handleManager;

        public:
            JavaHandleData(AbstractJavaHandleManager* handleManager)
            {
                this->handleManager = handleManager;
                this->nativeDataAssigned = false;
                this->javaObjectReferenceAssigned = false;

                this->handleManager->registerHandle(this);
            }

            virtual ~JavaHandleData()
            {
                this->handleManager->removeHandle(this);
            }

            template<typename T>
            void setNativeData(T* value)
            {
                this->rawPtr = static_cast<void*>(value);
                this->javaClass = getPackageManager().findClass<JavaClass<T>>();

                if(this->javaClass == nullptr)
                {
                    JBIND_THROW("Failed to create JavaHandle for Native C++ class \"" << TypeName<T>::get() << "\". No JavaClass wrapper was registered for this native type.");
                }

                this->nativeDataAssigned = true;
            }

            void setJavaObjectReference(jobject javaObject)
            {
                this->javaObject = javaObject;
                this->javaObjectReferenceAssigned = true;
            }

            jobject getJavaObjectReference()
            {
                return this->javaObject;
            }

            template<typename T>
            T* getNativeData()
            {
                if(!this->nativeDataAssigned)
                {
                    JBIND_THROW("Failed to get object reference from JavaHandle. Handle has not been initialized using set() before!");
                }
                return static_cast<T*>(this->rawPtr);    
            }

            AbstractJavaClass* getJavaClass()
            {
                if(!this->nativeDataAssigned)
                {
                    JBIND_THROW("Failed to get JavaClass from JavaHandle. Handle has not been initialized using setNativeData() before!");
                }
                return this->javaClass;
            }

            bool isValid() const
            {
                return this->nativeDataAssigned && this->javaObjectReferenceAssigned;
            }
    };
}