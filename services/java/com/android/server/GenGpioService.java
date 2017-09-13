package com.android.server;

import android.content.Context;
import android.os.Handler;
import android.os.IGenGpioService;
import android.os.Looper;
import android.os.Message;
import android.os.Process;
import android.util.Log;

// begin WITH_TAINT_TRACKING
import dalvik.system.Taint;
// end WITH_TAINT_TRACKING

public class GenGpioService extends IGenGpioService.Stub {

    private static final String TAG = GenGpioService.class.getSimpleName();
    private Context mContext;
    private int mNativePointer;

    public GenGpioService(Context context) {
        super();
        mContext = context;
        Log.i(TAG, "GenGpioService started");

        mNativePointer = init_native();

        //TODO implement test method
        //Log.i(TAG, "test() returns " + test_native(mNativePointer, 20));
    }

    protected void finalize() throws Throwable {
        finalize_native(mNativePointer);
        super.finalize();
    }

    public String read(int maxLength, int gpio_pin) {
        int length;
        byte[] buffer = new byte[maxLength];

        length = read_native(mNativePointer, buffer, gpio_pin);

        // begin WITH_TAINT_TRACKING
        int tag = Taint.TAINT_GENGPIO;
        String values = new String(buffer, 0, length);
        Taint.addTaintString(values, tag); //taint values is added in place
        Log.d(TAG, "Add Taint in GenGpioService while reading");
        return values;
        // end WITH_TAINT_TRACKING
    }

    public int write(String mString, int gpio_pin) {
        byte[] buffer = mString.getBytes();

        return write_native(mNativePointer, buffer, gpio_pin);
    }

    private static native int init_native();
    private static native void finalize_native(int ptr);
    private static native int read_native(int ptr, byte[] buffer, int gpio_pin);
    private static native int write_native(int ptr, byte[] buffer, int gpio_pin);
//    private static native int test_native(int ptr, int value);
}
