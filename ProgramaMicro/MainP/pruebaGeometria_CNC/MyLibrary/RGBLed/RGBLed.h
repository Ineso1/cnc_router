#ifndef RGBLED_H
#define RGBLED_H
#include <MKL25Z4.h>


class RGBLed{
    public:
        RGBLed();
        void RgbToogleBlue();
        void RgbToogleRed();
        void RgbToogleGreen();
        void RgbOff();
        void RgbConfig();

    private:

};

#endif // RGBLED_H
