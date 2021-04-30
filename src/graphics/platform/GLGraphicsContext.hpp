#ifdef RENDERER_OPENGL

/*
* Check vsync... currently double buffered has less FPS due to syncronozation on first GL call
*                   after glXSwapBuffers
* vsync not effecting single buffered
*/

#ifdef PLATFORM_LINUX

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

#endif

#endif