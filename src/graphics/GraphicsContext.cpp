#include "GameEngine/Core/App.hpp"
#include "../core/GELog.hpp"
#include "platform/GLGraphicsContext.hpp"

namespace GameEngine {
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
            h->swa.event_mask = ExposureMask|ButtonPressMask|ButtonReleaseMask|KeyPressMask|KeyReleaseMask|PointerMotionMask|StructureNotifyMask|FocusChangeMask;
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
            glClearColor( 0.0, 0.0, 0.0, 1.0 );
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glFlush();
            glXSwapBuffers(xdisplay, h->glxWin);

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
            #endif
        }
        #endif
    }
}