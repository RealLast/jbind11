#pragma once

#include "JavaClass/AbstractJavaClass.hpp"
#include <vector>
#include <memory.h>
#include "jbind_throw.hpp"
namespace jbind
{
    class JavaPackage
    {
        private:    
            std::map<std::string, std::unique_ptr<AbstractJavaClass>> javaClasses; 

            std::string packageName;

        public:
            JavaPackage(std::string packageName) : packageName(packageName)
            {

            }

            template<typename Class>
            void registerClass(Class&& javaClass)
            {
                printf("Register class called\n");
                Class* cls = new Class(std::move(javaClass));
     
                std::unique_ptr<AbstractJavaClass> uniquePtr = std::unique_ptr<AbstractJavaClass>(cls); 
            
                this->javaClasses.insert(std::make_pair(uniquePtr->getClassName(), std::move(uniquePtr)));
            }

            bool hasClass(const std::string& name) const
            {
                auto it = this->javaClasses.find(name);
                return it != this->javaClasses.end();
            }

            AbstractJavaClass* getClass(const std::string& name)
            {
                auto it = this->javaClasses.find(name);
                if(it == this->javaClasses.end())
                {
                    JBIND_THROW("Error, cannot find JavaClass \"" << name << "\" in JavaPackage \"" << packageName << "\"."
                    << "The class was not registered to the package.");
                }

                return it->second.get();
            }

            void print()
            {
                printf("Printing package %s %lu\n", packageName.c_str(), this->javaClasses.size());
                for(auto& entry : javaClasses)
                {
                    entry.second->print();
                }
            }
    };
}



