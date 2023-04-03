#pragma once

// stub JNI header containing no implementations, only the function declarations.
// Needed for crossplattform generation of jbind wrappers.

#include <stdio.h>
#include <stdarg.h>
#define JNICALL
#define JNIEXPORT     __attribute__((visibility("default")))
#define JNIIMPORT     __attribute__((visibility("default")))
#define JNI_VERSION_1_6 16
#define JNI_VERSION_1_7 17
#define JNI_VERSION_1_8 18
#define JNI_VERSION_1_9 19


typedef unsigned char    jboolean;
typedef int16_t          jchar;
typedef int16_t          jshort;
typedef float            jfloat;
typedef double           jdouble;
typedef int32_t          jint;
typedef jint             jsize;
typedef unsigned char    jbyte;
typedef int64_t          jlong;

class _jobject {};
class _jclass : public _jobject {};
class _jthrowable : public _jobject {};
class _jstring : public _jobject {};
class _jarray : public _jobject {};
class _jbooleanArray : public _jarray {};
class _jbyteArray : public _jarray {};
class _jcharArray : public _jarray {};
class _jshortArray : public _jarray {};
class _jintArray : public _jarray {};
class _jlongArray : public _jarray {};
class _jfloatArray : public _jarray {};
class _jdoubleArray : public _jarray {};
class _jobjectArray : public _jarray {};

typedef _jobject *jobject;
typedef _jclass *jclass;
typedef _jthrowable *jthrowable;
typedef _jstring *jstring;
typedef _jarray *jarray;
typedef _jbooleanArray *jbooleanArray;
typedef _jbyteArray *jbyteArray;
typedef _jcharArray *jcharArray;
typedef _jshortArray *jshortArray;
typedef _jintArray *jintArray;
typedef _jlongArray *jlongArray;
typedef _jfloatArray *jfloatArray;
typedef _jdoubleArray *jdoubleArray;
typedef _jobjectArray *jobjectArray;

typedef jobject jweak;

typedef union jvalue {
    jboolean z;
    jbyte    b;
    jchar    c;
    jshort   s;
    jint     i;
    jlong    j;
    jfloat   f;
    jdouble  d;
    jobject  l;
} jvalue;

struct _jfieldID;
typedef struct _jfieldID *jfieldID;

struct _jmethodID;
typedef struct _jmethodID *jmethodID;

/* Return values from jobjectRefType */
typedef enum _jobjectType {
     JNIInvalidRefType    = 0,
     JNILocalRefType      = 1,
     JNIGlobalRefType     = 2,
     JNIWeakGlobalRefType = 3
} jobjectRefType;




/*
 * jboolean constants
 */

#define JNI_FALSE 0
#define JNI_TRUE 1

/*
 * possible return values for JNI functions.
 */

#define JNI_OK           0                 /* success */
#define JNI_ERR          (-1)              /* unknown error */
#define JNI_EDETACHED    (-2)              /* thread detached from the VM */
#define JNI_EVERSION     (-3)              /* JNI version error */
#define JNI_ENOMEM       (-4)              /* not enough memory */
#define JNI_EEXIST       (-5)              /* VM already created */
#define JNI_EINVAL       (-6)              /* invalid arguments */

/*
 * used in ReleaseScalarArrayElements
 */

#define JNI_COMMIT 1
#define JNI_ABORT 2

/*
 * used in RegisterNatives to describe native method name, signature,
 * and function pointer.
 */

typedef struct {
    char *name;
    char *signature;
    void *fnPtr;
} JNINativeMethod;

/*
 * JNI Native Method Interface.
 */


struct JNIEnv_;

#ifdef __cplusplus
typedef JNIEnv_ JNIEnv;
#else
typedef const struct JNINativeInterface_ *JNIEnv;
#endif

/*
 * JNI Invocation Interface.
 */

struct JNIInvokeInterface_;

struct JavaVM_;

typedef JavaVM_ JavaVM;


struct JNINativeInterface_ {
    void *reserved0;
    void *reserved1;
    void *reserved2;

    void *reserved3;
    jint (JNICALL *GetVersion)(JNIEnv *env);

    jclass (JNICALL *DefineClass)
      (JNIEnv *env, const char *name, jobject loader, const jbyte *buf,
       jsize len);
    jclass (JNICALL *FindClass)
      (JNIEnv *env, const char *name);

    jmethodID (JNICALL *FromReflectedMethod)
      (JNIEnv *env, jobject method);
    jfieldID (JNICALL *FromReflectedField)
      (JNIEnv *env, jobject field);

    jobject (JNICALL *ToReflectedMethod)
      (JNIEnv *env, jclass cls, jmethodID methodID, jboolean isStatic);

    jclass (JNICALL *GetSuperclass)
      (JNIEnv *env, jclass sub);
    jboolean (JNICALL *IsAssignableFrom)
      (JNIEnv *env, jclass sub, jclass sup);

    jobject (JNICALL *ToReflectedField)
      (JNIEnv *env, jclass cls, jfieldID fieldID, jboolean isStatic);

    jint (JNICALL *Throw)
      (JNIEnv *env, jthrowable obj);
    jint (JNICALL *ThrowNew)
      (JNIEnv *env, jclass clazz, const char *msg);
    jthrowable (JNICALL *ExceptionOccurred)
      (JNIEnv *env);
    void (JNICALL *ExceptionDescribe)
      (JNIEnv *env);
    void (JNICALL *ExceptionClear)
      (JNIEnv *env);
    void (JNICALL *FatalError)
      (JNIEnv *env, const char *msg);

    jint (JNICALL *PushLocalFrame)
      (JNIEnv *env, jint capacity);
    jobject (JNICALL *PopLocalFrame)
      (JNIEnv *env, jobject result);

    jobject (JNICALL *NewGlobalRef)
      (JNIEnv *env, jobject lobj);
    void (JNICALL *DeleteGlobalRef)
      (JNIEnv *env, jobject gref);
    void (JNICALL *DeleteLocalRef)
      (JNIEnv *env, jobject obj);
    jboolean (JNICALL *IsSameObject)
      (JNIEnv *env, jobject obj1, jobject obj2);
    jobject (JNICALL *NewLocalRef)
      (JNIEnv *env, jobject ref);
    jint (JNICALL *EnsureLocalCapacity)
      (JNIEnv *env, jint capacity);

    jobject (JNICALL *AllocObject)
      (JNIEnv *env, jclass clazz);
    jobject (JNICALL *NewObject)
      (JNIEnv *env, jclass clazz, jmethodID methodID, ...);
    jobject (JNICALL *NewObjectV)
      (JNIEnv *env, jclass clazz, jmethodID methodID, va_list args);
    jobject (JNICALL *NewObjectA)
      (JNIEnv *env, jclass clazz, jmethodID methodID, const jvalue *args);

    jclass (JNICALL *GetObjectClass)
      (JNIEnv *env, jobject obj);
    jboolean (JNICALL *IsInstanceOf)
      (JNIEnv *env, jobject obj, jclass clazz);

    jmethodID (JNICALL *GetMethodID)
      (JNIEnv *env, jclass clazz, const char *name, const char *sig);

    jobject (JNICALL *CallObjectMethod)
      (JNIEnv *env, jobject obj, jmethodID methodID, ...);
    jobject (JNICALL *CallObjectMethodV)
      (JNIEnv *env, jobject obj, jmethodID methodID, va_list args);
    jobject (JNICALL *CallObjectMethodA)
      (JNIEnv *env, jobject obj, jmethodID methodID, const jvalue * args);

