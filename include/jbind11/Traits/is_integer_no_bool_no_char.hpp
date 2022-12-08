#pragma once

namespace jbind11
{
    template<typename T> struct is_integer_no_bool_no_char : public std::false_type {};
}

#define JBIND_IS_INTEGER_NO_BOOL_NO_CHAR(type) \
namespace jbind11\
{\
    template<>\
    struct is_integer_no_bool_no_char<type> : public std::true_type {};\
}


JBIND_IS_INTEGER_NO_BOOL_NO_CHAR(short)
JBIND_IS_INTEGER_NO_BOOL_NO_CHAR(unsigned short)

JBIND_IS_INTEGER_NO_BOOL_NO_CHAR(int)
JBIND_IS_INTEGER_NO_BOOL_NO_CHAR(unsigned int)

JBIND_IS_INTEGER_NO_BOOL_NO_CHAR(long)
JBIND_IS_INTEGER_NO_BOOL_NO_CHAR(unsigned long)

JBIND_IS_INTEGER_NO_BOOL_NO_CHAR(long long)
JBIND_IS_INTEGER_NO_BOOL_NO_CHAR(unsigned long long)


