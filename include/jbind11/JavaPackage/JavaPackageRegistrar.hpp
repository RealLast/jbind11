#pragma once

#include "JavaPackage.hpp"
#include "JavaPackageManager.hpp"
#include "JavaPackageInitializeFunctionInvoker.hpp"
#include <iostream>
#include <functional>

namespace jbind11
{
    class JavaPackageRegistrar
    {
        public:
        
            JavaPackageRegistrar(JavaPackage* package, void (*initializerFunction)(JavaPackage& package)) 
            {
                getPackageManager().registerPackage(std::move(*package), JavaPackageInitializeFunctionInvoker(initializerFunction));
            }
    };
}