    jboolean (JNICALL *CallBooleanMethod)
      (JNIEnv *env, jobject obj, jmethodID methodID, ...);
    jboolean (JNICALL *CallBooleanMethodV)
      (JNIEnv *env, jobject obj, jmethodID methodID, va_list args);
    jboolean (JNICALL *CallBooleanMethodA)
      (JNIEnv *env, jobject obj, jmethodID methodID, const jvalue * args);

    jbyte (JNICALL *CallByteMethod)
      (JNIEnv *env, jobject obj, jmethodID methodID, ...);
    jbyte (JNICALL *CallByteMethodV)
      (JNIEnv *env, jobject obj, jmethodID methodID, va_list args);
    jbyte (JNICALL *CallByteMethodA)
      (JNIEnv *env, jobject obj, jmethodID methodID, const jvalue *args);

    jchar (JNICALL *CallCharMethod)
      (JNIEnv *env, jobject obj, jmethodID methodID, ...);
    jchar (JNICALL *CallCharMethodV)
      (JNIEnv *env, jobject obj, jmethodID methodID, va_list args);
    jchar (JNICALL *CallCharMethodA)
      (JNIEnv *env, jobject obj, jmethodID methodID, const jvalue *args);

    jshort (JNICALL *CallShortMethod)
      (JNIEnv *env, jobject obj, jmethodID methodID, ...);
    jshort (JNICALL *CallShortMethodV)
      (JNIEnv *env, jobject obj, jmethodID methodID, va_list args);
    jshort (JNICALL *CallShortMethodA)
      (JNIEnv *env, jobject obj, jmethodID methodID, const jvalue *args);

    jint (JNICALL *CallIntMethod)
      (JNIEnv *env, jobject obj, jmethodID methodID, ...);
    jint (JNICALL *CallIntMethodV)
      (JNIEnv *env, jobject obj, jmethodID methodID, va_list args);
    jint (JNICALL *CallIntMethodA)
      (JNIEnv *env, jobject obj, jmethodID methodID, const jvalue *args);

    jlong (JNICALL *CallLongMethod)
      (JNIEnv *env, jobject obj, jmethodID methodID, ...);
    jlong (JNICALL *CallLongMethodV)
      (JNIEnv *env, jobject obj, jmethodID methodID, va_list args);
    jlong (JNICALL *CallLongMethodA)
      (JNIEnv *env, jobject obj, jmethodID methodID, const jvalue *args);

    jfloat (JNICALL *CallFloatMethod)
      (JNIEnv *env, jobject obj, jmethodID methodID, ...);
    jfloat (JNICALL *CallFloatMethodV)
      (JNIEnv *env, jobject obj, jmethodID methodID, va_list args);
    jfloat (JNICALL *CallFloatMethodA)
      (JNIEnv *env, jobject obj, jmethodID methodID, const jvalue *args);

    jdouble (JNICALL *CallDoubleMethod)
      (JNIEnv *env, jobject obj, jmethodID methodID, ...);
    jdouble (JNICALL *CallDoubleMethodV)
      (JNIEnv *env, jobject obj, jmethodID methodID, va_list args);
    jdouble (JNICALL *CallDoubleMethodA)
      (JNIEnv *env, jobject obj, jmethodID methodID, const jvalue *args);

    void (JNICALL *CallVoidMethod)
      (JNIEnv *env, jobject obj, jmethodID methodID, ...);
    void (JNICALL *CallVoidMethodV)
      (JNIEnv *env, jobject obj, jmethodID methodID, va_list args);
    void (JNICALL *CallVoidMethodA)
      (JNIEnv *env, jobject obj, jmethodID methodID, const jvalue * args);

