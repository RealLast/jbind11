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

#include <mutex>
#include "AbstractJavaHandleManager.hpp"
#include "JavaHandle.hpp"
namespace jbind11
{
    class JavaHandleManager : public AbstractJavaHandleManager
    {
        private:
            // Remember, that jobject is a pointer.
            
            std::vector<JavaHandleData*> registeredJavaObjects;
            std::mutex mutex;

        public:
            void registerHandle(JavaHandleData* data)
            {
                std::lock_guard<std::mutex> lock(this->mutex);
                this->registeredJavaObjects.push_back(data);
            }

            void removeHandle(JavaHandleData* data)
            {
                std::lock_guard<std::mutex> lock(this->mutex);
                for(auto it = this->registeredJavaObjects.begin(); it != this->registeredJavaObjects.end(); it++)
                {
                    if(*it == data)
                    {
                        this->registeredJavaObjects.erase(it);
                        return;
                    }
                }
            }

            JavaHandle findRegisteredJavaInstance(jobject instance)
            {
                std::lock_guard<std::mutex> lock(this->mutex);
                JavaHandle handle = JavaHandle::getHandleFromObject(JNIUtils::getEnv(), instance);

                if(!handle.isValid())
                {
                    JBIND_THROW("Error, found handle for java object of type \"" << JNIUtils::getNameOfClassOfObject(JNIUtils::getEnv(), instance) << "\",\n"
                    << " but handle is invalid. It seems the object wasnt allocated properly.")
                }

                return handle;


                // Dont call default handle constructor!
                // Handle constructor will create data.
                // Data will call registerHandle, which calls mutex.
                // Deadlock!
                // return JavaHandle::invalidHandle();
            }

            template<typename T>
            JavaHandle findRegisteredNativeInstance(T* data)
            {
                if(data == nullptr)
                {
                    JBIND_THROW("Failed to find java handle for object of type \"" << TypeName<T>::get() << "\".\n"
                    << "Data is null.")
                }
                std::lock_guard<std::mutex> lock(this->mutex);
                void* address = reinterpret_cast<void*>(data);
                void* managedDataAddress;

                for(auto it = this->registeredJavaObjects.begin(); it != this->registeredJavaObjects.end(); it++)
                {
                    // Compare addresses of objects.
                    // See if any handle is responsible for this object.
                    managedDataAddress = static_cast<void*>((*it)->getNativeData<void>());
                    if(managedDataAddress == address)
                    {
                        jobject javaObject = (*it)->getJavaObjectReference();
                        JavaHandle handle = JavaHandle::getHandleFromObject(JNIUtils::getEnv(), javaObject);

                        if(!handle.isValid())
                        {
                            JBIND_THROW("Error, found handle for java object of type \"" << JNIUtils::getNameOfClassOfObject(JNIUtils::getEnv(), javaObject) << "\",\n"
                            << " but handle is invalid. It seems the object wasnt allocated properly.")
                        }

                        return handle;
                    }
                }

                return JavaHandle::InvalidHandle();
            }
    };

    inline JavaHandleManager& getHandles()
    {
        static JavaHandleManager* javaHandles = new JavaHandleManager();
        return *javaHandles;
    }
}

