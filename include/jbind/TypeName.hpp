#pragma once
#ifdef __GNUG__

    // When using GNU C++ compiler, we can demangle the RTTI typeid::name string
    // and provide a more readable identifier for data types.
    #include <cstdlib>
    #include <memory>
    #include <cxxabi.h>

#endif

namespace jbind
{
    template<typename T>
    struct TypeName
    {
   
        static std::string get()
        {
            std::string name = typeid(T).name();


        #ifdef __GNUG__
            // When using GNU C++ compiler, we demangle the string returned from typeid::name
            int status = -4; // some arbitrary value to eliminate the compiler warning

            struct handle {
                char* p;
                handle(char* ptr) : p(ptr) { }
                ~handle() { std::free(p); }
            };

            handle result( abi::__cxa_demangle(name.c_str(), NULL, NULL, &status) );

            return (status==0) ? result.p : name ;
        #else
            // Otherwise we return the mangled string.
            
            if (name.find("struct") != std::string::npos)
            {
                name = name.substr(strlen("struct "), name.size() - strlen("struct "));
            }
            return name;
        #endif
        }
    };
}