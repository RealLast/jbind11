#pragma once

namespace jbind
{
    class AbstractJavaClass
    {
        public:
            virtual ~AbstractJavaClass()
            {
            }

            virtual void print() = 0;
    };
}