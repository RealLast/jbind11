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
#include "Caster/Caster.hpp"
#include "Caster/Primitives.hpp"
#include "Caster/PointerObjects.hpp"
#include "Caster/jobject.hpp"
#include "Caster/jclass.hpp"
#include "Caster/jstring.hpp"
#include "Caster/Enum.hpp"

#include "StandardTypes/StandardTypes.hpp"
#include "JNIUtils/JNIUtils.hpp"

namespace jbind11
{
    template<typename T>
    T fromJavaObject(jobject javaObject)
    {
        return Caster<T>::fromJavaObject(JNIUtils::getEnv(), javaObject);
    }

    template<typename T>
    typename std::enable_if<!std::is_pointer<T>::value, jobject>::type cast(T& value)
    {
        return Caster<T>::cast(JNIUtils::getEnv(), value);
    }

    template<typename T>
    typename std::enable_if<std::is_pointer<T>::value, jobject>::type cast(T value)
    {
        return Caster<T>::cast(JNIUtils::getEnv(), value);
    }
}