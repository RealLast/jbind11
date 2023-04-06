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
        
            JavaPackageRegistrar(volatile JavaPackage* package, void (*initializerFunction)(JavaPackage& package)) 
            {
                JavaPackage* ptr = const_cast<JavaPackage*>(package);
                std::cout << "Registering java package " << ptr->getPackageName() << "\n";
                JavaPackageManager::getInstance()->registerPackage(std::move(*ptr), JavaPackageInitializeFunctionInvoker(initializerFunction));
            }
    };
}