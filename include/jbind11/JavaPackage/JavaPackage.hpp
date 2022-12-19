#pragma once

#include "JavaClass/AbstractJavaClass.hpp"
#include <vector>
#include <memory.h>
#include <map>

#include "jbind11_throw.hpp"
namespace jbind11
{
    class JavaPackage
    {
        private:    
            std::map<std::string, std::unique_ptr<AbstractJavaClass>> javaClasses; 

            std::string packageName;

            bool startsWith(const std::string& a, const std::string& b) const
            {
                if(a.compare(0, b.size(), b) == 0)
                {
                    return true;
                }
                return false;
            }

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
            
                this->javaClasses.insert(std::make_pair(uniquePtr->getJavaClassName(), std::move(uniquePtr)));
            }

            std::string getClassNameFromCanonicalName(const std::string& canonicalName) const 
            {
                if(!this->startsWith(canonicalName, this->packageName))
                {
                    JBIND_THROW("Cannot get class name from canonical name \"" << canonicalName << "\".\n"
                    << "The class \"" << canonicalName << "\" does not belong to package \"" << this->packageName << "\"");
                }
                
                size_t length = this->packageName.size() + 1; // + 1 to skip the dot.
                return canonicalName.substr(length, canonicalName.size());
            }

            bool doesCanonicalClassNameBelongToPackage(const std::string& canonicalName) const
            {
                // Canonical name = packageName.className
                // Returns true, if packageName = this->packageName.
                if(!this->startsWith(canonicalName, this->packageName))
                {
                    return false;
                }

                std::string className = this->getClassNameFromCanonicalName(canonicalName);
                return this->hasClass(className);
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

            const std::string& getPackageName() const
            {
                return this->packageName;
            }

            std::vector<std::string> getClassNames() const
            {
                std::vector<std::string> classNames;
                for(const auto& entry : this->javaClasses)
                {
                    classNames.push_back(entry.first);
                }

                return classNames;
            }

         
    };
}



