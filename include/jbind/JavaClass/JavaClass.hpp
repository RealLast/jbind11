#pragma once

#include <map>
#include <vector>
#include <string>


#include "JavaField/AbstractJavaField.hpp"
#include "AbstractJavaClass.hpp"
#include "JavaPackage/JavaPackageHandle.hpp"
#include "TypeName.hpp"
#include "jbind_throw.hpp"
#include "JavaField/JavaPrimitiveField.hpp"

namespace jbind
{
    template<typename T>
    class JavaClass : public AbstractJavaClass
    {   
        public:
            std::map<std::string, std::shared_ptr<AbstractJavaField>> javaFields;
            static std::string staticClassName;
            std::string className;

            bool ownedByPackage = false;

            AbstractPackageHandle* packageHandle = nullptr;

            void registerName()
            {
                // Globally set the class name of this class.
                if(JavaClass<T>::staticClassName != this->className && JavaClass<T>::staticClassName != "")
                {
                    JBIND_THROW("Error, a wrapper for the native C++ class \"" << TypeName<T>::get() << " was created multiple times.\n"
                    << "Once under the name \"" << JavaClass<T>::staticClassName << "\" and another time under the name \"" << this->className << "\"");
                }
                JavaClass<T>::staticClassName = this->className;
            }

        public:

            JavaClass()
            {

            }

            ~JavaClass()
            {
                if(ownedByPackage)
                {
                    return;
                }
                printf("Destructor %s\n", JavaClass<T>::className.c_str());
                std::cout << "\n" << std::flush;
                
                this->registerName();

                // Move the contents of this object to an object registered in the package.
                packageHandle->transferOwnershipOfClassToPackage(static_cast<AbstractJavaClass*>(this), ownedByPackage);
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

            bool hasField(const std::string& fieldName)
            {
                auto it = this->javaFields.find(fieldName);
                return it != this->javaFields.end();
            }

            AbstractJavaField* getField(const std::string& fieldName)
            {
                auto it = this->javaFields.find(fieldName);

                if(it == this->javaFields.end())
                {
                    JBIND_THROW("Error, cannot get field \"" << fieldName << "\" from JavaClass \"" << this->className << "\". Field was not registered");
                }

                return it.second.get();
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
                this->javaFields.insert(std::make_pair(name, std::make_shared<JavaField<Class, MemberType>>(name, pointerToMember)));
                return *this;
            }

            void print()
            {
                for(auto entry : javaFields)
                {
                    printf("%s\n", entry.second->getFieldDeclaration().c_str());
                }
            }

            virtual const std::string& getClassName() const
            {
                return this->className;
            }
    };
}

// JavaClass<T>
// {


//     jobject getFieldValue(void* ptr, std::string fieldName)
//     {
//         return get(value, fieldName);
//     }

//     jobject getFieldValue(T* value, std::string fieldName)
//     {
//         T* handle = javaObject.getHandle();
//         AbstractField* field = class.getField(fieldName);
//         AbstractClass fieldClass = field.getClass();


//         jobject result = fieldClass.get(field.pointerToMember(handle));
//     }   
// }


// jni_get(JniENV* env, jobject object, jstring fieldName)
// {
//     AbstractJavaClass javaClass = object.getClass();
//     AbstractJavaField javaField = javaClass.getField(fieldName);

//     AbstractJavaClass fieldClass = javaField.getClass();
//     fieldClass.get();

// }



template<typename T>
std::string jbind::JavaClass<T>::staticClassName = "";