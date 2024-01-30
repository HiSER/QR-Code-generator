#include "QRGenerator.hpp"

QRGenerator::QRGenerator(const char* text, uint32_t color, int scale)
{
    memcpy(&qr.color, &color, sizeof(tRGBA));
    qr.scale = scale < 1 ? 1 : scale > 20 ? 20 : scale;
    qr.ok = qrcodegen_encodeText(text, qr.buffer, qr.qrcode, qrcodegen_Ecc_MEDIUM, qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
    if (!qr.ok)
    {
        TRACE_ERROR("QRGenerator '%s' failure", text);
    }
}

void QRGenerator::draw(HDC hdc, int x, int y)
{
    tRGBA* bmp_bits;
    HDC dc = CreateCompatibleDC(hdc);
    BITMAPINFO bmp_info;
    memset(&bmp_info, 0, sizeof(bmp_info));
    bmp_info.bmiHeader.biSize = sizeof(bmp_info.bmiHeader);
    bmp_info.bmiHeader.biWidth = getWidth();
    bmp_info.bmiHeader.biHeight = getHeight();
    bmp_info.bmiHeader.biPlanes = 1;
    bmp_info.bmiHeader.biBitCount = 32;
    bmp_info.bmiHeader.biCompression = BI_RGB;
    HBITMAP bmp = CreateDIBSection(dc, &bmp_info, DIB_RGB_COLORS, (void**)&bmp_bits, NULL, 0);
    HGDIOBJ old_bmp = SelectObject(dc, bmp);
    BitBlt(dc, 0, 0, getWidth(), getHeight(), hdc, x, y, SRCCOPY);
    tRGBA* pOUT;
    int oY, bX, bY;
    int bW = getWidth();
    int bH = getHeight();
    for (bY = 0; bY < bH; bY++)
    {
        oY = bW * (bH - bY - 1);
        for (bX = 0; bX < bW; bX++)
        {
            pOUT = &bmp_bits[oY + bX];
            if (qrcodegen_getModule(qr.qrcode, bX / qr.scale, bY / qr.scale))
            {
                pOUT->blue = qr.color.blue;
                pOUT->green = qr.color.green;
                pOUT->red = qr.color.red;
            }
            pOUT->alpha = 0xFF;
        }
    }
    BitBlt(hdc, x, y, getWidth(), getHeight(), dc, 0, 0, SRCCOPY);
    SelectObject(dc, old_bmp);
    DeleteObject(bmp);
    DeleteObject(dc);
}