    jobject (JNICALL *CallNonvirtualObjectMethod)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID, ...);
    jobject (JNICALL *CallNonvirtualObjectMethodV)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       va_list args);
    jobject (JNICALL *CallNonvirtualObjectMethodA)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       const jvalue * args);

    jboolean (JNICALL *CallNonvirtualBooleanMethod)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID, ...);
    jboolean (JNICALL *CallNonvirtualBooleanMethodV)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       va_list args);
    jboolean (JNICALL *CallNonvirtualBooleanMethodA)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       const jvalue * args);

    jbyte (JNICALL *CallNonvirtualByteMethod)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID, ...);
    jbyte (JNICALL *CallNonvirtualByteMethodV)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       va_list args);
    jbyte (JNICALL *CallNonvirtualByteMethodA)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       const jvalue *args);

    jchar (JNICALL *CallNonvirtualCharMethod)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID, ...);
    jchar (JNICALL *CallNonvirtualCharMethodV)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       va_list args);
    jchar (JNICALL *CallNonvirtualCharMethodA)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       const jvalue *args);

    jshort (JNICALL *CallNonvirtualShortMethod)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID, ...);
    jshort (JNICALL *CallNonvirtualShortMethodV)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       va_list args);
    jshort (JNICALL *CallNonvirtualShortMethodA)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       const jvalue *args);

    jint (JNICALL *CallNonvirtualIntMethod)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID, ...);
    jint (JNICALL *CallNonvirtualIntMethodV)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       va_list args);
    jint (JNICALL *CallNonvirtualIntMethodA)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       const jvalue *args);

    jlong (JNICALL *CallNonvirtualLongMethod)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID, ...);
    jlong (JNICALL *CallNonvirtualLongMethodV)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       va_list args);
    jlong (JNICALL *CallNonvirtualLongMethodA)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       const jvalue *args);

    jfloat (JNICALL *CallNonvirtualFloatMethod)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID, ...);
    jfloat (JNICALL *CallNonvirtualFloatMethodV)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       va_list args);
    jfloat (JNICALL *CallNonvirtualFloatMethodA)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       const jvalue *args);

    jdouble (JNICALL *CallNonvirtualDoubleMethod)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID, ...);
    jdouble (JNICALL *CallNonvirtualDoubleMethodV)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       va_list args);
    jdouble (JNICALL *CallNonvirtualDoubleMethodA)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       const jvalue *args);

    void (JNICALL *CallNonvirtualVoidMethod)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID, ...);
    void (JNICALL *CallNonvirtualVoidMethodV)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       va_list args);
    void (JNICALL *CallNonvirtualVoidMethodA)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       const jvalue * args);

    jfieldID (JNICALL *GetFieldID)
      (JNIEnv *env, jclass clazz, const char *name, const char *sig);

    jobject (JNICALL *GetObjectField)
      (JNIEnv *env, jobject obj, jfieldID fieldID);
    jboolean (JNICALL *GetBooleanField)
      (JNIEnv *env, jobject obj, jfieldID fieldID);
    jbyte (JNICALL *GetByteField)
      (JNIEnv *env, jobject obj, jfieldID fieldID);
    jchar (JNICALL *GetCharField)
      (JNIEnv *env, jobject obj, jfieldID fieldID);
    jshort (JNICALL *GetShortField)
      (JNIEnv *env, jobject obj, jfieldID fieldID);
    jint (JNICALL *GetIntField)
      (JNIEnv *env, jobject obj, jfieldID fieldID);
    jlong (JNICALL *GetLongField)
      (JNIEnv *env, jobject obj, jfieldID fieldID);
    jfloat (JNICALL *GetFloatField)
      (JNIEnv *env, jobject obj, jfieldID fieldID);
    jdouble (JNICALL *GetDoubleField)
      (JNIEnv *env, jobject obj, jfieldID fieldID);

    void (JNICALL *SetObjectField)
      (JNIEnv *env, jobject obj, jfieldID fieldID, jobject val);
    void (JNICALL *SetBooleanField)
      (JNIEnv *env, jobject obj, jfieldID fieldID, jboolean val);
    void (JNICALL *SetByteField)
      (JNIEnv *env, jobject obj, jfieldID fieldID, jbyte val);
    void (JNICALL *SetCharField)
      (JNIEnv *env, jobject obj, jfieldID fieldID, jchar val);
    void (JNICALL *SetShortField)
      (JNIEnv *env, jobject obj, jfieldID fieldID, jshort val);
    void (JNICALL *SetIntField)
      (JNIEnv *env, jobject obj, jfieldID fieldID, jint val);
    void (JNICALL *SetLongField)
      (JNIEnv *env, jobject obj, jfieldID fieldID, jlong val);
    void (JNICALL *SetFloatField)
      (JNIEnv *env, jobject obj, jfieldID fieldID, jfloat val);
    void (JNICALL *SetDoubleField)
      (JNIEnv *env, jobject obj, jfieldID fieldID, jdouble val);

    jmethodID (JNICALL *GetStaticMethodID)
      (JNIEnv *env, jclass clazz, const char *name, const char *sig);

    jobject (JNICALL *CallStaticObjectMethod)
      (JNIEnv *env, jclass clazz, jmethodID methodID, ...);
    jobject (JNICALL *CallStaticObjectMethodV)
      (JNIEnv *env, jclass clazz, jmethodID methodID, va_list args);
    jobject (JNICALL *CallStaticObjectMethodA)
      (JNIEnv *env, jclass clazz, jmethodID methodID, const jvalue *args);

    jboolean (JNICALL *CallStaticBooleanMethod)
      (JNIEnv *env, jclass clazz, jmethodID methodID, ...);
    jboolean (JNICALL *CallStaticBooleanMethodV)
      (JNIEnv *env, jclass clazz, jmethodID methodID, va_list args);
    jboolean (JNICALL *CallStaticBooleanMethodA)
      (JNIEnv *env, jclass clazz, jmethodID methodID, const jvalue *args);

    jbyte (JNICALL *CallStaticByteMethod)
      (JNIEnv *env, jclass clazz, jmethodID methodID, ...);
    jbyte (JNICALL *CallStaticByteMethodV)
      (JNIEnv *env, jclass clazz, jmethodID methodID, va_list args);
    jbyte (JNICALL *CallStaticByteMethodA)
      (JNIEnv *env, jclass clazz, jmethodID methodID, const jvalue *args);

    jchar (JNICALL *CallStaticCharMethod)
      (JNIEnv *env, jclass clazz, jmethodID methodID, ...);
    jchar (JNICALL *CallStaticCharMethodV)
      (JNIEnv *env, jclass clazz, jmethodID methodID, va_list args);
    jchar (JNICALL *CallStaticCharMethodA)
      (JNIEnv *env, jclass clazz, jmethodID methodID, const jvalue *args);

    jshort (JNICALL *CallStaticShortMethod)
      (JNIEnv *env, jclass clazz, jmethodID methodID, ...);
    jshort (JNICALL *CallStaticShortMethodV)
      (JNIEnv *env, jclass clazz, jmethodID methodID, va_list args);
    jshort (JNICALL *CallStaticShortMethodA)
      (JNIEnv *env, jclass clazz, jmethodID methodID, const jvalue *args);

    jint (JNICALL *CallStaticIntMethod)
      (JNIEnv *env, jclass clazz, jmethodID methodID, ...);
    jint (JNICALL *CallStaticIntMethodV)
      (JNIEnv *env, jclass clazz, jmethodID methodID, va_list args);
    jint (JNICALL *CallStaticIntMethodA)
      (JNIEnv *env, jclass clazz, jmethodID methodID, const jvalue *args);

    jlong (JNICALL *CallStaticLongMethod)
      (JNIEnv *env, jclass clazz, jmethodID methodID, ...);
    jlong (JNICALL *CallStaticLongMethodV)
      (JNIEnv *env, jclass clazz, jmethodID methodID, va_list args);
    jlong (JNICALL *CallStaticLongMethodA)
      (JNIEnv *env, jclass clazz, jmethodID methodID, const jvalue *args);

    jfloat (JNICALL *CallStaticFloatMethod)
      (JNIEnv *env, jclass clazz, jmethodID methodID, ...);
    jfloat (JNICALL *CallStaticFloatMethodV)
      (JNIEnv *env, jclass clazz, jmethodID methodID, va_list args);
    jfloat (JNICALL *CallStaticFloatMethodA)
      (JNIEnv *env, jclass clazz, jmethodID methodID, const jvalue *args);

    jdouble (JNICALL *CallStaticDoubleMethod)
      (JNIEnv *env, jclass clazz, jmethodID methodID, ...);
    jdouble (JNICALL *CallStaticDoubleMethodV)
      (JNIEnv *env, jclass clazz, jmethodID methodID, va_list args);
    jdouble (JNICALL *CallStaticDoubleMethodA)
      (JNIEnv *env, jclass clazz, jmethodID methodID, const jvalue *args);

    void (JNICALL *CallStaticVoidMethod)
      (JNIEnv *env, jclass cls, jmethodID methodID, ...);
    void (JNICALL *CallStaticVoidMethodV)
      (JNIEnv *env, jclass cls, jmethodID methodID, va_list args);
    void (JNICALL *CallStaticVoidMethodA)
      (JNIEnv *env, jclass cls, jmethodID methodID, const jvalue * args);

    jfieldID (JNICALL *GetStaticFieldID)
      (JNIEnv *env, jclass clazz, const char *name, const char *sig);
    jobject (JNICALL *GetStaticObjectField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID);
    jboolean (JNICALL *GetStaticBooleanField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID);
    jbyte (JNICALL *GetStaticByteField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID);
    jchar (JNICALL *GetStaticCharField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID);
    jshort (JNICALL *GetStaticShortField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID);
    jint (JNICALL *GetStaticIntField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID);
    jlong (JNICALL *GetStaticLongField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID);
    jfloat (JNICALL *GetStaticFloatField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID);
    jdouble (JNICALL *GetStaticDoubleField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID);

    void (JNICALL *SetStaticObjectField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID, jobject value);
    void (JNICALL *SetStaticBooleanField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID, jboolean value);
    void (JNICALL *SetStaticByteField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID, jbyte value);
    void (JNICALL *SetStaticCharField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID, jchar value);
    void (JNICALL *SetStaticShortField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID, jshort value);
    void (JNICALL *SetStaticIntField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID, jint value);
    void (JNICALL *SetStaticLongField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID, jlong value);
    void (JNICALL *SetStaticFloatField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID, jfloat value);
    void (JNICALL *SetStaticDoubleField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID, jdouble value);

    jstring (JNICALL *NewString)
      (JNIEnv *env, const jchar *unicode, jsize len);
    jsize (JNICALL *GetStringLength)
      (JNIEnv *env, jstring str);
    const jchar *(JNICALL *GetStringChars)
      (JNIEnv *env, jstring str, jboolean *isCopy);
    void (JNICALL *ReleaseStringChars)
      (JNIEnv *env, jstring str, const jchar *chars);

    jstring (JNICALL *NewStringUTF)
      (JNIEnv *env, const char *utf);
    jsize (JNICALL *GetStringUTFLength)
      (JNIEnv *env, jstring str);
    const char* (JNICALL *GetStringUTFChars)
      (JNIEnv *env, jstring str, jboolean *isCopy);
    void (JNICALL *ReleaseStringUTFChars)
      (JNIEnv *env, jstring str, const char* chars);


    jsize (JNICALL *GetArrayLength)
      (JNIEnv *env, jarray array);

    jobjectArray (JNICALL *NewObjectArray)
      (JNIEnv *env, jsize len, jclass clazz, jobject init);
    jobject (JNICALL *GetObjectArrayElement)
      (JNIEnv *env, jobjectArray array, jsize index);
    void (JNICALL *SetObjectArrayElement)
      (JNIEnv *env, jobjectArray array, jsize index, jobject val);

    jbooleanArray (JNICALL *NewBooleanArray)
      (JNIEnv *env, jsize len);
    jbyteArray (JNICALL *NewByteArray)
      (JNIEnv *env, jsize len);
    jcharArray (JNICALL *NewCharArray)
      (JNIEnv *env, jsize len);
    jshortArray (JNICALL *NewShortArray)
      (JNIEnv *env, jsize len);
    jintArray (JNICALL *NewIntArray)
      (JNIEnv *env, jsize len);
    jlongArray (JNICALL *NewLongArray)
      (JNIEnv *env, jsize len);
    jfloatArray (JNICALL *NewFloatArray)
      (JNIEnv *env, jsize len);
    jdoubleArray (JNICALL *NewDoubleArray)
      (JNIEnv *env, jsize len);

    jboolean * (JNICALL *GetBooleanArrayElements)
      (JNIEnv *env, jbooleanArray array, jboolean *isCopy);
    jbyte * (JNICALL *GetByteArrayElements)
      (JNIEnv *env, jbyteArray array, jboolean *isCopy);
    jchar * (JNICALL *GetCharArrayElements)
      (JNIEnv *env, jcharArray array, jboolean *isCopy);
    jshort * (JNICALL *GetShortArrayElements)
      (JNIEnv *env, jshortArray array, jboolean *isCopy);
    jint * (JNICALL *GetIntArrayElements)
      (JNIEnv *env, jintArray array, jboolean *isCopy);
    jlong * (JNICALL *GetLongArrayElements)
      (JNIEnv *env, jlongArray array, jboolean *isCopy);
    jfloat * (JNICALL *GetFloatArrayElements)
      (JNIEnv *env, jfloatArray array, jboolean *isCopy);
    jdouble * (JNICALL *GetDoubleArrayElements)
      (JNIEnv *env, jdoubleArray array, jboolean *isCopy);

    void (JNICALL *ReleaseBooleanArrayElements)
      (JNIEnv *env, jbooleanArray array, jboolean *elems, jint mode);
    void (JNICALL *ReleaseByteArrayElements)
      (JNIEnv *env, jbyteArray array, jbyte *elems, jint mode);
    void (JNICALL *ReleaseCharArrayElements)
      (JNIEnv *env, jcharArray array, jchar *elems, jint mode);
    void (JNICALL *ReleaseShortArrayElements)
      (JNIEnv *env, jshortArray array, jshort *elems, jint mode);
    void (JNICALL *ReleaseIntArrayElements)
      (JNIEnv *env, jintArray array, jint *elems, jint mode);
    void (JNICALL *ReleaseLongArrayElements)
      (JNIEnv *env, jlongArray array, jlong *elems, jint mode);
    void (JNICALL *ReleaseFloatArrayElements)
      (JNIEnv *env, jfloatArray array, jfloat *elems, jint mode);
    void (JNICALL *ReleaseDoubleArrayElements)
      (JNIEnv *env, jdoubleArray array, jdouble *elems, jint mode);

    void (JNICALL *GetBooleanArrayRegion)
      (JNIEnv *env, jbooleanArray array, jsize start, jsize l, jboolean *buf);
    void (JNICALL *GetByteArrayRegion)
      (JNIEnv *env, jbyteArray array, jsize start, jsize len, jbyte *buf);
    void (JNICALL *GetCharArrayRegion)
      (JNIEnv *env, jcharArray array, jsize start, jsize len, jchar *buf);
    void (JNICALL *GetShortArrayRegion)
      (JNIEnv *env, jshortArray array, jsize start, jsize len, jshort *buf);
    void (JNICALL *GetIntArrayRegion)
      (JNIEnv *env, jintArray array, jsize start, jsize len, jint *buf);
    void (JNICALL *GetLongArrayRegion)
      (JNIEnv *env, jlongArray array, jsize start, jsize len, jlong *buf);
    void (JNICALL *GetFloatArrayRegion)
      (JNIEnv *env, jfloatArray array, jsize start, jsize len, jfloat *buf);
    void (JNICALL *GetDoubleArrayRegion)
      (JNIEnv *env, jdoubleArray array, jsize start, jsize len, jdouble *buf);

    void (JNICALL *SetBooleanArrayRegion)
      (JNIEnv *env, jbooleanArray array, jsize start, jsize l, const jboolean *buf);
    void (JNICALL *SetByteArrayRegion)
      (JNIEnv *env, jbyteArray array, jsize start, jsize len, const jbyte *buf);
    void (JNICALL *SetCharArrayRegion)
      (JNIEnv *env, jcharArray array, jsize start, jsize len, const jchar *buf);
    void (JNICALL *SetShortArrayRegion)
      (JNIEnv *env, jshortArray array, jsize start, jsize len, const jshort *buf);
    void (JNICALL *SetIntArrayRegion)
      (JNIEnv *env, jintArray array, jsize start, jsize len, const jint *buf);
    void (JNICALL *SetLongArrayRegion)
      (JNIEnv *env, jlongArray array, jsize start, jsize len, const jlong *buf);
    void (JNICALL *SetFloatArrayRegion)
      (JNIEnv *env, jfloatArray array, jsize start, jsize len, const jfloat *buf);
    void (JNICALL *SetDoubleArrayRegion)
      (JNIEnv *env, jdoubleArray array, jsize start, jsize len, const jdouble *buf);

    jint (JNICALL *RegisterNatives)
      (JNIEnv *env, jclass clazz, const JNINativeMethod *methods,
       jint nMethods);
    jint (JNICALL *UnregisterNatives)
      (JNIEnv *env, jclass clazz);

    jint (JNICALL *MonitorEnter)
      (JNIEnv *env, jobject obj);
    jint (JNICALL *MonitorExit)
      (JNIEnv *env, jobject obj);

    jint (JNICALL *GetJavaVM)
      (JNIEnv *env, JavaVM **vm);

    void (JNICALL *GetStringRegion)
      (JNIEnv *env, jstring str, jsize start, jsize len, jchar *buf);
    void (JNICALL *GetStringUTFRegion)
      (JNIEnv *env, jstring str, jsize start, jsize len, char *buf);

    void * (JNICALL *GetPrimitiveArrayCritical)
      (JNIEnv *env, jarray array, jboolean *isCopy);
    void (JNICALL *ReleasePrimitiveArrayCritical)
      (JNIEnv *env, jarray array, void *carray, jint mode);

    const jchar * (JNICALL *GetStringCritical)
      (JNIEnv *env, jstring string, jboolean *isCopy);
    void (JNICALL *ReleaseStringCritical)
      (JNIEnv *env, jstring string, const jchar *cstring);

    jweak (JNICALL *NewWeakGlobalRef)
       (JNIEnv *env, jobject obj);
    void (JNICALL *DeleteWeakGlobalRef)
       (JNIEnv *env, jweak ref);

    jboolean (JNICALL *ExceptionCheck)
       (JNIEnv *env);

    jobject (JNICALL *NewDirectByteBuffer)
       (JNIEnv* env, void* address, jlong capacity);
    void* (JNICALL *GetDirectBufferAddress)
       (JNIEnv* env, jobject buf);
    jlong (JNICALL *GetDirectBufferCapacity)
       (JNIEnv* env, jobject buf);

    /* New JNI 1.6 Features */

    jobjectRefType (JNICALL *GetObjectRefType)
        (JNIEnv* env, jobject obj);

    /* Module Features */

    jobject (JNICALL *GetModule)
       (JNIEnv* env, jclass clazz);
};


