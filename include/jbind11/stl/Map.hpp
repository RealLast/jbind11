// #pragma once

// #pragma once

// #include <jni.h>
// #include "Caster.hpp"
// #include "Traits/is_specialization_of.hpp"
// #include <map>

// namespace jbind11
// {
//     template<class T>
//     struct Caster<T, typename std::enable_if<std::is_specialization_of<T, std::map>::value>::type> 
//     {
//         private:
//             typedef T::key_type Key;
//             typedef T::value_type Value;

//         public:

//             static T cast(JNIEnv* env, jobject& javaObject)
//             {
//                 // javaObject is native java type, hence we copy.
//                 std::map<Key, Value> result;

//                 JavaHashMap javaHashMap(javaObject);

//                 for(size_t i = 0; javaHashMap.size(); i++)
//                 {
//                     result.insert(std::make_pair(
//                         Caster<Key>::cast(javaHashMap.getKeyByIndex(i)), 
//                         Caster<Value>::cast(javaHashMap.getValueByIndex(i))));
//                 }

//                 return result;
//             }

//             static jobject toJavaObject(JNIEnv* env, std::map<Key, Value>& value)
//             {
//                 JavaHashMap<Key, Value> javaHashMap = new javaHashMap();
//                 for(std::pair<Key, Value>& entry : map)
//                 {
//                     JavaField<Key> keyField;
//                     JavaField<Value> valueField;

//                     javaHashMap.push_back(Caster<Key>::toJavaObject(entry.first), Caster<Value>::toJavaObject(entry.second));
//                 }

//                 return javaHashMap.getObject();
//              }

       
            
//             jobject get(std::map<Key, Value>& map)
//             {
                
//             }
//     };

// }   
