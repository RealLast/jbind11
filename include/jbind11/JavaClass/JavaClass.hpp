/***************************************************************************
* Copyright (C) 2023 ETH Zurich
* Core AI & Digital Biomarker, Acoustic and Inflammatory Biomarkers (ADAMMA)
* Centre for Digital Health Interventions (c4dhi.org)
* 
* Authors: Patrick Langer
* 
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
*         http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
***************************************************************************/

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
#include "JavaExtras/JavaExtrasTyped.hpp"
#include "JavaExtras/GenericClass.hpp"

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
            std::shared_ptr<JavaExtras> extras = nullptr;

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
                JavaClass<T>::staticPackageName = this->packageHandle->getPackageName();
            }

        public:
            JavaClass()
            {
                
            }

            // template<typename... Extras>
            // JavaClass(Extras... extras)
            // {
            //     this->extras = std::static_pointer_cast<JavaExtras>(
            //         std::make_shared<JavaExtrasTyped<Extras...>>(extras...)
            //     );
            // }

            template<typename JavaPackage, typename... Extras>
            JavaClass(JavaPackage& package, const char* className, Extras... extras)
            {
                this->className = className;
                this->packageHandle = new JavaPackageHandle<JavaPackage, JavaClass<T>>(&package);
                this->ownedByPackage = false;
                this->extras = std::static_pointer_cast<JavaExtras>(
                    std::make_shared<JavaExtrasTyped<Extras...>>(extras...)
                );
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

            template<typename E>
            bool hasExtra() const
            {
                return extras->hasExtra<E>();
            }

            template<typename E>
            E getExtra()
            {
                return extras->getExtra<E>();
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
                std::shared_ptr<JavaAttribute> attribute(new JavaAttribute(static_cast<AbstractJavaClass*>(this), name));
                this->javaAttributes.insert(std::make_pair(name, attribute));
                return *attribute.get();
            }

            template<typename Class, typename Return, typename... Params, typename... Extras>
            JavaClass& def(const char* name, Return (Class::*p)(Params...), Extras... extras)
            {
                typedef NonStaticJavaFunction<Class, Return, Params...> Function;
                std::string nameStr = name;
                
                std::shared_ptr<Function> func =
                    std::make_shared<Function>(nameStr, p, extras...);

                this->javaFunctions.insert(
                        std::make_pair(name, 
                            std::static_pointer_cast<AbstractJavaFunction>(func)));

                return *this;
            }

            template<typename Return, typename... Params, typename... Extras>
            JavaClass& def_static(const char* name, Return (*p)(Params...), Extras... extras)
            {
                typedef StaticJavaFunction<Return, Params...> Function;
                std::string nameStr = name;
                
                std::shared_ptr<Function> func =
                    std::make_shared<Function>(nameStr, p, extras...);

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

            virtual bool isGenericClass()
            {
                return this->hasExtra<GenericClass>();
            }

            virtual const std::string getGenericJavaClassName()
            {
                std::string className = this->getJavaClassName();
                if(this->hasExtra<GenericClass>())
                {
                    className += this->getExtra<GenericClass>().getGenericTag();
                }
                return className;
            }

            
            virtual bool hasStaticBlock()
            {
                return this->hasExtra<StaticBlock>();
            }

            StaticBlock getStaticBlock()
            {
                return this->getExtra<StaticBlock>();
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