struct JNIEnv_ {
    const struct JNINativeInterface_ *functions;
#ifdef __cplusplus

    jint GetVersion() {
        return -1;
    }
    jclass DefineClass(const char *name, jobject loader, const jbyte *buf,
                       jsize len) {
        return nullptr;
    }
    jclass FindClass(const char *name) {
        return nullptr;
    }
    jmethodID FromReflectedMethod(jobject method) {
        return nullptr;
    }
    jfieldID FromReflectedField(jobject field) {
        return nullptr;
    }

    jobject ToReflectedMethod(jclass cls, jmethodID methodID, jboolean isStatic) {
        return nullptr;
    }

    jclass GetSuperclass(jclass sub) {
        return nullptr;
    }
    jboolean IsAssignableFrom(jclass sub, jclass sup) {
        return false;
    }

    jobject ToReflectedField(jclass cls, jfieldID fieldID, jboolean isStatic) {
        return nullptr;
    }

    jint Throw(jthrowable obj) {
        return 0;
    }
    jint ThrowNew(jclass clazz, const char *msg) {
        return 0;
    }
    jthrowable ExceptionOccurred() {
        return nullptr;
    }
    void ExceptionDescribe() {
    }
    void ExceptionClear() {
    }
    void FatalError(const char *msg) {
    }

