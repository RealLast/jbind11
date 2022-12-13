#pragma once
#include "JavaField/AbstractJavaField.hpp"
#include "JavaFunction/AbstractJavaFunction.hpp"

namespace jbind11
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

            virtual jbind11::JavaHandle* spawnNewHandle() const = 0;

            virtual AbstractJavaField* getField(const std::string& fieldName) = 0;
            virtual std::vector<std::string> getFieldNames() = 0;

            virtual AbstractJavaFunction* getFunction(const std::string& functionName) = 0;
            virtual std::vector<std::string> getFunctionNames() = 0;

            


    };
}