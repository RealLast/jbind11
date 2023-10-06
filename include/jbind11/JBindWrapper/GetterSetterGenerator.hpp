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

#include <sstream>

#include "Caster/Caster.hpp"


namespace jbind11
{
    // Sufficient for most cases.
    // Mostly, this only would be specialized for classes that
    // want to have support for multiple setters and getters.
    // E.g., for std::vector, we might accept ArrayList<T> or T[] as input for the setter.
    template<typename T, class Enable = void>
    struct SetterGenerator
    {
        static std::string generate(const std::string& fieldName)
        {
            std::stringstream setterDeclaration;
            setterDeclaration << "public void " << "set_" << fieldName << " (" << Caster<T>::canonicalTypeName() << " value)"
                              << "{ nativeSet(\"" << fieldName << "\", value); }";

            return setterDeclaration.str();
        }
    };

    template<typename T, class Enable = void>
    struct GetterGenerator
    {
        static std::string generate(const std::string& fieldName)
        {
            std::stringstream getterDeclaration;
            getterDeclaration << "public " << Caster<T>::canonicalTypeName() << " get_" << fieldName << "()"
                              << "{ return (" << Caster<T>::canonicalTypeName() << ") nativeGet(\"" << fieldName << "\"); }";

            return getterDeclaration.str();
        }
    };
}