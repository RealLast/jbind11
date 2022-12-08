#pragma once

namespace jbind11
{
    template<typename T> struct is_integer_no_bool : public std::false_type {};
}

#define JBIND_IS_INTEGER_NO_BOOL(type) \
namespace jbind11\
{\
    template<>\
    struct is_integer_no_bool<type> : public std::true_type {};\
}

JBIND_IS_INTEGER_NO_BOOL(unsigned char)
JBIND_IS_INTEGER_NO_BOOL(signed char)

JBIND_IS_INTEGER_NO_BOOL(signed short)
JBIND_IS_INTEGER_NO_BOOL(unsigned short)

JBIND_IS_INTEGER_NO_BOOL(signed int)
JBIND_IS_INTEGER_NO_BOOL(unsigned int)

JBIND_IS_INTEGER_NO_BOOL(signed long)
JBIND_IS_INTEGER_NO_BOOL(unsigned long)

JBIND_IS_INTEGER_NO_BOOL(signed long long)
JBIND_IS_INTEGER_NO_BOOL(unsigned long long)


