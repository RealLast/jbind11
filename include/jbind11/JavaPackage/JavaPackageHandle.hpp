#pragma once

#include "AbstractPackageHandle.hpp"

namespace jbind11
{
    template<typename JavaPackage, typename JavaClass>
    class JavaPackageHandle : public AbstractPackageHandle
    {
        private: 
            JavaPackage* package;

        public:
            JavaPackageHandle(JavaPackage* package) : package(package)
            {

            }
            
            virtual void transferOwnershipOfClassToPackage(AbstractJavaClass* abstractClass, bool& ownerShipVariable)
            {
                // OwnerShipVariable is a reference to JavaClass.isOwnedByPackage.
                // We set it to true before transfering the ownership.
                // Check desctructor of JavaClass for details.
                ownerShipVariable = true;
                JavaClass* javaClass = static_cast<JavaClass*>(abstractClass);
                package->template registerClass<JavaClass>(std::move(*javaClass));
            }

            const std::string& getPackageName() const
            {
                return package->getPackageName();
            }
    };
}