    jint PushLocalFrame(jint capacity) {
        return 0;
    }
    jobject PopLocalFrame(jobject result) {
        return nullptr;
    }

    jobject NewGlobalRef(jobject lobj) {
        return nullptr;
    }
    void DeleteGlobalRef(jobject gref) {
    }
    void DeleteLocalRef(jobject obj) {
    }

    jboolean IsSameObject(jobject obj1, jobject obj2) {
        return false;
    }

    jobject NewLocalRef(jobject ref) {
        return nullptr;
    }
    jint EnsureLocalCapacity(jint capacity) {
        return 0;
    }

    jobject AllocObject(jclass clazz) {
        return nullptr;
    }
    jobject NewObject(jclass clazz, jmethodID methodID, ...) {
        return nullptr;
    }
    jobject NewObjectV(jclass clazz, jmethodID methodID,
                       va_list args) {
        return nullptr;
    }
    jobject NewObjectA(jclass clazz, jmethodID methodID,
                       const jvalue *args) {
        return nullptr;              
    }

    jclass GetObjectClass(jobject obj) {
        return nullptr;
    }
    jboolean IsInstanceOf(jobject obj, jclass clazz) {
        return false;
    }

    jmethodID GetMethodID(jclass clazz, const char *name,
                          const char *sig) {
        return nullptr;
    }

    jobject CallObjectMethod(jobject obj, jmethodID methodID, ...) {
        return nullptr;
    }
    jobject CallObjectMethodV(jobject obj, jmethodID methodID,
                        va_list args) {
        return nullptr;
    }
    jobject CallObjectMethodA(jobject obj, jmethodID methodID,
                        const jvalue * args) {
        return nullptr;
    }

    jboolean CallBooleanMethod(jobject obj,
                               jmethodID methodID, ...) {
        return false;
    }
    jboolean CallBooleanMethodV(jobject obj, jmethodID methodID,
                                va_list args) {
        return false;
    }
    jboolean CallBooleanMethodA(jobject obj, jmethodID methodID,
                                const jvalue * args) {
        return false;
    }

    jbyte CallByteMethod(jobject obj, jmethodID methodID, ...) {
        return 0;
    }
    jbyte CallByteMethodV(jobject obj, jmethodID methodID,
                          va_list args) {
        return 0;
    }
    jbyte CallByteMethodA(jobject obj, jmethodID methodID,
                          const jvalue * args) {
        return 0;
    }

    jchar CallCharMethod(jobject obj, jmethodID methodID, ...) {
        return 0;
    }
    jchar CallCharMethodV(jobject obj, jmethodID methodID,
                          va_list args) {
        return 0;
    }
    jchar CallCharMethodA(jobject obj, jmethodID methodID,
                          const jvalue * args) {
        return 0;
    }

    jshort CallShortMethod(jobject obj, jmethodID methodID, ...) {
        return 0;
    }
    jshort CallShortMethodV(jobject obj, jmethodID methodID,
                            va_list args) {
        return 0;
    }
    jshort CallShortMethodA(jobject obj, jmethodID methodID,
                            const jvalue * args) {
        return 0;
    }

    jint CallIntMethod(jobject obj, jmethodID methodID, ...) {
        return 0;
    }
    jint CallIntMethodV(jobject obj, jmethodID methodID,
                        va_list args) {
        return 0;
    }
    jint CallIntMethodA(jobject obj, jmethodID methodID,
                        const jvalue * args) {
        return 0;
    }

    jlong CallLongMethod(jobject obj, jmethodID methodID, ...) {
        return 0;
    }
    jlong CallLongMethodV(jobject obj, jmethodID methodID,
                          va_list args) {
        return 0;
    }
    jlong CallLongMethodA(jobject obj, jmethodID methodID,
                          const jvalue * args) {
        return 0;
    }

    jfloat CallFloatMethod(jobject obj, jmethodID methodID, ...) {
        return 0.0f;
    }
    jfloat CallFloatMethodV(jobject obj, jmethodID methodID,
                            va_list args) {
        return 0.0f;
    }
    jfloat CallFloatMethodA(jobject obj, jmethodID methodID,
                            const jvalue * args) {
        return 0.0f;
    }

    jdouble CallDoubleMethod(jobject obj, jmethodID methodID, ...) {
        return 0.0f;
    }
    jdouble CallDoubleMethodV(jobject obj, jmethodID methodID,
                        va_list args) {
        return 0.0f;
    }
    jdouble CallDoubleMethodA(jobject obj, jmethodID methodID,
                        const jvalue * args) {
        return 0.0f;
    }

    void CallVoidMethod(jobject obj, jmethodID methodID, ...) {
       
    }
    void CallVoidMethodV(jobject obj, jmethodID methodID,
                         va_list args) {

    }
    void CallVoidMethodA(jobject obj, jmethodID methodID,
                         const jvalue * args) {

    }

