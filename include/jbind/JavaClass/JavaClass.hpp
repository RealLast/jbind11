#pragma once

#include <map>
#include <vector>
#include <string>


#include "jbind/JavaClass/AbstractJavaField.hpp"
#include "jbind/JavaClass/JavaPrimitiveField.hpp"
#include "AbstractJavaClass.hpp"
#include "JavaPackageHandle.hpp"


namespace jbind
{
    template<typename T>
    class JavaClass : public AbstractJavaClass
    {   
        public:
            std::vector<std::shared_ptr<AbstractJavaField>> javaFields;
            std::string className;

            bool ownedByPackage = false;

            AbstractPackageHandle* packageHandle = nullptr;

        public:

            JavaClass()
            {

            }

            ~JavaClass()
            {
                // Move the contents of this object to an object registered in the package.
                packageHandle->transferOwnershipOfClassToPackage(static_cast<AbstractJavaClass*>(this));
                printf("Destructor\n");

            }

            template<typename JavaPackage>
            JavaClass(JavaPackage& package, const char* className)
            {
                this->className = className;
                this->packageHandle = new JavaPackageHandle<JavaPackage, JavaClass<T>>(&package);
                this->ownedByPackage = false;
            }

            template<typename Function>
            JavaClass& def(const char* name, Function&& f)
            {

            }

            // E.g.:
            /* 
                class MyClass
                {
                    int myVar;
                };
                ...
                def_readwrite("myVar", &MyClass::myVar);

                In that case, MemberType is int (the type of the member variable),
                and Class is MyClass (the Class the member variable belongs to).
            */
            template<typename MemberType, typename Class>
            JavaClass& def_readwrite(const char* name, MemberType Class::*pointerToMember)
            {
                printf("Registering field %s\n", name);
                this->javaFields.push_back(std::make_shared<JavaPrimitiveField<MemberType>>(name));
                return *this;
            }

            void print()
            {
                printf("print class %d\n", this->javaFields.size());
                for(std::shared_ptr<AbstractJavaField>& javaField : javaFields)
                {
                    printf("%s\n", javaField->getFieldDeclaration().c_str());
                }
            }
    };
}