#pragma once

#include <string>

namespace jbind11
{
    class GenericClass
    {
        private:
            std::string tag = "T";

        public:
            GenericClass()
            {

            }

            GenericClass(std::string tag) : tag(tag)
            {

            }

            std::string getGenericTag()
            {
                return std::string("<") + tag + std::string(">");
            }
    };
}