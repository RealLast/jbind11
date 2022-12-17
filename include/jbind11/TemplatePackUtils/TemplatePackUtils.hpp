#pragma once

#include <functional>

// Needed for bind_with_variadic_arguments, see inside ChannelSynchronizer class
// See: https://stackoverflow.com/questions/21192659/variadic-templates-and-stdbind#21193316
template<int> // begin with 0 here!
struct jbind11_placeholder_template
{
};

// Needs to be in namespace std!!
namespace std {

//////////////////////////////////////////////////////////////////////////////

template<int N>
struct is_placeholder<jbind11_placeholder_template<N>>: integral_constant<int, N+1> // the 1 is important
{
};

//////////////////////////////////////////////////////////////////////////////

}

namespace jbind11
{
    template<int ...>
    struct sequence { };

    template<int N, int ...S>
    struct generateIntegerSequence : generateIntegerSequence<N-1, N-1, S...> { };

    template<int ...S>
    struct generateIntegerSequence<0, S...> {
    typedef sequence<S...> type;
    };

    struct TemplatePackUtils
    {
        // bind_with_variadic_arguments (also please check at the beginning of this
        // file for definition of a specialized std jbind11_is_placeholder type_trait).
        template<class Class, class Return, class... Args, int... Is>
        static std::function<Return (Args...)> bind_with_variadic_placeholders(Return (Class::*p)(Args...), Class* obj, sequence<Is...>)
        {
            // std::function<void (Args...)>  y = std::bind(p, obj, boost::placeholders::_1, boost::placeholders::_2);
            std::function<Return (Args...)> x = std::bind(p, obj, jbind11_placeholder_template<Is>{}...);
            return x;
        }

        template<class Class, class Return, class... Args>
        static std::function<Return (Args...)> bind_with_variadic_placeholders(Return (Class::*p)(Args...), Class* obj)
        {
            return bind_with_variadic_placeholders(p, obj, typename generateIntegerSequence<sizeof...(Args)>::type());
        }

        // bind_static_with_variadic_arguments (also please check at the beginning of this
        // file for definition of a specialized std jbind11_is_placeholder type_trait).
        template<class Return, class... Args, int... Is>
        static std::function<Return (Args...)> bind_static_with_variadic_placeholders(Return (*p)(Args...), sequence<Is...>)
        {
            // std::function<void (Args...)>  y = std::bind(p, obj, boost::placeholders::_1, boost::placeholders::_2);
            std::function<Return (Args...)> x = std::bind(p, jbind11_placeholder_template<Is>{}...);
            return x;
        }

        template<class Return, class... Args>
        static std::function<Return (Args...)> bind_static_with_variadic_placeholders(Return (*p)(Args...))
        {
            return bind_static_with_variadic_placeholders(p, typename generateIntegerSequence<sizeof...(Args)>::type());
        }

        template<typename Return, typename... Params, int ...S>
        static Return applyTupleToStdFunction(sequence<S...>, std::tuple<Params...>& tuple, std::function<Return(Params...)> function) 
        {
            return function(std::get<S>(tuple) ...);
        }

        template<typename Return, typename... Params>
        static Return applyTupleToStdFunction(std::tuple<Params...>& tuple, std::function<Return(Params...)> function)
        {
            return applyTupleToStdFunction<Return>(typename generateIntegerSequence<sizeof...(Params)>::type(), tuple, function);
        }
         

        template<typename Class, typename Return, typename... Params, int ...S>
        static Return applyTupleToMemberFunction(sequence<S...>, std::tuple<Params...>& tuple, Return (Class::*function)(Params...), Class& obj) 
        {
            return (obj.*function)(std::get<S>(tuple) ...);
        }

        template<typename Class, typename Return, typename... Params>
        static Return applyTupleToMemberFunction(std::tuple<Params...>& tuple, Return (Class::*function)(Params...), Class& obj)
        {
            return applyTupleToMemberFunction<Return>(typename generateIntegerSequence<sizeof...(Params)>::type(), tuple, function, obj);
        }
    };
}