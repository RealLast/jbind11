#pragma once

#include <sstream>

#include "Caster/Caster.hpp"


namespace jbind
{
    // Sufficient for most cases.
    // Mostly, this only would be specialized for classes that
    // want to have support for multiple setters and getters.
    // E.g., for std::vector, we might accept ArrayList<T> or T[] as input for the setter.
    template<typename T, class Enable = void>
    struct SetterGenerator
    {
        static std::string generate(const std::string& fieldName)
        {
            std::stringstream setterDeclaration;
            setterDeclaration << "public void " << "set_" << fieldName << "(" << Caster<T>::canonicalTypeName() << " value)"
                              << "{ nativeSet(\"" << fieldName << "\", value); }";

            return setterDeclaration.str();
        }
    };

    template<typename T, class Enable = void>
    struct GetterGenerator
    {
        static std::string generate(const std::string& fieldName)
        {
            std::stringstream getterDeclaration;
            getterDeclaration << "public " << Caster<T>::canonicalTypeName() << "get_" << fieldName << "()"
                              << "{ return nativeGet(\"" << fieldName << "\"); }";

            return getterDeclaration.str();
        }
    };
}