    jobject CallNonvirtualObjectMethod(jobject obj, jclass clazz,
                                       jmethodID methodID, ...) {
        return nullptr;
    }
    jobject CallNonvirtualObjectMethodV(jobject obj, jclass clazz,
                                        jmethodID methodID, va_list args) {
        return nullptr;
                                                   
    }
    jobject CallNonvirtualObjectMethodA(jobject obj, jclass clazz,
                                        jmethodID methodID, const jvalue * args) {
        return nullptr;
    }

    jboolean CallNonvirtualBooleanMethod(jobject obj, jclass clazz,
                                         jmethodID methodID, ...) {
        return false;
    }
    jboolean CallNonvirtualBooleanMethodV(jobject obj, jclass clazz,
                                          jmethodID methodID, va_list args) {
        return false;

    }
    jboolean CallNonvirtualBooleanMethodA(jobject obj, jclass clazz,
                                          jmethodID methodID, const jvalue * args) {
        return false;

    }

    jbyte CallNonvirtualByteMethod(jobject obj, jclass clazz,
                                   jmethodID methodID, ...) {
        return 0;
    }
    jbyte CallNonvirtualByteMethodV(jobject obj, jclass clazz,
                                    jmethodID methodID, va_list args) {
        return 0;
    }
    jbyte CallNonvirtualByteMethodA(jobject obj, jclass clazz,
                                    jmethodID methodID, const jvalue * args) {
        return 0;
    }

    jchar CallNonvirtualCharMethod(jobject obj, jclass clazz,
                                   jmethodID methodID, ...) {
        return 0;   
    }
    jchar CallNonvirtualCharMethodV(jobject obj, jclass clazz,
                                    jmethodID methodID, va_list args) {
        return 0;

    }
    jchar CallNonvirtualCharMethodA(jobject obj, jclass clazz,
                                    jmethodID methodID, const jvalue * args) {
        return 0;

    }

    jshort CallNonvirtualShortMethod(jobject obj, jclass clazz,
                                     jmethodID methodID, ...) {
        return 0;

    }
    jshort CallNonvirtualShortMethodV(jobject obj, jclass clazz,
                                      jmethodID methodID, va_list args) {
        return 0;

    }
    jshort CallNonvirtualShortMethodA(jobject obj, jclass clazz,
                                      jmethodID methodID, const jvalue * args) {
        return 0;

    }

    jint CallNonvirtualIntMethod(jobject obj, jclass clazz,
                                 jmethodID methodID, ...) {
        return 0;

    }
    jint CallNonvirtualIntMethodV(jobject obj, jclass clazz,
                                  jmethodID methodID, va_list args) {
        return 0;
    }
    jint CallNonvirtualIntMethodA(jobject obj, jclass clazz,
                                  jmethodID methodID, const jvalue * args) {
        return 0;
    }

    jlong CallNonvirtualLongMethod(jobject obj, jclass clazz,
                                   jmethodID methodID, ...) {
        return 0;
    }
    jlong CallNonvirtualLongMethodV(jobject obj, jclass clazz,
                                    jmethodID methodID, va_list args) {
        return 0;
    }
    jlong CallNonvirtualLongMethodA(jobject obj, jclass clazz,
                                    jmethodID methodID, const jvalue * args) {
        return 0;
    }

    jfloat CallNonvirtualFloatMethod(jobject obj, jclass clazz,
                                     jmethodID methodID, ...) {
        return 0;
    }
    jfloat CallNonvirtualFloatMethodV(jobject obj, jclass clazz,
                                      jmethodID methodID,
                                      va_list args) {
        return 0;
    }
    jfloat CallNonvirtualFloatMethodA(jobject obj, jclass clazz,
                                      jmethodID methodID,
                                      const jvalue * args) {
        return 0;
    }

    jdouble CallNonvirtualDoubleMethod(jobject obj, jclass clazz,
                                       jmethodID methodID, ...) {
        return 0;
    }
    jdouble CallNonvirtualDoubleMethodV(jobject obj, jclass clazz,
                                        jmethodID methodID,
                                        va_list args) {
        return 0;
    }
    jdouble CallNonvirtualDoubleMethodA(jobject obj, jclass clazz,
                                        jmethodID methodID,
                                        const jvalue * args) {
        return 0;
    }

    void CallNonvirtualVoidMethod(jobject obj, jclass clazz,
                                  jmethodID methodID, ...) {

    }
    void CallNonvirtualVoidMethodV(jobject obj, jclass clazz,
                                   jmethodID methodID,
                                   va_list args) {

    }
    void CallNonvirtualVoidMethodA(jobject obj, jclass clazz,
                                   jmethodID methodID,
                                   const jvalue * args) {

    }

    jfieldID GetFieldID(jclass clazz, const char *name,
                        const char *sig) {
        return nullptr;
    }

    jobject GetObjectField(jobject obj, jfieldID fieldID) {
        return nullptr;
    }
    jboolean GetBooleanField(jobject obj, jfieldID fieldID) {
        return false;
    }
    jbyte GetByteField(jobject obj, jfieldID fieldID) {
        return 0;
    }
    jchar GetCharField(jobject obj, jfieldID fieldID) {
        return 0;
    }
    jshort GetShortField(jobject obj, jfieldID fieldID) {
        return 0;
    }
    jint GetIntField(jobject obj, jfieldID fieldID) {
        return 0;
    }
    jlong GetLongField(jobject obj, jfieldID fieldID) {
        return 0;
    }
    jfloat GetFloatField(jobject obj, jfieldID fieldID) {
        return 0;
    }
    jdouble GetDoubleField(jobject obj, jfieldID fieldID) {
        return 0;
    }

    void SetObjectField(jobject obj, jfieldID fieldID, jobject val) {

    }
    void SetBooleanField(jobject obj, jfieldID fieldID,
                         jboolean val) {

    }
    void SetByteField(jobject obj, jfieldID fieldID,
                      jbyte val) {

    }
    void SetCharField(jobject obj, jfieldID fieldID,
                      jchar val) {

    }
    void SetShortField(jobject obj, jfieldID fieldID,
                       jshort val) {

    }
    void SetIntField(jobject obj, jfieldID fieldID,
                     jint val) {

    }
    void SetLongField(jobject obj, jfieldID fieldID,
                      jlong val) {

    }
    void SetFloatField(jobject obj, jfieldID fieldID,
                       jfloat val) {

    }
    void SetDoubleField(jobject obj, jfieldID fieldID,
                        jdouble val) {

    }

    jmethodID GetStaticMethodID(jclass clazz, const char *name,
                                const char *sig) {
        return nullptr;
    }

    jobject CallStaticObjectMethod(jclass clazz, jmethodID methodID,
                             ...) {
        return nullptr;

    }
    jobject CallStaticObjectMethodV(jclass clazz, jmethodID methodID,
                              va_list args) {
        return nullptr;
    }
    jobject CallStaticObjectMethodA(jclass clazz, jmethodID methodID,
                              const jvalue *args) {
        return nullptr;
    }

