#pragma once

#include "JavaPackage/JavaPackage.hpp"
#include <vector>
#include <memory.h>

namespace jbind
{
    class JavaPackageManager
    {
        private:
            static std::vector<std::unique_ptr<JavaPackage>> javaPackages;
    
        public:

            void static registerPackage(JavaPackage&& package)
            {
                std::unique_ptr<JavaPackage> uniquePackage = 
                    std::make_unique<JavaPackage>(std::move(package));
            }

            template<typename JavaClass>
            static AbstractJavaClass* findClass()
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
            static AbstractJavaClass* findClassByCanonicalName(const std::string& canonicalName)
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

            const std::vector<std::unique_ptr<JavaPackage>>& getPackages() const
            {
                return javaPackages;
            }
            

    };
}

std::vector<std::unique_ptr<jbind::JavaPackage>> jbind::JavaPackageManager::javaPackages;