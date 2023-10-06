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
#include "JavaField/AbstractJavaField.hpp"
#include "JavaFunction/AbstractJavaFunction.hpp"
#include "JavaExtras/StaticBlock.hpp"
#include <vector>
namespace jbind11
{
    class JavaHandle;

    class AbstractJavaClass
    {
        public:
            virtual ~AbstractJavaClass()
            {
            }


            virtual const std::string& getJavaClassName() const = 0; 

            // packagename.classname
            virtual const std::string getCanonicalName() const = 0;

            virtual jbind11::JavaHandle* spawnNewHandle() const = 0;

            virtual AbstractJavaField* getField(const std::string& fieldName) = 0;
            virtual std::vector<std::string> getFieldNames() = 0;

            virtual AbstractJavaFunction* getFunction(const std::string& functionName) = 0;
            virtual std::vector<std::string> getFunctionNames() = 0;

            virtual bool isGenericClass() = 0;
            // returns classname<T> (T specified by GenericClass() extra, could also be "U" or sth else),
            // if class is generic, otherwise classname.
            virtual const std::string getGenericJavaClassName() = 0;

            virtual bool hasStaticBlock() = 0;
            virtual StaticBlock getStaticBlock() = 0;

            


    };
}