    jboolean CallStaticBooleanMethod(jclass clazz,
                                     jmethodID methodID, ...) {
        return false;
    }
    jboolean CallStaticBooleanMethodV(jclass clazz,
                                      jmethodID methodID, va_list args) {
        return false;
    }
    jboolean CallStaticBooleanMethodA(jclass clazz,
                                      jmethodID methodID, const jvalue *args) {
        return false;
    }

    jbyte CallStaticByteMethod(jclass clazz,
                               jmethodID methodID, ...) {
        return 0;
    }
    jbyte CallStaticByteMethodV(jclass clazz,
                                jmethodID methodID, va_list args) {
        return 0;
    }
    jbyte CallStaticByteMethodA(jclass clazz,
                                jmethodID methodID, const jvalue *args) {
        return 0;
    }

    jchar CallStaticCharMethod(jclass clazz,
                               jmethodID methodID, ...) {
        return 0;
    }
    jchar CallStaticCharMethodV(jclass clazz,
                                jmethodID methodID, va_list args) {
        return 0;
    }
    jchar CallStaticCharMethodA(jclass clazz,
                                jmethodID methodID, const jvalue *args) {
        return 0;
    }

    jshort CallStaticShortMethod(jclass clazz,
                                 jmethodID methodID, ...) {
        return 0;
    }
    jshort CallStaticShortMethodV(jclass clazz,
                                  jmethodID methodID, va_list args) {
        return 0;
    }
    jshort CallStaticShortMethodA(jclass clazz,
                                  jmethodID methodID, const jvalue *args) {
        return 0;
    }

    jint CallStaticIntMethod(jclass clazz,
                             jmethodID methodID, ...) {
        return 0;
    }
    jint CallStaticIntMethodV(jclass clazz,
                              jmethodID methodID, va_list args) {
        return 0;
    }
    jint CallStaticIntMethodA(jclass clazz,
                              jmethodID methodID, const jvalue *args) {
        return 0;
    }

    jlong CallStaticLongMethod(jclass clazz,
                               jmethodID methodID, ...) {
        return 0;
    }
    jlong CallStaticLongMethodV(jclass clazz,
                                jmethodID methodID, va_list args) {
        return 0;
    }
    jlong CallStaticLongMethodA(jclass clazz,
                                jmethodID methodID, const jvalue *args) {
        return 0;
    }

    jfloat CallStaticFloatMethod(jclass clazz,
                                 jmethodID methodID, ...) {
        return 0;
    }
    jfloat CallStaticFloatMethodV(jclass clazz,
                                  jmethodID methodID, va_list args) {
        return 0;
    }
    jfloat CallStaticFloatMethodA(jclass clazz,
                                  jmethodID methodID, const jvalue *args) {
        return 0;
    }

    jdouble CallStaticDoubleMethod(jclass clazz,
                                   jmethodID methodID, ...) {
        return 0;
    }
    jdouble CallStaticDoubleMethodV(jclass clazz,
                                    jmethodID methodID, va_list args) {
        return 0;
    }
    jdouble CallStaticDoubleMethodA(jclass clazz,
                                    jmethodID methodID, const jvalue *args) {
        return 0;
    }

    void CallStaticVoidMethod(jclass cls, jmethodID methodID, ...) {
 
    }
    void CallStaticVoidMethodV(jclass cls, jmethodID methodID,
                               va_list args) {
       
    }
    void CallStaticVoidMethodA(jclass cls, jmethodID methodID,
                               const jvalue * args) {
     
    }

    jfieldID GetStaticFieldID(jclass clazz, const char *name,
                              const char *sig) {
        return nullptr;
    }
    jobject GetStaticObjectField(jclass clazz, jfieldID fieldID) {
        return nullptr;
    }
    jboolean GetStaticBooleanField(jclass clazz, jfieldID fieldID) {
        return false;
    }
    jbyte GetStaticByteField(jclass clazz, jfieldID fieldID) {
        return 0;
    }
    jchar GetStaticCharField(jclass clazz, jfieldID fieldID) {
        return 0;
    }
    jshort GetStaticShortField(jclass clazz, jfieldID fieldID) {
        return 0;
    }
    jint GetStaticIntField(jclass clazz, jfieldID fieldID) {
        return 0;
    }
    jlong GetStaticLongField(jclass clazz, jfieldID fieldID) {
        return 0;
    }
    jfloat GetStaticFloatField(jclass clazz, jfieldID fieldID) {
        return 0;
    }
    jdouble GetStaticDoubleField(jclass clazz, jfieldID fieldID) {
        return 0;
    }

    void SetStaticObjectField(jclass clazz, jfieldID fieldID,
                        jobject value) {
    }
    void SetStaticBooleanField(jclass clazz, jfieldID fieldID,
                        jboolean value) {
    }
    void SetStaticByteField(jclass clazz, jfieldID fieldID,
                        jbyte value) {
    }
    void SetStaticCharField(jclass clazz, jfieldID fieldID,
                        jchar value) {
    }
    void SetStaticShortField(jclass clazz, jfieldID fieldID,
                        jshort value) {
    }
    void SetStaticIntField(jclass clazz, jfieldID fieldID,
                        jint value) {
    }
    void SetStaticLongField(jclass clazz, jfieldID fieldID,
                        jlong value) {
    }
    void SetStaticFloatField(jclass clazz, jfieldID fieldID,
                        jfloat value) {
    }
    void SetStaticDoubleField(jclass clazz, jfieldID fieldID,
                        jdouble value) {
    }

    jstring NewString(const jchar *unicode, jsize len) {
        return nullptr;
    }
    jsize GetStringLength(jstring str) {
        return 0;
    }
    const jchar *GetStringChars(jstring str, jboolean *isCopy) {
        return nullptr;
    }
    void ReleaseStringChars(jstring str, const jchar *chars) {
        
    }

    jstring NewStringUTF(const char *utf) {
        return nullptr;
    }
    jsize GetStringUTFLength(jstring str) {
        return 0;
    }
    const char* GetStringUTFChars(jstring str, jboolean *isCopy) {
        return nullptr;
    }
    void ReleaseStringUTFChars(jstring str, const char* chars) {
       
    }

    jsize GetArrayLength(jarray array) {
        return 0;
    }

    jobjectArray NewObjectArray(jsize len, jclass clazz,
                                jobject init) {
        return nullptr;
    }
    jobject GetObjectArrayElement(jobjectArray array, jsize index) {
        return nullptr;
    }
    void SetObjectArrayElement(jobjectArray array, jsize index,
                               jobject val) {

    }

    jbooleanArray NewBooleanArray(jsize len) {
        return nullptr;
    }
    jbyteArray NewByteArray(jsize len) {
        return nullptr;
    }
    jcharArray NewCharArray(jsize len) {
        return nullptr;
    }
    jshortArray NewShortArray(jsize len) {
        return nullptr;
    }
    jintArray NewIntArray(jsize len) {
        return nullptr;
    }
    jlongArray NewLongArray(jsize len) {
        return nullptr;
    }
    jfloatArray NewFloatArray(jsize len) {
        return nullptr;
    }
    jdoubleArray NewDoubleArray(jsize len) {
        return nullptr;
    }

