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

#include "AbstractPackageHandle.hpp"

namespace jbind11
{
    template<typename JavaPackage, typename JavaClass>
    class JavaPackageHandle : public AbstractPackageHandle
    {
        private: 
            JavaPackage* package;

        public:
            JavaPackageHandle(JavaPackage* package) : package(package)
            {

            }
            
            virtual void transferOwnershipOfClassToPackage(AbstractJavaClass* abstractClass, bool& ownerShipVariable)
            {
                // OwnerShipVariable is a reference to JavaClass.isOwnedByPackage.
                // We set it to true before transfering the ownership.
                // Check desctructor of JavaClass for details.
                ownerShipVariable = true;
                JavaClass* javaClass = static_cast<JavaClass*>(abstractClass);
                package->template registerClass<JavaClass>(std::move(*javaClass));
            }

            const std::string& getPackageName() const
            {
                return package->getPackageName();
            }
    };
}