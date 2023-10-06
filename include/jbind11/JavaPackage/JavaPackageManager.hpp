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

#include "JavaPackage/JavaPackage.hpp"
#include "JavaPackageInitializeFunctionInvoker.hpp"
#include <vector>
#include <memory.h>
#include <iostream>
namespace jbind11
{
    class JavaPackageManager
    {
        private:
            std::vector<std::unique_ptr<JavaPackage>> javaPackages;
            std::vector<JavaPackageInitializeFunctionInvoker> initializeFunctionInvokers;
    
        public:

            void registerPackage(JavaPackage&& package, JavaPackageInitializeFunctionInvoker&& initializeFunctionInvoker)
            {
                std::unique_ptr<JavaPackage> uniquePackage = 
                    std::make_unique<JavaPackage>(std::move(package));

                javaPackages.push_back(std::move(uniquePackage));
                initializeFunctionInvokers.push_back(std::move(initializeFunctionInvoker));
            }

            template<typename JavaClass>
            AbstractJavaClass* findClass()
            {
                // Get the instance of the class that was registered to the package.
                std::string className = JavaClass::staticClassName;

                if(className == "")
                {
                    return nullptr;
                }

                std::string packageName = JavaClass::staticPackageName;

                if(packageName == "")
                {
                    return nullptr;
                }

                std::string canonicalName = packageName + std::string(".") + className;

                return findClassByCanonicalName(canonicalName);
            }

            // We assume canonical name is full name, i.e. packageName.className
            AbstractJavaClass* findClassByCanonicalName(const std::string& canonicalName)
            {
                for(std::unique_ptr<JavaPackage>& package : javaPackages)
                {
                    // Checks the full class name.
                    // First, it is checked whether the class belongs to the package by checking if the
                    // canonical name starts with the name of the package.
                    // If yes, the className is extracted from the canonicalName, and it is checked whether
                    // the package contains the class or not (that's why we do not check package->hasClass(className) 
                    // manually below).
                    if(package->doesCanonicalClassNameBelongToPackage(canonicalName))
                    {
                        std::string className = package->getClassNameFromCanonicalName(canonicalName);
                        return package->getClass(className);
                    }                    
                }

                return nullptr;
            }

            const std::vector<std::unique_ptr<JavaPackage>>& getPackages() 
            {
                return javaPackages;
            }

            JavaPackage* getPackage(const std::string packageName)
            {
                for(std::unique_ptr<JavaPackage>& ptr : javaPackages)
                {
                    if(ptr->getPackageName() == packageName)
                    {
                        return ptr.get();
                    }
                }
                return nullptr;
            }

            void initializePackages()
            {
                for(size_t i = 0; i < javaPackages.size(); i++)
                {
                    std::unique_ptr<JavaPackage>& packagePtr = javaPackages[i];
                    JavaPackage& package = *packagePtr.get();
                    JavaPackageInitializeFunctionInvoker& invoker = initializeFunctionInvokers[i];
                    invoker.invoke(package);
                }
            }
            

    };

    inline JavaPackageManager& getPackageManager()
    {
        static JavaPackageManager* packageManager = new JavaPackageManager();
        return *packageManager;
    }
}
