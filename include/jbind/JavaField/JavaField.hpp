#pragma once

namespace jbind
{
    template<typename Class, typename T, class Enable = void>
    class JavaField : public AbstractJavaField 
    {
        public:
            JavaField()
            {
                
            }

            JavaField(const std::string& name) : AbstractJavaField(name)
            {

            }

            virtual ~JavaField()
            {

            }

            // Exctracts the C++ instance from the handle and uses the stored pointer to member to 
            // convert the member to a Java object.
            virtual jobject getValueFromHandle(JNIEnv* env, JavaHandle& javaHandle)
            {

            }

          //  virtual void* getPointerToMember(JavaHandle* handle);

            virtual std::string getFieldDeclaration()
            {

            }

            virtual std::string getGetter()
            {

            }            
            
            virtual std::string getSetter()
            {

            }


         
    };
}