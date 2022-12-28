#pragma once

#include <string>

namespace jbind11
{
    // Used to mark functions that return a generic type, e.g.:
    // public<T> T myFunction();
    // (Note, that the function has to include GenericFunction() extra aswell).
    // Do not use this if you want to return a generic class from the function, e.g.:
    // public<T> MyClass<T> myFunction();
    // In that case, please use GenericTypeReturn();
    class GenericReturn
    {
        private:
            std::string identifier = "T";

        public:
            GenericReturn()
            {

            }

            GenericReturn(std::string identifier) : identifier(identifier)
            {

            }

            std::string getIdentifier()
            {
                return identifier;
            }
    };
}