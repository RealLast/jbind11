#pragma once

namespace jbind
{
    class AbstractJavaClass
    {
        public:
            virtual ~AbstractJavaClass()
            {
            }

            virtual void print() = 0;

            virtual const std::string& getClassName() const = 0; 
    };
}