#pragma once


#include "JavaClass/AbstractJavaClass.hpp"

namespace jbind
{
    class AbstractPackageHandle
    {
        public:
            virtual void transferOwnershipOfClassToPackage(AbstractJavaClass* abstractClass, bool& ownerShipVariable) = 0;
    };
}