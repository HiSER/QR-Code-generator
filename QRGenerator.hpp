#ifndef QRGENERATOR_HPP_
#define QRGENERATOR_HPP_

#include "lib/qrcodegen.h"
#ifdef _MSC_VER
#include "Windows.h"
#endif

class QRGenerator
{
public:

    QRGenerator(const char* text, uint32_t color = 0xFFFFFF, int scale = 1);

    bool isLoaded() const { return qr.ok; }

    int getWidth() { return qr.scale * qrcodegen_getSize(qr.qrcode); }
    int getHeight() { return getWidth(); }

#ifdef _MSC_VER
    void draw(HDC hdc, int x = 0, int y = 0);
#else
    // TODO: other platforms
#endif

#ifdef _MSC_VER
#include <pshpack1.h>
    typedef struct
    {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t alpha;
    } tRGBA;
#include <poppack.h>
#else
    typedef struct __attribute__((aligned(1)))
    {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t alpha;
    } tRGBA;
#endif

private:
   
    typedef struct
    {
        uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
        uint8_t buffer[qrcodegen_BUFFER_LEN_MAX];
        int scale;
        tRGBA color;
        bool ok;
    } tQR;

    tQR qr;
};

#endif /* QRGENERATOR_HPP_ */
