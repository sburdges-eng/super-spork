#pragma once

// Simplified REAPER FX Embed Header

enum
{
    REAPER_FXEMBED_WM_IS_SUPPORTED = 0x1000,
    REAPER_FXEMBED_WM_CREATE,
    REAPER_FXEMBED_WM_DESTROY,
    REAPER_FXEMBED_WM_GETMINMAXINFO,
    REAPER_FXEMBED_WM_PAINT,
    REAPER_FXEMBED_WM_SETCURSOR,
    REAPER_FXEMBED_WM_MOUSEMOVE,
    REAPER_FXEMBED_WM_LBUTTONDOWN,
    REAPER_FXEMBED_WM_LBUTTONUP,
    REAPER_FXEMBED_WM_LBUTTONDBLCLK,
    REAPER_FXEMBED_WM_RBUTTONDOWN,
    REAPER_FXEMBED_WM_RBUTTONUP,
    REAPER_FXEMBED_WM_RBUTTONDBLCLK,
    REAPER_FXEMBED_WM_MOUSEWHEEL,
};

namespace reaper
{
    struct REAPER_FXEMBED_IBitmap
    {
        virtual int getWidth() = 0;
        virtual int getHeight() = 0;
        virtual int getRowSpan() = 0;
        virtual unsigned char* getBits() = 0;
    };

    struct REAPER_FXEMBED_DrawInfo
    {
        int x, y, w, h;
    };

    struct REAPER_FXEMBED_SizeHints
    {
        int min_width, min_height;
        int max_width, max_height;
        int preferred_aspect;
        int minimum_aspect;
    };
}

constexpr int __effEditDrawDeprecated = 16;
