#pragma once

#include <string>

namespace jbind11
{
    // Used to mark functions that return a type that itself is generic (e.g., ArrayList<T>).
    // public<T> ArrayList<T> myFunction();
    // (Note, that the function has to include GenericFunction() extra aswell).
    // Do not use this if you want to return a generic type in general, e.g.:
    // public<T> T myFunction();
    // In that case, please use GenericReturn();
    class GenericTypeReturn
    {
        private:
            std::string identifier = "T";

        public:
            GenericTypeReturn()
            {

            }

            GenericTypeReturn(std::string identifier) : identifier(identifier)
            {

            }

            std::string getTag()
            {
                return std::string("<") + identifier + std::string(">");
            }
    };
}