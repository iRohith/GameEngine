#ifdef RENDERER_OPENGL

#ifdef PLATFORM_LINUX

/*
* Check vsync... currently double buffered has less FPS due to syncronozation on first GL call
*                   after glXSwapBuffers
* vsync not effecting single buffered
*/

#include "../../core/platform/X11HNDL.hpp"
#include <X11/XKBlib.h>

namespace GameEngine {
    extern Display* xdisplay;
    static Bool WaitForNotify( Display *dpy, XEvent *event, XPointer arg ) {
        return (event->type == MapNotify) && (event->xmap.window == (XWindow) arg);
    }

    static const int singleBufferAttributess[] = {
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE,   GLX_RGBA_BIT,
        GLX_DEPTH_SIZE,    24,
        GLX_DOUBLEBUFFER,  False,
        GLX_RED_SIZE,      1,
        GLX_GREEN_SIZE,    1,
        GLX_BLUE_SIZE,     1,
        None
    };

    static const int doubleBufferAttributes[] = {
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE,   GLX_RGBA_BIT,
        GLX_DEPTH_SIZE,    24,
        GLX_DOUBLEBUFFER,  True,
        GLX_RED_SIZE,      1,
        GLX_GREEN_SIZE,    1, 
        GLX_BLUE_SIZE,     1,
        None
    };

    static const int context_attribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        GLX_CONTEXT_MINOR_VERSION_ARB, 3,
        None
    };

    struct GLGraphicsContext : public GraphicsContext {
        
        XVisualInfo          *vInfo;
        GLXFBConfig          *fbConfigs;
        
        static GLGraphicsContext* Create(const GraphicsConfig& cfg){
            XVisualInfo          *vInfo;
            GLXFBConfig          *fbConfigs;
            GLXContext            context;
            int                   numReturned;
            bool doubleBuffered;
            
            xdisplay = XOpenDisplay((char *)0);
            auto done = XkbSetDetectableAutoRepeat(xdisplay, true, NULL);
            
            if (xdisplay == nullptr) {
                GE_LOGF("Failed to open X11 display");
                return nullptr;
            }

            int screen = DefaultScreen(xdisplay);
            if (!gladLoaderLoadGLX(xdisplay, screen)) {
                GE_LOGF("Glad GLX load failed");
                return nullptr;
            }

            if (cfg.doubleBuffered) {
                fbConfigs = glXChooseFBConfig(xdisplay, screen,
                                   doubleBufferAttributes, &numReturned );
                if ( fbConfigs == NULL ) {
                    GE_LOGW("Double buffer context not available. Falling back to single buffered");
                    fbConfigs = glXChooseFBConfig(xdisplay, screen,
                                    singleBufferAttributess, &numReturned );
                    doubleBuffered = false;
                } else doubleBuffered = true;
            } else {
                fbConfigs = glXChooseFBConfig(xdisplay, screen,
                                    singleBufferAttributess, &numReturned );
                doubleBuffered = false;
            }

            if ( fbConfigs == NULL ) {
                GE_LOGF("No GLX Framebuffer config found");
                return nullptr;
            }

            vInfo = glXGetVisualFromFBConfig(xdisplay, fbConfigs[0] );
            context = glXCreateContextAttribsARB(xdisplay, fbConfigs[0], NULL, true, context_attribs);

            if (!context) {
                GE_LOGE("Failed to create OpenGL context");
                return nullptr;
            }

            auto gc = new GLGraphicsContext();
            gc->cfg.doubleBuffered = doubleBuffered;
            gc->hndl = context;
            gc->fbConfigs = fbConfigs;
            gc->vInfo = vInfo;
            return gc;
        }

        ~GLGraphicsContext(){
            glFinish();
            // Seg fault in Ubuntu 20.10 not in Arch/Manjaro
            //glXDestroyContext(xdisplay, (GLXContext)hndl);
            XFree(vInfo);
            XFree(fbConfigs);
            XCloseDisplay(xdisplay);
            gladLoaderUnloadGLX();
        }

        void SwapBuffers(GameEngine::Window* w) {
            if (!cfg.doubleBuffered) { glFlush(); return;}
            if (w == nullptr) w = App::get()->GetDefaultWindow();
            auto h = GetX11HNDL((void*)(*(intptr_t*)(w)));
            glXSwapBuffers (xdisplay, h->glxWin);
        }
    };
}

#elif defined(PLATFORM_WINDOWS)

#include "../../core/platform/Win32HNDL.hpp"
#include <glad/wgl.h>
#include <glad/gl.h>

