#pragma once
#include <tuple>
#include "TypeIdentifier.hpp"
#include "JavaExtras/JavaExtras.hpp"
namespace jbind11
{
    template<typename... Extras>
    class JavaExtrasTyped : public JavaExtras
    {
        private:
            std::tuple<Extras...> extras;

            template<int C, typename I, typename... Is>
            bool hasExtraHelper(intptr_t uniqueIdentifier) const
            {
                if(TypeIdentifier::get<I>() == uniqueIdentifier)
                {
                    return true;
                }

                return hasExtraHelper<C + 1, Is...>(uniqueIdentifier);
            }

            template<int C>
            bool hasExtraHelper(intptr_t uniqueIdentifier) const
            {
                // Base case, extra not found
                return false;
            }

            template<int C, typename I, typename... Is>
            void* getExtraHelper(intptr_t uniqueIdentifier)
            {
                if(TypeIdentifier::get<I>() == uniqueIdentifier)
                {
                    // Return ptr to extra.
                    return static_cast<void*>(&std::get<C>(extras));
                }

                return getExtraHelper<C + 1, Is...>(uniqueIdentifier);
            }

            template<int C>
            void* getExtraHelper(intptr_t uniqueIdentifier)
            {
                // Base case
                JBIND_THROW("Error, could not get extra from JavaClass or JavaFunction.\n"
                << "Extra not found / was not registered for jbind11::JavaFunction or jbind11::JavaClass.");

                return nullptr;
            }
            


        public:
            JavaExtrasTyped(Extras... extras) : extras(extras...)
            {

            }

            virtual bool hasExtraWithUniqueIdentifier(intptr_t uniqueIdentifier) const 
            {
                return hasExtraHelper<0, Extras...>(uniqueIdentifier);
            }
           
            virtual void* getExtraWithUniqueIdentifier(intptr_t uniqueIdentifier)
            {
                return getExtraHelper<0, Extras...>(uniqueIdentifier);
            }

           



        
    };
}