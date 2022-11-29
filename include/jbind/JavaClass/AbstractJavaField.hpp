#pragma once
#include <string>

namespace jbind
{
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

            virtual std::string getFieldDeclaration() = 0;
            virtual std::string getGetter() = 0;
            virtual std::string getSetter() = 0;
        
    };
}