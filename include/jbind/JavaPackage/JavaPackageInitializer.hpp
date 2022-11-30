#pragma once

#include "JavaPackage.hpp"
#include <iostream>

namespace jbind
{
    class JavaPackageInitializer
    {
        public:
        
            JavaPackageInitializer(JavaPackage& package, void (*initializerFunction)(JavaPackage& package))
            {
                initializerFunction(package);
            }

    };
}