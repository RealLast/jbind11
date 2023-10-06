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
#include <jni.h>
namespace jbind11
{
    struct JavaHandle;
    struct JavaArrayList;
    class AbstractJavaFunction
    {
        public:
            // Note, that if function is a void function, returned jobject will be nullptr.
            virtual jobject execute(JavaHandle handle, JavaArrayList stack) = 0;

            // Sadly, canonicalClassName is required in order to support static functions,
            // since for those it is required to specify the class name.
            virtual std::string getFunctionDefinition(const std::string& canonicalClassName) = 0;
    };
}