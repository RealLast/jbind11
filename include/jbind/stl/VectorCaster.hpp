#pragma once

#pragma once

#include <jni.h>
#include "Caster.hpp"
#include "Traits/is_specialization_of.hpp"
#include <vector>

namespace jbind
{
    template<class T>
    struct Caster<T, typename std::enable_if<std::is_specialization_of<T, std::vector>::value>::type> 
    {
        private:
            typedef T::value_type Value;

        public:

            static std::vector<T> cast(JNIEnv* env, jobject& javaObject)
            {
                // javaObject is native java type, hence we copy.
                std::vector<Value> result;

                JavaArrayList javaArrayList(javaObject);

                for(size_t i = 0; javaHashMap.size(); i++)
                {
                    result.push_back(Caster<Value>::cast(javaArrayList.getByIndex(i)));
                }

                return result;
            }

            static jobject toJavaObject(JNIEnv* env, std::vector<Value>& vector)
            {
                // we map vector to ArrayList. Since both are native types in their corresponding language,
                // we copy the values.

                JavaArrayList arrayList = JavaArrayList::create();

                for(Value& value : vector)
                {
                    arrayList.put(Caster<Value>::toJavaObject(env, value));
                }

                return arrayList.getObject();
             }
    };

}   
