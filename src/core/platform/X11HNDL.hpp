#pragma once 

#ifdef RENDERER_OPENGL
#include <glad/glx.h>
#else
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif

using XWindow = Window;

struct X11HNDL {
    XWindow xwindow;
    int index;

    #ifdef RENDERER_OPENGL
    XWindow rootWindow;
    GLXWindow glxWin;
    XSetWindowAttributes  swa;
    #endif
};

inline X11HNDL* GetX11HNDL(void* ptr) { return ptr == nullptr ? nullptr : (X11HNDL*)ptr; }