    jboolean * GetBooleanArrayElements(jbooleanArray array, jboolean *isCopy) {
        return nullptr;
    }
    jbyte * GetByteArrayElements(jbyteArray array, jboolean *isCopy) {
        return nullptr;
    }
    jchar * GetCharArrayElements(jcharArray array, jboolean *isCopy) {
        return nullptr;
    }
    jshort * GetShortArrayElements(jshortArray array, jboolean *isCopy) {
        return nullptr;
    }
    jint * GetIntArrayElements(jintArray array, jboolean *isCopy) {
        return nullptr;
    }
    jlong * GetLongArrayElements(jlongArray array, jboolean *isCopy) {
        return nullptr;
    }
    jfloat * GetFloatArrayElements(jfloatArray array, jboolean *isCopy) {
        return nullptr;
    }
    jdouble * GetDoubleArrayElements(jdoubleArray array, jboolean *isCopy) {
        return nullptr;
    }

    void ReleaseBooleanArrayElements(jbooleanArray array,
                                     jboolean *elems,
                                     jint mode) {
    }
    void ReleaseByteArrayElements(jbyteArray array,
                                  jbyte *elems,
                                  jint mode) {
    }
    void ReleaseCharArrayElements(jcharArray array,
                                  jchar *elems,
                                  jint mode) {
    }
    void ReleaseShortArrayElements(jshortArray array,
                                   jshort *elems,
                                   jint mode) {
    }
    void ReleaseIntArrayElements(jintArray array,
                                 jint *elems,
                                 jint mode) {
    }
    void ReleaseLongArrayElements(jlongArray array,
                                  jlong *elems,
                                  jint mode) {
    }
    void ReleaseFloatArrayElements(jfloatArray array,
                                   jfloat *elems,
                                   jint mode) {
    }
    void ReleaseDoubleArrayElements(jdoubleArray array,
                                    jdouble *elems,
                                    jint mode) {
    }

    void GetBooleanArrayRegion(jbooleanArray array,
                               jsize start, jsize len, jboolean *buf) {
    }
    void GetByteArrayRegion(jbyteArray array,
                            jsize start, jsize len, jbyte *buf) {
    }
    void GetCharArrayRegion(jcharArray array,
                            jsize start, jsize len, jchar *buf) {
    }
    void GetShortArrayRegion(jshortArray array,
                             jsize start, jsize len, jshort *buf) {
    }
    void GetIntArrayRegion(jintArray array,
                           jsize start, jsize len, jint *buf) {
    }
    void GetLongArrayRegion(jlongArray array,
                            jsize start, jsize len, jlong *buf) {
    }
    void GetFloatArrayRegion(jfloatArray array,
                             jsize start, jsize len, jfloat *buf) {
    }
    void GetDoubleArrayRegion(jdoubleArray array,
                              jsize start, jsize len, jdouble *buf) {
    }

    void SetBooleanArrayRegion(jbooleanArray array, jsize start, jsize len,
                               const jboolean *buf) {
    }
    void SetByteArrayRegion(jbyteArray array, jsize start, jsize len,
                            const jbyte *buf) {
    }
    void SetCharArrayRegion(jcharArray array, jsize start, jsize len,
                            const jchar *buf) {
    }
    void SetShortArrayRegion(jshortArray array, jsize start, jsize len,
                             const jshort *buf) {
    }
    void SetIntArrayRegion(jintArray array, jsize start, jsize len,
                           const jint *buf) {
    }
    void SetLongArrayRegion(jlongArray array, jsize start, jsize len,
                            const jlong *buf) {
    }
    void SetFloatArrayRegion(jfloatArray array, jsize start, jsize len,
                             const jfloat *buf) {
    }
    void SetDoubleArrayRegion(jdoubleArray array, jsize start, jsize len,
                              const jdouble *buf) {
    }

    jint RegisterNatives(jclass clazz, const JNINativeMethod *methods,
                         jint nMethods) {
        return 0;
    }
    jint UnregisterNatives(jclass clazz) {
        return  0;
    }

    jint MonitorEnter(jobject obj) {
        return  0;
    }
    jint MonitorExit(jobject obj) {
        return  0;
    }

    jint GetJavaVM(JavaVM **vm) {
        return  0;
    }

    void GetStringRegion(jstring str, jsize start, jsize len, jchar *buf) {
   
    }
    void GetStringUTFRegion(jstring str, jsize start, jsize len, char *buf) {
    }

    void * GetPrimitiveArrayCritical(jarray array, jboolean *isCopy) {
        return nullptr;
    }
    void ReleasePrimitiveArrayCritical(jarray array, void *carray, jint mode) {
    }

    const jchar * GetStringCritical(jstring string, jboolean *isCopy) {
        return nullptr;
    }
    void ReleaseStringCritical(jstring string, const jchar *cstring) {
    }

    jweak NewWeakGlobalRef(jobject obj) {
        return nullptr;
    }
    void DeleteWeakGlobalRef(jweak ref) {
    }

    jboolean ExceptionCheck() {
        return false;
    }

    jobject NewDirectByteBuffer(void* address, jlong capacity) {
        return nullptr;
    }
    void* GetDirectBufferAddress(jobject buf) {
        return nullptr;
    }
    jlong GetDirectBufferCapacity(jobject buf) {
        return 0;
    }
    jobjectRefType GetObjectRefType(jobject obj) {
        return jobjectRefType::JNIInvalidRefType;
    }

    /* Module Features */

    jobject GetModule(jclass clazz) {
        return nullptr;
    }

#endif /* __cplusplus */
};

/*
 * optionString may be any option accepted by the JVM, or one of the
 * following:
 *
 * -D<name>=<value>          Set a system property.
 * -verbose[:class|gc|jni]   Enable verbose output, comma-separated. E.g.
 *                           "-verbose:class" or "-verbose:gc,class"
 *                           Standard names include: gc, class, and jni.
 *                           All nonstandard (VM-specific) names must begin
 *                           with "X".
 * vfprintf                  extraInfo is a pointer to the vfprintf hook.
 * exit                      extraInfo is a pointer to the exit hook.
 * abort                     extraInfo is a pointer to the abort hook.
 */
typedef struct JavaVMOption {
    char *optionString;
    void *extraInfo;
} JavaVMOption;

typedef struct JavaVMInitArgs {
    jint version;

    jint nOptions;
    JavaVMOption *options;
    jboolean ignoreUnrecognized;
} JavaVMInitArgs;

typedef struct JavaVMAttachArgs {
    jint version;

    char *name;
    jobject group;
} JavaVMAttachArgs;

/* These will be VM-specific. */



/* End VM-specific. */

struct JNIInvokeInterface_ {
    void *reserved0;
    void *reserved1;
    void *reserved2;

    jint (JNICALL *DestroyJavaVM)(JavaVM *vm);

    jint (JNICALL *AttachCurrentThread)(JavaVM *vm, void **penv, void *args);

    jint (JNICALL *DetachCurrentThread)(JavaVM *vm);

    jint (JNICALL *GetEnv)(JavaVM *vm, void **penv, jint version);

    jint (JNICALL *AttachCurrentThreadAsDaemon)(JavaVM *vm, void **penv, void *args);
};

struct JavaVM_ {
    const struct JNIInvokeInterface_ *functions;
#ifdef __cplusplus

    jint DestroyJavaVM() {
        return 0;
    }
    jint AttachCurrentThread(void **penv, void *args) {
        return 0;
    }
    jint DetachCurrentThread() {
        return 0;
    }

    jint GetEnv(void **penv, jint version) {
        return 0;
    }
    jint AttachCurrentThreadAsDaemon(void **penv, void *args) {
        return 0;
    }
#endif
};



