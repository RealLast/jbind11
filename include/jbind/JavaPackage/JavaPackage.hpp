#pragma once

#include "jbind/JavaClass/AbstractJavaClass.hpp"
#include <vector>
#include <memory.h>
namespace jbind
{
    class JavaPackage
    {
        private:    
            std::vector<std::unique_ptr<AbstractJavaClass>> javaClasses; 

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
            
                this->javaClasses.push_back(std::move(uniquePtr));
            }

            void print()
            {
                printf("Printing package %s %d\n", packageName.c_str(), this->javaClasses.size());
                for(std::unique_ptr<AbstractJavaClass>& javaClass : javaClasses)
                {
                    javaClass->print();
                }
            }
    };
}



