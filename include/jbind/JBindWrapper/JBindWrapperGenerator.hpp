#pragma once

#include "JavaPackage/JavaPackage.hpp"
#include "JavaPackageManager.hpp"

namespace jbind
{
    class JBindWrapperGenerator
    {
        public:
            void generateWrapperForClass(AbstractJavaClass* class, std::string& content)
            {
                content = "";

                
            }

            void generateWrappersForPackage(JavaPackage* package, std::map<std::string, std::string>& result)
            {
                // result will contain names of java classes and corresponding generated code.
                result.clear();


            };

            // void generateWrappersForAllPackages(std::map<std::string, std::string>)
            // {
            //     std::vector<std::unique_ptr<JavaPackage>>& packages =
            //         JavaPackageManager::getPackages();
            // }
    };
}