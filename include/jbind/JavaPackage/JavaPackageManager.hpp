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

                for(std::unique_ptr<JavaPackage>& package : javaPackages)
                {
                    if(package->hasClass(className))
                    {
                        return package->getClass(className);
                    }
                }

                return nullptr;
            }
    };
}

std::vector<std::unique_ptr<jbind::JavaPackage>> jbind::JavaPackageManager::javaPackages;