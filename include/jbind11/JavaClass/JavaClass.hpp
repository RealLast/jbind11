#pragma once

#include <map>
#include <vector>
#include <string>


#include "JavaField/AbstractJavaField.hpp"
#include "AbstractJavaClass.hpp"
#include "JavaPackage/JavaPackageHandle.hpp"
#include "TypeName.hpp"
#include "jbind11_throw.hpp"

#include "JavaField/JavaField.hpp"
#include "JavaAttribute/JavaAttribute.hpp"

#include "JavaHandle/JavaHandle.hpp"

#include "JavaFunction/JavaFunction.hpp"

namespace jbind11
{
    template<typename T>
    class JavaClass : public AbstractJavaClass
    {   
        public:
            std::map<std::string, std::shared_ptr<AbstractJavaField>> javaFields;

            // I.e., public static variables.
            std::map<std::string, std::shared_ptr<JavaAttribute>> javaAttributes;

            std::map<std::string, std::shared_ptr<AbstractJavaFunction>> javaFunctions;

            static std::string staticClassName;
            static std::string staticPackageName;

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

                registerPackageName();
            }

            void registerPackageName()
            {
                if(this->packageHandle == nullptr)
                {
                    JBIND_THROW("Error, cannot register JavaClass to a package. A package handle was not provided."
                    << "Make sure to follow jbind11's standard approach for creating JavaClass.");
                }

                // Globally set the package name of this class.
                if(JavaClass<T>::staticPackageName != this->packageHandle->getPackageName() && JavaClass<T>::staticPackageName != "")
                {
                    JBIND_THROW("Error, wrapper for the native C++ class \"" << TypeName<T>::get() << " was added to multiple different packages."
                    << "Once to package \"" << JavaClass<T>::staticPackageName << "\" and once under \"" << this->packageHandle->getPackageName() << "\".");
                }
                JavaClass<T>::staticClassName = this->className;
            }

        public:

            JavaClass()
            {

            }

            ~JavaClass()
            {
                printf("JVC destru\n");
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
                    JBIND_THROW("Error, cannot get field \"" << fieldName << "\" from JavaClass \"" << this->className << "\". Field was not registered.");
                }

                return it->second.get();
            }

            AbstractJavaFunction* getFunction(const std::string& functionName)
            {
                auto it = this->javaFunctions.find(functionName);

                if(it == this->javaFunctions.end())
                {
                    JBIND_THROW("Error, cannot get function \"" << functionName << "\" from JavaClass \"" << this->className << "\". Function was not registered.");
                }

                return it->second.get();
            }

            jbind11::JavaHandle* spawnNewHandle() const
            {
                T* t = new T();
                JavaHandle* handle = new JavaHandle();
                handle->setNativeDataTakeOwnership(t);
                return handle;
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

            // Define a/access static variable
            JavaAttribute& attr(const std::string& name)
            {
                std::shared_ptr<JavaAttribute> attribute(new JavaAttribute(name));
                this->javaAttributes.insert(std::make_pair(name, attribute));
                return *attribute.get();
            }

            template<typename Class, typename Return, typename... Params>
            JavaClass& def(const char* name, Return (Class::*p)(Params...))
            {
                typedef JavaFunction<Class, Return, Params...> Function;
                std::string nameStr = name;
                
                std::shared_ptr<Function> func =
                    std::make_shared<Function>(nameStr, p);

                this->javaFunctions.insert(
                        std::make_pair(name, 
                            std::static_pointer_cast<AbstractJavaFunction>(func)));

                return *this;
            }
           

            virtual const std::string& getJavaClassName() const
            {
                return this->className;
            }

            virtual const std::string getCanonicalName() const
            {
                if(this->packageHandle == nullptr)
                {
                    JBIND_THROW("Error, cannot get canonical name from JavaClass. The class was never assigned to a package.\n"
                    << "Make sure to follow jbind11's standard approach for creating JavaClass.");
                }
                return this->packageHandle->getPackageName() + std::string(".") + this->className;
            }

            virtual std::vector<std::string> getFieldNames()
            {
                std::vector<std::string> fieldNames;
                for(const auto& entry : this->javaFields)
                {
                    fieldNames.push_back(entry.first);
                }
                return fieldNames;
            }

            virtual std::vector<std::string> getFunctionNames()
            {
                std::vector<std::string> functionNames;
                for(const auto& entry : this->javaFunctions)
                {
                    functionNames.push_back(entry.first);
                }
                return functionNames;
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
std::string jbind11::JavaClass<T>::staticClassName = "";

template<typename T>
std::string jbind11::JavaClass<T>::staticPackageName = "";


