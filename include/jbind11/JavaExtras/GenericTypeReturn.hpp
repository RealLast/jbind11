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

#include <string>

namespace jbind11
{
    // Used to mark functions that return a type that itself is generic (e.g., ArrayList<T>).
    // public<T> ArrayList<T> myFunction();
    // (Note, that the function has to include GenericFunction() extra aswell).
    // Do not use this if you want to return a generic type in general, e.g.:
    // public<T> T myFunction();
    // In that case, please use GenericReturn();
    class GenericTypeReturn
    {
        private:
            std::string identifier = "T";

        public:
            GenericTypeReturn()
            {

            }

            GenericTypeReturn(std::string identifier) : identifier(identifier)
            {

            }

            std::string getTag()
            {
                return std::string("<") + identifier + std::string(">");
            }
    };
}