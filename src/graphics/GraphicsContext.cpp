#include "GameEngine/Core/App.hpp"
#include "../core/GELog.hpp"
#include "platform/GLGraphicsContext.hpp"

namespace GameEngine {

    #ifdef PLATFORM_WINDOWS
    extern LRESULT windowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    #endif

    GraphicsContext* GraphicsContext::Create(const GraphicsConfig& cfg) {
        #ifdef RENDERER_OPENGL
        if (cfg.apiType == GraphicsApiType::OPENGL) {
            return GLGraphicsContext::Create(cfg);
        }
        #endif
        return nullptr;
    }

    void* CreateNativeWindow(const WindowConfig& wcfg, void* ptr) {
        auto gc = App::get()->GetGC();
        #ifdef RENDERER_OPENGL
        if (gc->GetConfig().apiType == GraphicsApiType::OPENGL) {
            auto& glgc = *(GLGraphicsContext*)gc;
            #ifdef PLATFORM_LINUX

            auto h = GetX11HNDL(ptr);
            h->rootWindow = RootWindow(xdisplay, glgc.vInfo->screen);
            h->swa.border_pixel = 0;
            h->swa.event_mask = ExposureMask|ButtonPressMask|ButtonReleaseMask|KeyPressMask|KeyReleaseMask|LeaveWindowMask|PointerMotionMask|StructureNotifyMask|FocusChangeMask;
            h->swa.colormap = XCreateColormap(xdisplay, h->rootWindow,
                                            glgc.vInfo->visual, AllocNone );

            h->xwindow = XCreateWindow(xdisplay, h->rootWindow, 0, 0, wcfg.Width, wcfg.Height,
                          0, glgc.vInfo->depth, InputOutput, glgc.vInfo->visual,
                          CWBorderPixel | CWColormap | CWEventMask, &h->swa );

            if (h->index == 0) {
                if (!h->xwindow) { GE_LOGF("Failed to create window"); return nullptr;}
            } else {
                if (!h->xwindow) { GE_LOGE("Failed to create window"); return nullptr;}
            }
            
            XStoreName(xdisplay, h->xwindow, wcfg.Title);

            h->glxWin = glXCreateWindow(xdisplay, glgc.fbConfigs[0], h->xwindow, NULL );

            XClearWindow(xdisplay, h->xwindow);
            XMapWindow(xdisplay, h->xwindow);
            XEvent event;
            XIfEvent(xdisplay, &event, WaitForNotify, (XPointer) h->xwindow );
            
            glXMakeContextCurrent(xdisplay, h->glxWin, h->glxWin, (GLXContext)gc->GetNativeHandle() );
            if (glXSwapIntervalEXT)
                glXSwapIntervalEXT(xdisplay, h->glxWin, gc->GetConfig().vsync);
            else if (glXSwapIntervalMESA)
                glXSwapIntervalMESA(gc->GetConfig().vsync);
            else if (glXSwapIntervalSGI)
                glXSwapIntervalSGI(gc->GetConfig().vsync);

            if (h->index == 0) {
                if (!gladLoaderLoadGL()) {
                    GE_LOGE("Failed to load glad GL");
                    return nullptr;
                }

                GE_LOGD("OpenGL context created.\n\tVersion {}\n\tVendor {}\n\tRenderer {}\n",
                    glGetString(GL_VERSION),
                    glGetString(GL_VENDOR),
                    glGetString(GL_RENDERER));
            }

            glEnable(GL_DEPTH_TEST); 
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            glClearColor( 0.0, 0.0, 0.0, 1.0 );
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glFlush();
            glXSwapBuffers(xdisplay, h->glxWin);

            return h;
            #elif defined(PLATFORM_WINDOWS)
            
            Window* win = (Window*)ptr;
            auto h = GetWin32HNDL(win->GetNativeHandle());
            
            h->hInstance = GetModuleHandle(NULL);

            static const char wincClassName[] = "GameEngine Application";

            WNDCLASSEX wc;
            
            wc.lpszClassName = wincClassName;
            wc.lpfnWndProc = windowProcedure;
            wc.cbSize = sizeof(WNDCLASSEX);
            wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
            wc.cbClsExtra = 0;
            wc.cbWndExtra = 0;
            wc.hInstance = h->hInstance;
            wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            wc.hCursor = LoadCursor(NULL, IDC_ARROW);
            wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // (HBRUSH) CreateSolidBrush(RGB(10, 20, 30)); // 
            wc.lpszMenuName = NULL;
            wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

            int success = RegisterClassEx(&wc);
            
            int pos_left = GetSystemMetrics(SM_CXSCREEN)/2 - wcfg.Width/2;
            int pos_top  = GetSystemMetrics(SM_CYSCREEN)/2 - wcfg.Height/2;

            h->hWnd = CreateWindowA(
                wc.lpszClassName,
                wcfg.Title,
                WS_OVERLAPPEDWINDOW,
                pos_left,
                pos_top,
                wcfg.Width,
                wcfg.Height,
                nullptr,
                nullptr,
                h->hInstance,
                win
            );

            if (h->index == 0) {
                if (!h->hWnd) { GE_LOGF("Failed to create window"); return nullptr;}
            } else {
                if (!h->hWnd) { GE_LOGE("Failed to create window"); return nullptr;}
            }
            
            h->hdc = GetDC(h->hWnd);
            glgc.CreateWin32GC(h->hdc);
            
            if (wglSwapIntervalEXT)
                wglSwapIntervalEXT(gc->GetConfig().vsync);
            
            if (h->index == 0) {
                if (!gladLoaderLoadGL()) {
                    GE_LOGE("Failed to load glad GL");
                    return nullptr;
                }

                GE_LOGD("OpenGL context created.\n\tVersion {}\n\tVendor {}\n\tRenderer {}\n",
                    glGetString(GL_VERSION),
                    glGetString(GL_VENDOR),
                    glGetString(GL_RENDERER));
            }

            glEnable(GL_DEPTH_TEST); 
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            glClearColor( 0.0, 0.0, 0.0, 1.0 );
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glFlush();
            SwapBuffers(h->hdc);

            return h;

            #endif
        }
        #endif
        return nullptr;
    }

    void DestroyNativeWindow(void* ptr) {
        auto gc = App::get()->GetGC();
        #ifdef RENDERER_OPENGL
        if (gc->GetConfig().apiType == GraphicsApiType::OPENGL) {
            auto& glgc = *(GLGraphicsContext*)gc;
            #ifdef PLATFORM_LINUX
            auto h = GetX11HNDL(ptr);
            glXMakeContextCurrent(xdisplay, None, None, NULL);
            glXDestroyWindow(xdisplay, h->glxWin);
            XFreeColormap(xdisplay, h->swa.colormap);
            #elif defined(PLATFORM_WINDOWS)
            auto h = GetWin32HNDL(ptr);
            wglMakeCurrent(NULL, NULL);
            DestroyWindow(h->hWnd);
            if (h->index == -1) PostQuitMessage(0);
            #endif
        }
        #endif
    }
}