#include "pch.hpp"
#include "Platform/Clipboard.hpp"

void ToClipboard(const std::vector<u8>& rgbaData_, s32 width_, s32 height_)
{
    if (rgbaData_.size() != static_cast<s64>(width_ * height_ * 4))
    {
        ERROR_LOG_EX("Buffer size mismatch", "Clipboard", DebugVerbosity::LITTLE_DEBUG);
        return;
    }

    //Reorder the bits from RGBA to BGRA because of Bitmap format
    std::vector<u8> bgra(rgbaData_.size());
    for (s64 i = 0; i < width_ * height_; ++i) {
        bgra[i * 4 + 0] = rgbaData_[i * 4 + 2];
        bgra[i * 4 + 1] = rgbaData_[i * 4 + 1];
        bgra[i * 4 + 2] = rgbaData_[i * 4 + 0];
        bgra[i * 4 + 3] = rgbaData_[i * 4 + 3];
    }

    //Create the bitmap

    HDC hdc = GetDC(nullptr);

    BITMAPINFOHEADER bmi = {};
    bmi.biSize = sizeof(bmi);
    bmi.biWidth = width_;
    bmi.biHeight = -height_;
    bmi.biPlanes = 1;
    bmi.biBitCount = 32;
    bmi.biCompression = BI_RGB;

    HBITMAP hBitmap = CreateDIBitmap(
        hdc,
        &bmi,
        CBM_INIT,
        bgra.data(),
        (BITMAPINFO*)&bmi,
        DIB_RGB_COLORS
    );

    ReleaseDC(nullptr, hdc);

    if (!hBitmap)
    {
        ERROR_LOG_EX("CreateDIBitmap failed", "Clipboard", DebugVerbosity::LITTLE_DEBUG);
        return;
    }


    if (!OpenClipboard(nullptr)) //Open the Clipboard
    {
        DeleteObject(hBitmap);
        ERROR_LOG_EX("OpenClipboard failed", "Clipboard", DebugVerbosity::LITTLE_DEBUG);
        return;
    }

    EmptyClipboard();                     //Clear the Clipboard
    SetClipboardData(CF_BITMAP, hBitmap); //Set the bitmap in the Clipboard
    CloseClipboard();                     //Close the Clipboard
}