#include "../GELog.hpp"
#include "X11HNDL.hpp"
#include "GameEngine/Core/App.hpp"
#include "../EventHandler.hpp"
#include <cstring>

using MyWindow = GameEngine::Window;

namespace GameEngine {
    extern Display* xdisplay;
    extern void* CreateNativeWindow(const WindowConfig&, void*);
    extern void DestroyNativeWindow(void*);
}

MyWindow* MyWindow::Create(const WindowConfig& config){
    auto win = new MyWindow(config);
    auto h = new X11HNDL();
    win->hndl = h;

    h->index = windows.size();
    
    int screen = DefaultScreen(xdisplay);
    /*h->xwindow = XCreateSimpleWindow(xdisplay, RootWindow(xdisplay, screen), 0, 0, 
                    config.Width, config.Height,
                    0, BlackPixel(xdisplay,screen), WhitePixel(xdisplay, screen));*/

    if (CreateNativeWindow(config, h) == nullptr) return nullptr;

    //XSelectInput(xdisplay, h->xwindow, ExposureMask|ButtonPressMask|ButtonReleaseMask|KeyPressMask|KeyReleaseMask|PointerMotionMask|StructureNotifyMask|FocusChangeMask);

    EventHandler::Get()->RegisterWindow(win, win->hndl);

    if (config.Fullscreen) {
        Atom _NET_WM_STATE = XInternAtom(xdisplay, "_NET_WM_STATE", False);
        Atom _NET_WM_STATE_FULLSCREEN = XInternAtom(xdisplay, "_NET_WM_STATE_FULLSCREEN", False);

        XEvent xev;
        memset(&xev, 0, sizeof(xev));

        xev.type = ClientMessage;
        xev.xclient.window = h->xwindow;
        xev.xclient.message_type = _NET_WM_STATE;
        xev.xclient.format = 32;
        xev.xclient.data.l[0] = 1;//h->windowed ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
        xev.xclient.data.l[1] = _NET_WM_STATE_FULLSCREEN;
        xev.xclient.data.l[2] = 0;
        
        XSendEvent (xdisplay, h->rootWindow, False, h->swa.event_mask, &xev);
        XFlush(xdisplay);
    }

    win->renderer = CreateRenderer(App::get()->GetAppConfig().gcConfig.apiType);
    win->renderer->SetViewPort({0,0,config.Width,config.Height});

    windows.push_back(win);

    return win;
}

MyWindow::Window(const WindowConfig& config) : cfg(config) {}

MyWindow::~Window(){
    auto h = GetX11HNDL(hndl);
    delete renderer;
    
    int idx = 0;
    auto iteridx = windows.begin();
    for (; *iteridx != this && iteridx != windows.end(); ++iteridx, ++idx);
    if (iteridx != windows.end()) {
        windows.erase(iteridx);
        for (int i=idx; i<windows.size(); ++i) GetX11HNDL(windows[i]->hndl)->index = i;
    }

    if (windows.size() == 0) {
        App::get()->Exit(0);
    }

    DestroyNativeWindow(hndl);

    delete h;
    hndl = nullptr;
}

void MyWindow::Close(){
    auto h = GetX11HNDL(hndl);
    XUnmapWindow(xdisplay, h->xwindow);
    delete this;
}

void MyWindow::UpdateConfig(const WindowConfig& config){
    
    auto h = GetX11HNDL(hndl);
    if (cfg.Width != config.Width || cfg.Height != config.Height){
        XResizeWindow(xdisplay, h->xwindow, config.Width, config.Height);
    }
    if (cfg.Title != config.Title){
        XStoreName(xdisplay, h->xwindow, config.Title);
    }
    if (cfg.Fullscreen != config.Fullscreen){
        Atom _NET_WM_STATE = XInternAtom(xdisplay, "_NET_WM_STATE", False);
        Atom _NET_WM_STATE_FULLSCREEN = XInternAtom(xdisplay, "_NET_WM_STATE_FULLSCREEN", False);

        XEvent xev;
        memset(&xev, 0, sizeof(xev));

        xev.type = ClientMessage;
        xev.xclient.window = h->xwindow;
        xev.xclient.message_type = _NET_WM_STATE;
        xev.xclient.format = 32;
        xev.xclient.data.l[0] = config.Fullscreen ? 1 : 0;
        xev.xclient.data.l[1] = _NET_WM_STATE_FULLSCREEN;
        xev.xclient.data.l[2] = 0;
        
        XSendEvent (xdisplay, h->xwindow, False, h->swa.event_mask, &xev);
        XFlush(xdisplay);
    }
    cfg = config;
}