namespace GameEngine {
    struct GLGraphicsContext : public GraphicsContext {
        static constexpr const int attributes[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 3,   // Set the MAJOR version of OpenGL to 3
            WGL_CONTEXT_MINOR_VERSION_ARB, 3,   // Set the MINOR version of OpenGL to 2
            WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, // Set our OpenGL context to be forward compatible
            0
        };

        static constexpr const PIXELFORMATDESCRIPTOR singleBufferPfd = any(PIXELFORMATDESCRIPTOR())._apply_(it, 
            it.nSize = sizeof(PIXELFORMATDESCRIPTOR);
            it.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
            it.iPixelType = PFD_TYPE_RGBA;
            it.cColorBits = 32;
            it.cDepthBits = 24;
            it.iLayerType = PFD_MAIN_PLANE;
        )();
        
        static constexpr const PIXELFORMATDESCRIPTOR doubleBufferPfd = any(PIXELFORMATDESCRIPTOR())._apply_(it, 
            it.nSize = sizeof(PIXELFORMATDESCRIPTOR);
            it.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
            it.iPixelType = PFD_TYPE_RGBA;
            it.cColorBits = 32;
            it.cDepthBits = 24;
            it.iLayerType = PFD_MAIN_PLANE;
        )();

        HDC hdc;
        HGLRC temp_context;
        
        static GLGraphicsContext* Create(const GraphicsConfig& cfg){
            HDC hdc = GetDC(NULL);
            //HDC hdc = CreateCompatibleDC(shdc);
            if (hdc == NULL) {
                GE_LOGF("Failed to get device context");
                return nullptr;
            }

            int format = 0;
            
            auto pfd = &doubleBufferPfd;
            if (!cfg.doubleBuffered || (format = ChoosePixelFormat(hdc, &doubleBufferPfd)) == 0){
                format = ChoosePixelFormat(hdc, &singleBufferPfd);
                pfd = &singleBufferPfd;
            }

            if (format == 0 || SetPixelFormat(hdc, format, pfd) == FALSE) {
                ReleaseDC(NULL, hdc);
                GE_LOGE("Failed to set a compatible pixel format!");
                return nullptr;
            }
            // Create and enable a temporary (helper) opengl context:
            HGLRC temp_context;
            if (NULL == (temp_context = wglCreateContext(hdc))) {
                ReleaseDC(NULL, hdc);
                GE_LOGE("Failed to create the rendering context!");
                return nullptr;
            }
            wglMakeCurrent(hdc, temp_context);

            // Load WGL Extensions:
            if (!gladLoaderLoadWGL(hdc)) {
                wglMakeCurrent(NULL, NULL);
                wglDeleteContext(temp_context);
                ReleaseDC(NULL, hdc);
                GE_LOGE("Glad WGL Loader failed!");
                return nullptr;
            }

            if (!gladLoaderLoadGL()) {
                wglMakeCurrent(NULL, NULL);
                wglDeleteContext(temp_context);
                ReleaseDC(NULL, hdc);
                GE_LOGE("Glad Loader failed!");
                return nullptr;
            }

            auto gc = new GLGraphicsContext();
            gc->cfg.doubleBuffered = pfd == &doubleBufferPfd;
            gc->hdc = hdc;
            return gc;
        }

        void CreateWin32GC(const HDC& hdc){
            HGLRC opengl_context = NULL;

            auto pfd = &doubleBufferPfd;
            int format = 0;
            if (!cfg.doubleBuffered || (format = ChoosePixelFormat(hdc, &doubleBufferPfd)) == 0){
                format = ChoosePixelFormat(hdc, &singleBufferPfd);
                pfd = &singleBufferPfd;
            }

            if (format == 0 || SetPixelFormat(hdc, format, pfd) == FALSE) {
                ReleaseDC(NULL, hdc);
                GE_LOGE("Failed to set a compatible pixel format!");
            }
            
            if (NULL == (opengl_context = wglCreateContextAttribsARB(hdc, NULL, attributes))) {
                wglDeleteContext(temp_context);
                ReleaseDC(NULL, hdc);
                GE_LOGE("Failed to create the final rendering context!");
            }

            hndl = opengl_context;
            wglMakeCurrent(hdc, opengl_context);    // Make our OpenGL 3.3 context current
        }

        ~GLGraphicsContext(){
            glFinish();
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(temp_context);
            wglDeleteContext((HGLRC)hndl);
            ReleaseDC(NULL, hdc);
        }

        void SwapBuffers(GameEngine::Window* w) {
            if (!cfg.doubleBuffered) { glFlush(); return;}
            if (w == nullptr) w = App::get()->GetDefaultWindow();
            auto h = GetWin32HNDL(w->GetNativeHandle());
            ::SwapBuffers(h->hdc);
        }
    };
}

#endif

#endif