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

#pragma once


#define JBIND_CONCATENATE(a, b) a##b
#define JBIND_TO_STRING(a) #a

#if !defined(JBIND_EXPORT)
#    if defined(WIN32) || defined(_WIN32)
#        define JBIND_EXPORT __declspec(dllexport)
#    else
#        define JBIND_EXPORT __attribute__((visibility("default")))
#    endif
#endif


// defines a static function (jbindInit*) and two static objects:
// First, the JavaPackage and second, the JavaPackageRegistrar.
// The JavaPackagerJavaPackageRegistrar get's the JavaPackage and the init function as paramter.
// E.g., for JBIND_PACKAGE(MyPackage, p), it expands to:
/*

    static void jbindInitMyPackage(jbind11::JavaPackage& p); // DECLARATION of init function, so we can pass it to the packageInitializer before defining it
    extern "C" JBIND_EXPORT jbind11::JavaPackage* jbindMyPackage = new jbind11::JavaPackage("MyPackage");
    extern "C" JBIND_EXPORT jbind11::JavaPackageRegistrar jbindJavaPackageRegistrarMyPackage(&jbindMyPackage, &jbindInitMyPackage);
    static void jbindInitMyPackage(JavaPackage& p)
    // FUNCTION DEFINITION GOES HERE
*/
#define JBIND11_PACKAGE(packageName, packageVariable) \
    static void JBIND_CONCATENATE(jbindInit, packageName)(jbind11::JavaPackage& packageVariable);\
    extern "C" JBIND_EXPORT jbind11::JavaPackage* JBIND_CONCATENATE(jbind11, packageName) = new jbind11::JavaPackage(JBIND_TO_STRING(packageName));\
    extern "C" JBIND_EXPORT jbind11::JavaPackageRegistrar JBIND_CONCATENATE(jbindJavaPackageRegistrar, packageName)(JBIND_CONCATENATE(jbind11, packageName), &JBIND_CONCATENATE(jbindInit, packageName));\
    static void JBIND_CONCATENATE(jbindInit, packageName)(jbind11::JavaPackage& packageVariable)
