#pragma once
#include "JavaField/AbstractJavaField.hpp"

namespace jbind
{
    class JavaHandle;

    class AbstractJavaClass
    {
        public:
            virtual ~AbstractJavaClass()
            {
            }


            virtual const std::string& getJavaClassName() const = 0; 

            // packagename.classname
            virtual const std::string getCanonicalName() const = 0;

            virtual jbind::JavaHandle* spawnNewObject() const = 0;

            virtual AbstractJavaField* getField(const std::string& fieldName) = 0;

            virtual const std::map<std::string, std::shared_ptr<AbstractJavaField>>& getFields() const = 0;
    };
}