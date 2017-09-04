package android.os;

import android.os.IGenGpioService;

public class GenGpioManager {
    IGenGpioService mService;

    public GenGpioManager(IGenGpioService service) {
        this.mService = service;
    }

    public String read(int maxLength, int gpio_pin) {
        try {
            return mService.read(maxLength, gpio_pin);
        } catch (RemoteException e) {
            return null;
        }
    }

    public int write(String mString, int gpio_pin) {
    try {
            return mService.write(mString, gpio_pin);
        } catch (RemoteException e) {
            return 0;
        }
    }
}
