#pragma once

#pragma once

#include <jni.h>
#include "Caster/Caster.hpp"
#include "Traits/is_specialization_of.hpp"
#include <vector>

#include "JavaNativeClasses/JavaArrayList.hpp"

namespace jbind
{
    template<class T>
    struct Caster<T, typename std::enable_if<is_specialization_of<T, std::vector>::value>::type> 
    {
        private:
            typedef typename T::value_type Value;

            template<typename U = T>
            static typename std::enable_if<std::is_arithmetic<typename U::value_type>::value>::type
            fasterCopy(JNIEnv* env, jobject javaObject, std::vector<Value>& result)
            {
                std::string signature =  JNIUtils::getSignatureOfPrimitiveType<Value>();
                std::string className = JNIUtils::getNameOfClassOfObject(env, javaObject);

                std::string arraySignature = std::string("[") + signature;

                if(className != arraySignature)
                {
                    JBIND_THROW("Cannot convert java ArrayList to std::vector<" << TypeName<Value>::get() << ">.\n"
                        << "An object with signature \"" + className << "\" was provided, but \"" + arraySignature + "\" was expected.")
                }

                std::cout << "is plain array, faster copy \n" << arraySignature << " " << className << "\n" << std::flush;
               
                jsize size = env->GetArrayLength(static_cast<jarray>(javaObject));
                result = std::vector<Value>(size);
                JNIUtils::getPrimitiveArrayRegion(env, static_cast<jarray>(javaObject), 0, size, result.data());
            }

            template<typename U = T>
            static typename std::enable_if<!std::is_arithmetic<typename U::value_type>::value>::type
            fasterCopy(JNIEnv* env, jobject javaObject, std::vector<Value>& result)
            {
                JBIND_THROW("Error, VectorCaster::fasterCopy() called for vector "
                    << "with non-primitive type \"" << TypeName<Value>::get() << "\".");
            }

            static bool isPlainArray(JNIEnv* env, jobject javaObject)
            {
                std::string className = JNIUtils::getNameOfClassOfObject(env, javaObject);

                std::vector<std::string> signatures = {
                "[Z", // bool array
                "[B", // byte array
                "[C", // char array
                "[S", // short array
                "[I", // int array
                "[J", // long array
                "[F", // float array
                "[D", // double array
                };

                return std::find(signatures.begin(), signatures.end(), className) != signatures.end();
            }



        public:

            static std::vector<Value> cast(JNIEnv* env, jobject javaObject)
            {
                std::vector<Value> result;
                // if the java object is a plain array, e.g. jbyte array
                // and T is something like vector<int8_t> (Value is arithmetic),
                // we can use a faster method to copy the data from the object to the vector.
                if(std::is_arithmetic<Value>::value && isPlainArray(env, javaObject))
                {
                    fasterCopy(env, javaObject, result);
                    return result;
                }

                // javaObject is native java type, hence we copy.

                JavaNativeClasses::JavaArrayList javaArrayList(env, javaObject);

                size_t size = javaArrayList.size(env);

                for(size_t i = 0; i < javaArrayList.size(env); i++)
                {
                    result.push_back(Caster<Value>::cast(env, javaArrayList.get(env, i)));
                }

                return result;
            }

            static jobject toJavaObject(JNIEnv* env, std::vector<Value>& vector)
            {
                // we map vector to ArrayList. Since both are native types in their corresponding language,
                // we copy the values.

                JavaNativeClasses::JavaArrayList arrayList(env, vector.size());

                for(Value& value : vector)
                {
                    arrayList.add(env, Caster<Value>::toJavaObject(env, value));
                }

                return arrayList.getJavaObject();
             }
    };

}   
