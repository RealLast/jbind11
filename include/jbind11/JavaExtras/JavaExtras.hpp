#pragma once

#include "TypeIdentifier.hpp"
#include "Extras.hpp"
#include <cstdint>

namespace jbind11
{
    class JavaExtras
    {
        private:   
            virtual bool hasExtraWithUniqueIdentifier(intptr_t uniqueIdentifier) const = 0;
            virtual void* getExtraWithUniqueIdentifier(intptr_t uniqueIdentifier) = 0;

        public:
            template<typename T>
            bool hasExtra()
            {
                return hasExtraWithUniqueIdentifier(TypeIdentifier::get<T>());
            }

            template<typename T>
            T& getExtra()
            {
                return *static_cast<T*>(getExtraWithUniqueIdentifier(TypeIdentifier::get<T>()));
            }
    };
}