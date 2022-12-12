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
                std::cout << "find class  1\n" << std::flush;

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
