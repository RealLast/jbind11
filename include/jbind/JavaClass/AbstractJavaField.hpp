#pragma once
#include <string>
#include "JavaHandle.hpp"

namespace jbind
{
    // A JavaField stores the name of a field of a java class as well as
    // the pointer to member of the wrapped class (see JavaPrimitiveField and JavaObjectField).
    // It allows to access the member value of a wrapped JavaClass, if a JavaHandle to the associated class object is provided.
    // E.g.:
    /*
        struct TestStruct
        {
            int val;
        };

        class JavaClass<TestStruct>
        {
            JavaClass()
            {
                def_readwrite("val", &TestStruct::val);
            }
        }

        TestStruct instance;
        JavaHandle handle;
        
        // Automatically looks up JavaClass<TestStruct>
        handle.set(instance);

        JavaClass<TestStruct> javaClass = handle.getClass();
        AbstractJavaField field = javaClass.getField("val");

        // Get's instance.val and converts it to java.lang.Integer
        jobject value = field.getValueFromHandle(handle);        

    */
    class AbstractJavaField
    {  
        protected:
            std::string name;

        public:
            
            AbstractJavaField()
            {
                
            }

            AbstractJavaField(const std::string& name) : name(name)
            {

            }

            virtual ~AbstractJavaField()
            {

            }

            // Exctracts the C++ instance from the handle, and uses the pointer to member to 
            // convert the member to a Java object.
            virtual jobject getValueFromHandle(JavaHandle& javaHandle) = 0;

          //  virtual void* getPointerToMember(JavaHandle* handle);

            virtual std::string getFieldDeclaration() = 0;
            virtual std::string getGetter() = 0;
            virtual std::string getSetter() = 0;
        
    };
}