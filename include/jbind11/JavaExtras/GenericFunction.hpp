#pragma once

#include <string>

namespace jbind11
{
    class GenericFunction
    {
        private:
            std::string tag = "T";

        public:
            GenericFunction()
            {

            }

            GenericFunction(std::string tag) : tag(tag)
            {

            }

            std::string getTag()
            {
                return std::string("<") + tag + std::string(">");
            }
    };
}