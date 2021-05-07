#include <GameEngine/Core/App.hpp>
#include "../GELog.hpp"
#include "../EventHandler.hpp"
#include "Win32HNDL.hpp"

using namespace GameEngine;

namespace GameEngine {
    extern void* CreateNativeWindow(const WindowConfig&, void*);
    extern void DestroyNativeWindow(void*);
	extern LRESULT windowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
}

Window* Window::Create(const WindowConfig& config){
	auto win = new Window(config);
    auto h = new Win32HNDL();
    win->hndl = h;

    h->index = windows.size();
	if (CreateNativeWindow(config, win) == nullptr) return nullptr;
	EventHandler::Get()->RegisterWindow(win, win->hndl);
    win->renderer = CreateRenderer(App::get()->GetAppConfig().gcConfig.apiType);
    win->renderer->SetViewPort({0,0,config.Width,config.Height});

    windows.push_back(win);

    GetWindowPlacement(h->hWnd, &h->wpc);
    h->HWNDStyle = GetWindowLong(h->hWnd, GWL_STYLE);
    h->HWNDStyleEx = GetWindowLong(h->hWnd, GWL_EXSTYLE);

    if (config.Fullscreen) {
        LONG NewHWNDStyle = h->HWNDStyle;
        NewHWNDStyle &= ~WS_BORDER;
        NewHWNDStyle &= ~WS_DLGFRAME;
        NewHWNDStyle &= ~WS_THICKFRAME;

        LONG NewHWNDStyleEx = h->HWNDStyleEx;
        NewHWNDStyleEx &= ~WS_EX_WINDOWEDGE;

        SetWindowLong(h->hWnd, GWL_STYLE, NewHWNDStyle | WS_POPUP);
        SetWindowLong(h->hWnd, GWL_EXSTYLE, NewHWNDStyleEx | WS_EX_TOPMOST);
        ShowWindow(h->hWnd, SW_SHOWMAXIMIZED);
    } else ShowWindow(h->hWnd, SW_SHOWNORMAL);

	UpdateWindow(h->hWnd);
	
    return win;
}

Window::Window(const WindowConfig& config) : cfg(config) {}

Window::~Window() {
    auto h = GetWin32HNDL(hndl);
    delete renderer;
    
    int idx = 0;
    auto iteridx = windows.begin();
    for (; *iteridx != this && iteridx != windows.end(); ++iteridx, ++idx);
    if (iteridx != windows.end()) {
        windows.erase(iteridx);
        for (int i=idx; i<windows.size(); ++i) GetWin32HNDL(windows[i]->hndl)->index = i;
    }

    if (windows.size() == 0) {
        App::get()->Exit(0);
        h->index = -1;
    }

    DestroyNativeWindow(hndl);

    delete h;
    hndl = nullptr;
}

void Window::Close() {
	//auto h = GetWin32HNDL(hndl);
    delete this;
} 

void Window::UpdateConfig(const WindowConfig& config){
    
    auto h = GetWin32HNDL(hndl);
    if (cfg.Width != config.Width || cfg.Height != config.Height){
        SetWindowPos(h->hWnd, HWND_TOP, 0, 0, config.Width, config.Height, SWP_NOMOVE);
        cfg.Width = config.Width; cfg.Height = config.Height;
    }
    if (cfg.Title != config.Title){
        SetWindowText(h->hWnd, config.Title);
        cfg.Title = config.Title;
    }
    if (cfg.Fullscreen != config.Fullscreen){
        if (config.Fullscreen)
        {
            GetWindowPlacement(h->hWnd, &h->wpc);
            
            LONG NewHWNDStyle = h->HWNDStyle;
            NewHWNDStyle &= ~WS_BORDER;
            NewHWNDStyle &= ~WS_DLGFRAME;
            NewHWNDStyle &= ~WS_THICKFRAME;

            LONG NewHWNDStyleEx = h->HWNDStyleEx;
            NewHWNDStyleEx &= ~WS_EX_WINDOWEDGE;

            SetWindowLong(h->hWnd, GWL_STYLE, NewHWNDStyle | WS_POPUP);
            SetWindowLong(h->hWnd, GWL_EXSTYLE, NewHWNDStyleEx | WS_EX_TOPMOST);
            ShowWindow(h->hWnd, SW_SHOWMAXIMIZED);
        }
        else
        {
            SetWindowLong(h->hWnd, GWL_STYLE, h->HWNDStyle);
            SetWindowLong(h->hWnd, GWL_EXSTYLE, h->HWNDStyleEx);
            ShowWindow(h->hWnd, SW_SHOWNORMAL);
            SetWindowPlacement(h->hWnd, &h->wpc);
        }
        cfg.Fullscreen = config.Fullscreen;
    }
}