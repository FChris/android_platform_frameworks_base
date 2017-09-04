#define LOG_TAG "GenGPioService"

#include "jni.h"
#include "JNIHelp.h"
#include "android_runtime/AndroidRuntime.h"

#include <utils/misc.h>
#include <utils/Log.h>
#include <hardware/hardware.h>
#include <hardware/gen_gpio.h>

#include <stdio.h>

namespace android
{

gen_gpio_device_t* gen_gpio_dev;

static jint init_native(JNIEnv *env, jobject clazz) {

    int err;
    hw_module_t* module;
    gen_gpio_device_t* dev = NULL;

    err = hw_get_module(GEN_GPIO_HARDWARE_MODULE_ID, (hw_module_t const**) &module);
    if (err == 0) {
        if (module->methods->open(module, "", ((hw_device_t**) &dev)) != 0) {
            return 0;
        }
    }

    return (jint)dev;
};

static void finalize_native(JNIEnv *env, jobject clazz, int ptr) {

    gen_gpio_device_t* dev = (gen_gpio_device_t*) ptr;

    if (dev == NULL) {
        return;
    }

    free(dev);
};

static int read_native(JNIEnv *env, jobject clazz, int ptr, jbyteArray buffer, int gpio_pin) {
    gen_gpio_device_t* dev = (gen_gpio_device_t*) ptr;
    jbyte* real_byte_array;
    int length;

    real_byte_array = env->GetByteArrayElements(buffer, NULL);

    if (dev == NULL) {
        return 0;
    }

    length = dev->read((char*) real_byte_array, env->GetArrayLength(buffer), gpio_pin);

    env->ReleaseByteArrayElements(buffer, real_byte_array, 0);

    return length;
};

static int write_native(JNIEnv *env, jobject clazz, int ptr, jbyteArray buffer, int gpio_pin) {
    gen_gpio_device_t* dev = (gen_gpio_device_t*) ptr;
    jbyte* real_byte_array;
    int length;

    real_byte_array = env->GetByteArrayElements(buffer, NULL);

    if (dev == NULL) {
        return 0;
    }

    length = dev->write((char*) buffer, env->GetArrayLength(buffer), gpio_pin);

    env->ReleaseByteArrayElements(buffer, real_byte_array, 0);

    return length;
};

static JNINativeMethod method_table[] = {
    { "init_native", "()I", (void*) init_native},
    { "finalize_native", "(I)V", (void*) finalize_native},
    { "read_native", "(I[BI)I", (void*) read_native },
    { "write_native", "(I[BI)I", (void*) write_native } ,
};

int register_android_server_GenGpioService(JNIEnv *env) {
    return jniRegisterNativeMethods(env, "com/android/server/GenGpioService",
            method_table, NELEM(method_table));
};

};
