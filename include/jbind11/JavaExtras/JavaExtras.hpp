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

#include "TypeIdentifier.hpp"
#include "Extras.hpp"
#include <cstdint>

namespace jbind11
{
    class JavaExtras
    {
        private:   
            virtual bool hasExtraWithUniqueIdentifier(intptr_t uniqueIdentifier) const = 0;
            virtual void* getExtraWithUniqueIdentifier(intptr_t uniqueIdentifier) = 0;

        public:
            template<typename T>
            bool hasExtra()
            {
                return hasExtraWithUniqueIdentifier(TypeIdentifier::get<T>());
            }

            template<typename T>
            T& getExtra()
            {
                return *static_cast<T*>(getExtraWithUniqueIdentifier(TypeIdentifier::get<T>()));
            }
    };
}