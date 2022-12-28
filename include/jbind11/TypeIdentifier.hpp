#pragma once

#include <cstdint>


namespace jbind11
{
    namespace TypeIdentifier
    {
        // Use extern to make sure the adress of this function is always the same no matter where it is used within the code.
        template<typename T>
        extern intptr_t get()
        {
            // Use the address of this function for the current data type as unique identifier.
			// Windows is a bitch. 
			// On Linux/Unix, can just do return reinterpret_cast<intptr_t>(&getDataTypeUniqueIdentifier<T>)
			auto fptr = &get<T>;
			void* tmp = reinterpret_cast<void *&>(fptr);
            return reinterpret_cast<intptr_t>(tmp);
        }
    }
    
}   

