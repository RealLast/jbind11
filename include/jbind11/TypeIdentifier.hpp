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

#include <cstdint>


namespace jbind11
{
    namespace TypeIdentifier
    {
        // Use extern to make sure the adress of this function is always the same no matter where it is used within the code.
        template<typename T>
        extern intptr_t get()
        {
            // Use the address of this function for the current data type as unique identifier.
			// Windows is a bitch. 
			// On Linux/Unix, can just do return reinterpret_cast<intptr_t>(&getDataTypeUniqueIdentifier<T>)
			auto fptr = &get<T>;
			void* tmp = reinterpret_cast<void *&>(fptr);
            return reinterpret_cast<intptr_t>(tmp);
        }
    }
    
}   

