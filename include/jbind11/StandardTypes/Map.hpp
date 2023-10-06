/***************************************************************************
* Copyright (C) 2023 ETH Zurich
* Core AI & Digital Biomarker, Acoustic and Inflammatory Biomarkers (ADAMMA)
* Centre for Digital Health Interventions (c4dhi.org)
* 
* Authors: Patrick Langer
* 
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
*         http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
***************************************************************************/

// #pragma once

// #pragma once

// #include <jni.h>
// #include "Caster.hpp"
// #include "Traits/jbind11_is_specialization_of.hpp"
// #include <map>

// namespace jbind11
// {
//     template<class T>
//     struct Caster<T, typename std::enable_if<std::jbind11_is_specialization_of<T, std::map>::value>::type> 
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
