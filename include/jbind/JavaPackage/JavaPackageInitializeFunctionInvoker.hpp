#pragma once

#include "JavaPackage/JavaPackage.hpp"
#include <functional>


namespace jbind
{
    class JavaPackageInitializeFunctionInvoker
    {
        private:
            std::function<void (JavaPackage& package)> initializerFunction;

        public:
            JavaPackageInitializeFunctionInvoker(
                std::function<void (JavaPackage& package)> initializerFunction) : initializerFunction(initializerFunction)
            {
                
            }

            void invoke(JavaPackage& package)
            {
                initializerFunction(package);
            }
    };
}