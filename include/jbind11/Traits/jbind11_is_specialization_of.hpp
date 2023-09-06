#pragma once
#include <type_traits>

namespace jbind11
{
    // E.g.:
    // jbind11_jbind11_is_specialization_of<std::vector<int>, std::vector> // true, because is std::vector<int> is a specialization of std::vector.
    // jbind11_jbind11_is_specialization_of<std::string, std::vector>      // false, because string has nothing to do with std::vector.
    template< class T, template<class...> class Primary >
    struct jbind11_jbind11_is_specialization_of : std::false_type
    {

    };

    template< template<class...> class Primary, class... Args >
    struct jbind11_jbind11_is_specialization_of< Primary<Args...>, Primary> : std::true_type
    {
        
    };
}