package android.os;
interface IGenGpioService {
    /**
    * {@hide}
    */
    String read(int maxLength, int gpio_pin);
    int write(String mString, int gpio_pin);
}
