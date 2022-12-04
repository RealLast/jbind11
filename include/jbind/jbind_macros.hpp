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

    static void jbindInitMyPackage(jbind::JavaPackage& p); // DECLARATION of init function, so we can pass it to the packageInitializer before defining it
    extern "C" JBIND_EXPORT jbind::JavaPackage* jbindMyPackage = new jbind::JavaPackage("MyPackage");
    extern "C" JBIND_EXPORT jbind::JavaPackageRegistrar jbindJavaPackageRegistrarMyPackage(&jbindMyPackage, &jbindInitMyPackage);
    static void jbindInitMyPackage(JavaPackage& p)
    // FUNCTION DEFINITION GOES HERE
*/
#define JBIND_PACKAGE(packageName, packageVariable) \
    static void JBIND_CONCATENATE(jbindInit, packageName)(jbind::JavaPackage& packageVariable);\
    extern "C" JBIND_EXPORT jbind::JavaPackage* JBIND_CONCATENATE(jbind, packageName) = new jbind::JavaPackage(JBIND_TO_STRING(packageName));\
    extern "C" JBIND_EXPORT jbind::JavaPackageRegistrar JBIND_CONCATENATE(jbindJavaPackageRegistrar, packageName)(JBIND_CONCATENATE(jbind, packageName), &JBIND_CONCATENATE(jbindInit, packageName));\
    static void JBIND_CONCATENATE(jbindInit, packageName)(jbind::JavaPackage& packageVariable)
