/***************************************************************************
* Copyright (C) 2023 ETH Zurich
* Core AI & Digital Biomarker, Acoustic and Inflammatory Biomarkers (ADAMMA)
* Centre for Digital Health Interventions (c4dhi.org)
* 
* Authors: Patrick Langer
* 
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
*         http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
***************************************************************************/

#pragma once
#ifdef __GNUG__

    // When using GNU C++ compiler, we can demangle the RTTI typeid::name string
    // and provide a more readable identifier for data types.
    #include <cstdlib>
    #include <memory>
    #include <cxxabi.h>

#endif

namespace jbind11
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