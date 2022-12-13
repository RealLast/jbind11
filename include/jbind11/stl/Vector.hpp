#pragma once

#pragma once

#include <jni.h>
#include "Caster/Caster.hpp"
#include "Traits/is_specialization_of.hpp"
#include <vector>

#include "JavaNativeClasses/JavaArrayList.hpp"
#include "JNIUtils/JNIUtils.hpp"


#include "JBindWrapper/GetterSetterGenerator.hpp"

namespace jbind11
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

            static std::vector<Value> fromJavaObject(JNIEnv* env, jobject javaObject)
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

                JavaArrayList javaArrayList(javaObject);

                size_t size = javaArrayList.size();

                for(size_t i = 0; i < javaArrayList.size(); i++)
                {
                    result.push_back(Caster<Value>::fromJavaObject(env, javaArrayList.get(i)));
                }

                return result;
            }

            static jobject cast(JNIEnv* env, std::vector<Value>& vector)
            {
                // we map vector to ArrayList. Since both are native types in their corresponding language,
                // we copy the values.

                JavaArrayList arrayList(vector.size());

                for(Value& value : vector)
                {
                    // use java::Object in the future for automatic
                    // reference handling ? 
                    jobject jval = Caster<Value>::cast(env, value);
                    arrayList.add(jval);
                    env->DeleteLocalRef(jval);
                }

                return arrayList.getJavaObject();
            }

            static std::string canonicalTypeName()
            {
                return "java.util.ArrayList<" + Caster<Value>::canonicalTypeName() + ">";
            }
    };

    // Template specialization for SetterGenerator, since we accept either ArrayList<T> or T[] as input, if T is primitive.
    // Might think about adding support for List<T> and Vector<T>
    template<class T>
    struct SetterGenerator<T, typename std::enable_if<is_specialization_of<T, std::vector>::value && std::is_arithmetic<typename T::Value>::value>::type>
    {
        typedef typename T::value_type Value;
        static std::string generateForArrayList(const std::string& fieldName)
        {
            std::stringstream setterDeclaration;
            setterDeclaration << "public void " << "set_" << fieldName << "(" << Caster<T>::canonicalTypeName() << " value)"
                              << "{ nativeSet(\"" << fieldName << "\", value); }";

            return setterDeclaration.str();
        }

        static std::string generateForPrimitiveArray(const std::string& fieldName)
        {
    

            std::stringstream setterDeclaration;
            setterDeclaration << "public void " << "set_" << fieldName << " (" << JNIUtils::getPrimitiveArrayTypeDeclaration<T>() << " value)"
                              << "{ nativeSet(\"" << fieldName << "\", value); }";

            return setterDeclaration.str();
        }

        static std::string generate(const std::string& fieldName)
        {
            std::stringstream setterDeclaration;
            setterDeclaration << generateForArrayList(fieldName) << "\n"
                              << generateForPrimitiveArray(fieldName);

            return setterDeclaration.str();
        }
    };

}   
