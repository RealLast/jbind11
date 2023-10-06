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
#include "AbstractJavaAttributeValue.hpp"
#include "JavaAttributeValue.hpp"
#include "jbind11/cast.hpp"
#include <memory>

namespace jbind11
{
    // static variables of the java class.
    class JavaAttribute
    {
        private:   
            std::string attributeName;
            
            std::shared_ptr<AbstractJavaAttributeValue> attributeValue;

            AbstractJavaClass* parentClass;

        public:

            JavaAttribute(AbstractJavaClass* parentClass, const char* name) : parentClass(parentClass), attributeName(name)
            {

            }

            JavaAttribute(AbstractJavaClass* parentClass, const std::string& name) : parentClass(parentClass), attributeName(name)
            {

            }

            // If T is const, remove constness and cast.
            template<typename T>
            typename std::enable_if<std::is_const<T>::value, JavaAttribute&>::type
            operator=(T& other)
            {
                typedef typename std::remove_const<T>::type NonConstT;
                return operator=(*const_cast<NonConstT*>(&other));
            }

            template<typename T>
            typename std::enable_if<!std::is_const<T>::value, JavaAttribute&>::type
            operator=(T& other)
            {
                this->attributeValue = std::static_pointer_cast<AbstractJavaAttributeValue>(
                    std::make_shared<JavaAttributeValue<T>>(other));
                return *this;    
            }

            operator jobject() const
            {
                return attributeValue->getJavaObject();
            }
    };
}