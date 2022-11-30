#pragma once

#include <map>
#include <vector>
#include <string>


#include "jbind/JavaClass/AbstractJavaField.hpp"
#include "jbind/JavaClass/JavaPrimitiveField.hpp"
#include "AbstractJavaClass.hpp"
#include "JavaPackageHandle.hpp"
#include "jbind/TypeName.hpp"
#include "jbind/jbind_throw.hpp"

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
                // Globally set the class name of this class.
                if(JavaClass<T>::staticClassName != "")
                {
                    JBIND_THROW("Error, JavaClass \"" << JavaClass<T>::className.c_str() << "\" (wrapper for native class \"" << TypeName<T>::get().c_str() << "\") was created multiple times");
                }
                JavaClass<T>::staticClassName = this->className;
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
                this->javaFields.insert(std::make_pair(name, std::make_shared<JavaPrimitiveField<Class, MemberType>>(name, pointerToMember)));
                return *this;
            }

            void print()
            {
                printf("print class %d\n", this->javaFields.size());
                for(auto entry : javaFields)
                {
                    printf("%s\n", entry.second->getFieldDeclaration().c_str());
                }
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


// template<typename Key, typename Value>
// class JavaMapField
// {
//     jobject get(void* ptr)
//     {
//         return get(static_cast<std::map<Key, Value>(ptr));
//     }

//     jobject get(std::map<Key, Value>& map)
//     {
//         HashMap<Key, Value> javaHashMap = new javaHashMap();
//         for(std::pair<Key, Value>& entry : map)
//         {
//             JavaField<Key> keyField;
//             JavaField<Value> valueField;

//             javaHashMap.push_back(keyField.get(entry.first), valueField.get(entry.second));
//         }

//         return javaHashMap;
//     }
// }

template<typename T>
std::string jbind::JavaClass<T>::staticClassName = "";