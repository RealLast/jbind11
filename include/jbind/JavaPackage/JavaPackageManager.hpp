#pragma once

#include "jbind/JavaPackage/JavaPackage.hpp"
#include <vector>
#include <memory.h>

namespace jbind
{
    class JavaPackageManager
    {
        private:
          std::vector<std::unique_ptr<JavaPackage>> javaPackages;
    
        public:

            template<typename T>
            static AbstractJavaClass* findClass()
            {

            }
    };
}