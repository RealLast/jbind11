/***************************************************************************
* Copyright (C) 2023 ETH Zurich
* Core AI & Digital Biomarker, Acoustic and Inflammatory Biomarkers (ADAMMA)
* Centre for Digital Health Interventions (c4dhi.org)
* 
* Authors: Patrick Langer
* 
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
*         http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
***************************************************************************/

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

            std::shared_ptr<void> rawPtr = nullptr;
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
            void setNativeDataTakeOwnership(T* value)
            {
                this->rawPtr = std::static_pointer_cast<void>(std::shared_ptr<T>(value));
                this->javaClass = getPackageManager().findClass<JavaClass<T>>();

                if(this->javaClass == nullptr)
                {
                    JBIND_THROW("Failed to create JavaHandle for Native C++ class \"" << TypeName<T>::get() << "\". No JavaClass wrapper was registered for this native type.");
                }

                this->nativeDataAssigned = true;
            }

            template<typename T>
            void overrideNativeData(std::shared_ptr<T> data)
            {
                if(!this->nativeDataAssigned)
                {
                    JBIND_THROW("Failed to override native data of type \"" << TypeName<T>::get() << "\"\nOverride is only possible if data has been previously assigned to this handle, however no data was assigned yet.");

                } 

                this->rawPtr = std::static_pointer_cast<void>(data);
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
                return std::static_pointer_cast<T>(this->rawPtr).get();    
            }

            AbstractJavaClass* getJavaClass()
            {
                if(!this->nativeDataAssigned)
                {
                    JBIND_THROW("Failed to get JavaClass from JavaHandle. Handle has not been initialized using setNativeDataTakeOwnership() before!");
                }
                return this->javaClass;
            }

            bool isValid() const
            {
                return this->nativeDataAssigned && this->javaObjectReferenceAssigned;
            }
    };
}