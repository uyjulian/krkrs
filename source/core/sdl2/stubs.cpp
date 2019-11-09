
#include "tjsCommHead.h"
#include "StorageIntf.h"
#include "EventIntf.h"
#include "SysInitImpl.h"
#include "WindowImpl.h"
#include "LayerBitmapIntf.h"
#include "LayerBitmapImpl.h"
#include "TickCount.h"
#include "Random.h"
#include "UtilStreams.h"
#include "vkdefine.h"
#include "Platform.h"
#include "Application.h"
#include "ScriptMgnIntf.h"
#include "win32/TVPWindow.h"
#include "VelocityTracker.h"
#include "SystemImpl.h"
#include "RenderManager.h"
#include "VideoOvlIntf.h"
#include "Exception.h"
#include "win32/MenuItemImpl.h"
#include <sys/time.h>
#include <time.h>
#include <sys/stat.h>
#include <SDL.h>

#include <unistd.h>
#include <stdio.h>
#include <limits.h>



#include <wchar.h>

#include <string> 
#include <locale> 
#include <switch.h>


class TVPWindowLayer;

// static TVPWindowManagerOverlay *_windowMgrOverlay = nullptr;
// static TVPConsoleWindow* _consoleWin = nullptr;
static float _touchMoveThresholdSq;
static float _mouseCursorScale;
// static Vec2 _mouseTouchPoint, _mouseBeginPoint;
// static std::set<Touch*> _mouseTouches;
static tTVPMouseButton _mouseBtn;
static int _touchBeginTick;
static bool _virutalMouseMode = false;
static bool _mouseMoved, _mouseClickedDown;
static tjs_uint8 _scancode[0x200];
static tjs_uint16 _keymap[0x200];
static TVPWindowLayer *_lastWindowLayer, *_currentWindowLayer;

bool sdlProcessEvents();
bool sdlProcessEventsForFrames(int frames);

#include <iostream>

SDL_Window *window;
SDL_Renderer* renderer;

class TVPWindowLayer : public iWindowLayer {
protected:
	tTVPMouseCursorState MouseCursorState = mcsVisible;
	tjs_int HintDelay = 500;
	tjs_int ZoomDenom = 1; // Zooming factor denominator (setting)
	tjs_int ZoomNumer = 1; // Zooming factor numerator (setting)
	double TouchScaleThreshold = 5, TouchRotateThreshold = 5;
	

	
	tjs_int ActualZoomDenom; // Zooming factor denominator (actual)
	tjs_int ActualZoomNumer; // Zooming factor numerator (actual)
	// Sprite *DrawSprite = nullptr;
	// Node *PrimaryLayerArea = nullptr;
	int LayerWidth = 0, LayerHeight = 0;
	//iTVPTexture2D *DrawTexture = nullptr;
	TVPWindowLayer *_prevWindow, *_nextWindow;
	friend class TVPWindowManagerOverlay;
	friend class TVPMainScene;
	int _LastMouseX = 0, _LastMouseY = 0;
	std::string _caption;
//	std::map<tTJSNI_BaseVideoOverlay*, Sprite*> _AllOverlay;
	float _drawSpriteScaleX = 1.0f, _drawSpriteScaleY = 1.0f;
	float _drawTextureScaleX = 1.f, _drawTextureScaleY = 1.f;
	bool UseMouseKey = false, MouseLeftButtonEmulatedPushed = false, MouseRightButtonEmulatedPushed = false;
	bool LastMouseMoved = false, Visible = false;
	tjs_uint32 LastMouseKeyTick = 0;
	tjs_int MouseKeyXAccel = 0;
	tjs_int MouseKeyYAccel = 0;
	int LastMouseDownX = 0, LastMouseDownY = 0;
	VelocityTrackers TouchVelocityTracker;
	VelocityTracker MouseVelocityTracker;
	static const int TVP_MOUSE_MAX_ACCEL = 30;
	static const int TVP_MOUSE_SHIFT_ACCEL = 40;
	static const int TVP_TOOLTIP_SHOW_DELAY = 500;
	SDL_Texture* framebuffer;
	// SDL_Texture* framebuffer;
	
	tTJSNI_Window *TJSNativeInstance;
	bool hasDrawn = false;
	bool isBeingDeleted = false;
	int curWindowW = 640, curWindowH = 480;

public:
	TVPWindowLayer(tTJSNI_Window *w)
	{
		_nextWindow = nullptr;
		_prevWindow = _lastWindowLayer;
		_lastWindowLayer = this;
		// ActualZoomDenom = 1;
		// ActualZoomNumer = 1;
		if (_prevWindow) {
			_prevWindow->_nextWindow = this;
		}
		if (!_currentWindowLayer) {
			_currentWindowLayer = this;
		}
		if (w) {
			TJSNativeInstance = w;
		}
		else {
			TJSNativeInstance = nullptr;
		}
		
		
		
		framebuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, curWindowW, curWindowH);
		SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
		// SDL_ShowWindow(window);
	}

	virtual ~TVPWindowLayer() {
		if (_lastWindowLayer == this) _lastWindowLayer = _prevWindow;
		if (_nextWindow) _nextWindow->_prevWindow = _prevWindow;
		if (_prevWindow) {_prevWindow->_nextWindow = _nextWindow;}

		if (_currentWindowLayer == this) {
			// TVPWindowLayer *anotherWin = _lastWindowLayer;
			// while (anotherWin && !anotherWin->isVisible()) {
			// 	anotherWin = anotherWin->_prevWindow;
			// }
			// if (anotherWin && anotherWin->isVisible()) {
			// 	anotherWin->setPosition(0, 0);
			// 	//anotherWin->setVisible(true);
			// }
			_currentWindowLayer = _lastWindowLayer;
			SDL_SetWindowSize(window, _currentWindowLayer->curWindowW, _currentWindowLayer->curWindowH);
		}
		SDL_DestroyTexture(framebuffer);
		framebuffer = NULL;


	}

	bool init() {
		// bool ret = inherit::init();
		// setClippingToBounds(false);
		// DrawSprite = Sprite::create();
		// DrawSprite->setAnchorPoint(Vec2(0, 1)); // top-left
		// PrimaryLayerArea = Node::create();
		// addChild(PrimaryLayerArea);
		// PrimaryLayerArea->addChild(DrawSprite);
		// setAnchorPoint(Size::ZERO);
		// EventListenerMouse *evmouse = EventListenerMouse::create();
		// evmouse->onMouseScroll = std::bind(&TVPWindowLayer::onMouseScroll, this, std::placeholders::_1);
		// evmouse->onMouseDown = std::bind(&TVPWindowLayer::onMouseDownEvent, this, std::placeholders::_1);
		// evmouse->onMouseUp = std::bind(&TVPWindowLayer::onMouseUpEvent, this, std::placeholders::_1);
		// evmouse->onMouseMove = std::bind(&TVPWindowLayer::onMouseMoveEvent, this, std::placeholders::_1);
		// _eventDispatcher->addEventListenerWithSceneGraphPriority(evmouse, this);
		// setTouchEnabled(false);
		// //_touchListener->setSwallowTouches(true);
		// setVisible(false);
		

		return true;
	}

	static TVPWindowLayer *create(tTJSNI_Window *w) {
		TVPWindowLayer *ret = new TVPWindowLayer (w);
		ret->init();
		// ret->autorelease();
		return ret;
	}

	virtual void SetPaintBoxSize(tjs_int w, tjs_int h) override {
		// LayerWidth = w; LayerHeight = h;
		// RecalcPaintBox();
	}
	virtual bool GetFormEnabled() override {
		return SDL_GetWindowFlags(window) & SDL_WINDOW_SHOWN;

	}
	virtual void SetDefaultMouseCursor() override {

	}
	virtual void GetCursorPos(tjs_int &x, tjs_int &y) override {
		SDL_GetMouseState(&x, &y);
		// x = _LastMouseX;
		// y = _LastMouseY;
	}
	virtual void SetCursorPos(tjs_int x, tjs_int y) override {
		SDL_WarpMouseInWindow(window, x, y);
		// _LastMouseX = x;
		// _LastMouseY = y;
	}
	virtual void SetHintText(const ttstr &text) override {

	}
	// tjs_int _textInputPosY;
	virtual void SetAttentionPoint(tjs_int left, tjs_int top, const struct tTVPFont * font) override {
		// _textInputPosY = top;
	}
	virtual void ZoomRectangle(
		tjs_int & left, tjs_int & top,
		tjs_int & right, tjs_int & bottom) override {

	}
	virtual void BringToFront() override {
		// left = tjs_int64(left) * ActualZoomNumer / ActualZoomDenom;
		// top = tjs_int64(top) * ActualZoomNumer / ActualZoomDenom;
		// right = tjs_int64(right) * ActualZoomNumer / ActualZoomDenom;
		// bottom = tjs_int64(bottom) * ActualZoomNumer / ActualZoomDenom;
		if (_currentWindowLayer != this) {
			if (_currentWindowLayer) {
				// const Size &size = _currentWindowLayer->getViewSize();
				// _currentWindowLayer->setPosition(Vec2(size.width, 0));
				_currentWindowLayer->TJSNativeInstance->OnReleaseCapture();
			}
			_currentWindowLayer = this;
		}
		SDL_RaiseWindow(window);
	}
	virtual void ShowWindowAsModal() override {
		in_mode_ = true;
		BringToFront();
		modal_result_ = 0;
		while (this == _currentWindowLayer && !modal_result_) {
			sdlProcessEvents();
			if (::Application->IsTarminate()) {
				modal_result_ = mrCancel;
			} else if (modal_result_ != 0) {
				break;
			}
		}
		in_mode_ = false;
	}
	virtual bool GetVisible() override {
		return SDL_GetWindowFlags(window) & SDL_WINDOW_SHOWN;
	}
	virtual void SetVisible(bool bVisible) override {
		if (bVisible) {
			SDL_ShowWindow(window);
			BringToFront();
		}
		else {
			SDL_HideWindow(window);
			if (_currentWindowLayer == this) {
				_currentWindowLayer = _prevWindow ? _prevWindow : _nextWindow;
			}
		}

	}
	virtual const char *GetCaption() override {
		return SDL_GetWindowTitle(window);

	}
	virtual void SetCaption(const tjs_string & ws) override {
		SDL_SetWindowTitle(window, ttstr(ws).AsNarrowStdString().c_str());
	}
	virtual void SetWidth(tjs_int w) override {
		//int h;
		//SDL_GetWindowSize(window, NULL, &h);
		curWindowW = w;
		SDL_SetWindowSize(window, curWindowW, curWindowH);
		if (framebuffer) {
			SDL_DestroyTexture(framebuffer);
			framebuffer = NULL;
		}
		framebuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, curWindowW, curWindowH);
	}
	virtual void SetHeight(tjs_int h) override {
		//int w;
		//SDL_GetWindowSize(window, &w, NULL);
		curWindowH = h;
		SDL_SetWindowSize(window, curWindowW, curWindowH);
		if (framebuffer) {
			SDL_DestroyTexture(framebuffer);
			framebuffer = NULL;
		}
		framebuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, curWindowW, curWindowH);
	}
	virtual void SetSize(tjs_int w, tjs_int h) override {
		curWindowW = w;
		curWindowH = h;
		SDL_SetWindowSize(window, w, h);
		if (framebuffer) {
			SDL_DestroyTexture(framebuffer);
			framebuffer = NULL;
		}
		framebuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, curWindowW, curWindowH);
	}
	virtual void GetSize(tjs_int &w, tjs_int &h) override {
		w = curWindowW;
		h = curWindowH;
		//SDL_GetWindowSize(window, &w, &h);
	}
	virtual tjs_int GetWidth() const override {
		//int w;
		//SDL_GetWindowSize(window, &w, NULL);
		return curWindowW;
	}
	virtual tjs_int GetHeight() const override {
		//int h;
		//SDL_GetWindowSize(window, NULL, &h);
		return curWindowH;
	}
	virtual void GetWinSize(tjs_int &w, tjs_int &h) override {
		w = curWindowW;
		h = curWindowH;
		//SDL_GetWindowSize(window, &w, &h);
	}
	virtual void SetZoom(tjs_int numer, tjs_int denom) override {
	}
	virtual void UpdateDrawBuffer(iTVPTexture2D *tex) override {
		if (!tex) return;
		//TODO: Update and swap SDL
		// SDL_LockTexture(framebuffer, NULL, NULL, NULL);
		SDL_UpdateTexture(framebuffer, NULL, tex->GetPixelData(), tex->GetPitch());
		// SDL_UnlockTexture(framebuffer);
		SDL_Rect destR;
		destR.x = 0;
		destR.y = 0;
		destR.w = curWindowW;
		destR.h = curWindowH;
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, framebuffer, NULL, &destR);
		SDL_RenderPresent(renderer);
		hasDrawn = true;
	}
#if 0
	virtual void AddOverlay(tTJSNI_BaseVideoOverlay *ovl) = 0;
	virtual void RemoveOverlay(tTJSNI_BaseVideoOverlay *ovl) = 0;
	virtual void UpdateOverlay() = 0;
#endif
	virtual void InvalidateClose() override {
		isBeingDeleted = true;
	}
	virtual bool GetWindowActive() override {
		return _currentWindowLayer == this;
	}
	bool Closing = false, ProgramClosing = false, CanCloseWork = false;
	bool in_mode_ = false; // is modal
	int modal_result_ = 0;
	enum CloseAction {
		caNone,
		caHide,
		caFree,
		caMinimize
	};
	void OnClose(CloseAction& action) {
		if (modal_result_ == 0)
			action = caNone;
		else
			action = caHide;

		if (ProgramClosing) {
			if (TJSNativeInstance) {
				if (TJSNativeInstance->IsMainWindow()) {
					// this is the main window
				} else 			{
					// not the main window
					action = caFree;
				}
				//if (TVPFullScreenedWindow != this) {
					// if this is not a fullscreened window
				//	SetVisible(false);
				//}
				iTJSDispatch2 * obj = TJSNativeInstance->GetOwnerNoAddRef();
				TJSNativeInstance->NotifyWindowClose();
				obj->Invalidate(0, NULL, NULL, obj);
				TJSNativeInstance = NULL;
				SetVisible(false);
				isBeingDeleted = true;
			}
		}
	}
	bool OnCloseQuery() {
		// closing actions are 3 patterns;
		// 1. closing action by the user
		// 2. "close" method
		// 3. object invalidation

		if (TVPGetBreathing()) {
			return false;
		}

		// the default event handler will invalidate this object when an onCloseQuery
		// event reaches the handler.
		if (TJSNativeInstance && (modal_result_ == 0 ||
			modal_result_ == mrCancel/* mrCancel=when close button is pushed in modal window */)) {
			iTJSDispatch2 * obj = TJSNativeInstance->GetOwnerNoAddRef();
			if (obj) {
				tTJSVariant arg[1] = { true };
				static ttstr eventname(TJS_W("onCloseQuery"));

				if (!ProgramClosing) {
					// close action does not happen immediately
					if (TJSNativeInstance) {
						TVPPostInputEvent(new tTVPOnCloseInputEvent(TJSNativeInstance));
					}

					Closing = true; // waiting closing...
				//	TVPSystemControl->NotifyCloseClicked();
					return false;
				} else {
					CanCloseWork = true;
					TVPPostEvent(obj, obj, eventname, 0, TVP_EPT_IMMEDIATE, 1, arg);
					sdlProcessEvents(); // for post event
					// this event happens immediately
					// and does not return until done
					return CanCloseWork; // CanCloseWork is set by the event handler
				}
			} else {
				return true;
			}
		} else {
			return true;
		}
	}
	virtual void Close() override {
		// closing action by "close" method
		if (Closing) return; // already waiting closing...

		ProgramClosing = true;
		try {
			//tTVPWindow::Close();
			if (in_mode_) {
				modal_result_ = mrCancel;
			} 
			else if (OnCloseQuery()) {
				CloseAction action = caFree;
				OnClose(action);
				switch (action) {
				case caNone:
					break;
				case caHide:
					SetVisible(false);
					break;
				case caMinimize:
					//::ShowWindow(GetHandle(), SW_MINIMIZE);
					break;
				case caFree:
				default:
					isBeingDeleted = true;
					//::PostMessage(GetHandle(), TVP_EV_WINDOW_RELEASE, 0, 0);
					break;
				}
			}
		}
		catch (...) {
			ProgramClosing = false;
			throw;
		}
		ProgramClosing = false;
	}
	virtual void OnCloseQueryCalled(bool b) override {
		// closing is allowed by onCloseQuery event handler
		if (!ProgramClosing) {
			// closing action by the user
			if (b) {
				if (in_mode_)
					modal_result_ = 1; // when modal
				else
					SetVisible(false);  // just hide

				Closing = false;
				if (TJSNativeInstance) {
					if (TJSNativeInstance->IsMainWindow()) {
						// this is the main window
						iTJSDispatch2 * obj = TJSNativeInstance->GetOwnerNoAddRef();
						obj->Invalidate(0, NULL, NULL, obj);
						// TJSNativeInstance = NULL; // ¤³¤Î¶ÎëA¤Ç¤Ï¼È¤Ëthis¤¬Ï÷³ý¤µ¤ì¤Æ¤¤¤ë¤¿¤á¡¢\á\ó\Ð©`¤Ø\¢\¯\»\¹¤·¤Æ¤Ï¤¤¤±¤Ê¤¤
					}
				} else {
					delete this;
				}
			} else {
				Closing = false;
			}
		} else {
			// closing action by the program
			CanCloseWork = b;
		}
	}
	virtual void InternalKeyDown(tjs_uint16 key, tjs_uint32 shift) override {
	}
	virtual void OnKeyUp(tjs_uint16 vk, int shift) override {
	}
	virtual void OnKeyPress(tjs_uint16 vk, int repeat, bool prevkeystate, bool convertkey) override {
	}
	tTVPImeMode LastSetImeMode = ::imDisable;
	tTVPImeMode DefaultImeMode = ::imDisable;
	virtual tTVPImeMode GetDefaultImeMode() const override {
		return DefaultImeMode;
	}
	virtual void SetImeMode(tTVPImeMode mode) override {
		switch (mode) {
		case ::imDisable:
		case ::imClose:
// //#ifdef _MSC_VER
// 			TVPMainScene::GetInstance()->detachWithIME();
// #endif
			break;
		case ::imOpen:
			//TVPMainScene::GetInstance()->attachWithIME();
			//break;
		default:
// #ifdef _MSC_VER
// 			TVPMainScene::GetInstance()->attachWithIME();
// #endif
			break;
		}
	}
	virtual void ResetImeMode() override {
	}
	virtual void UpdateWindow(tTVPUpdateType type) override {
		if (TJSNativeInstance) {
			tTVPRect r;
			r.left = 0;
			r.top = 0;
			r.right = curWindowW;
			r.bottom = curWindowH;
			//SDL_GetWindowSize(window, &, &r.bottom);
			TJSNativeInstance->NotifyWindowExposureToLayer(r);
			TVPDeliverWindowUpdateEvents();
		}
	}
	virtual void SetVisibleFromScript(bool b) override {
		SetVisible(b);
	}
	virtual void SetUseMouseKey(bool b) override {
	}
	virtual bool GetUseMouseKey() const override {
		return false;
	}
	virtual void ResetMouseVelocity() override {
	}
	virtual void ResetTouchVelocity(tjs_int id) override {
	}
	virtual bool GetMouseVelocity(float& x, float& y, float& speed) const override {
		return false;
	}
	virtual void TickBeat() override {
	}
	Sint32 mouseLastX = 0;
	Sint32 mouseLastY = 0;
	void sdlRecvEvent(SDL_Event event) {
		if (isBeingDeleted) {
			delete this;
			return;
		}
		if (window && _prevWindow) {
			uint32_t windowID = SDL_GetWindowID(window);
			bool tryParentWindow = false;
			switch (event.type) {
				case SDL_DROPFILE:
				case SDL_DROPTEXT:
				case SDL_DROPBEGIN:
				case SDL_DROPCOMPLETE:
					tryParentWindow = event.drop.windowID != windowID;
					break;
				case SDL_KEYDOWN:
				case SDL_KEYUP:
					tryParentWindow = event.key.windowID != windowID;
					break;
				case SDL_MOUSEMOTION:
					tryParentWindow = event.motion.windowID != windowID;
					break;
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
					tryParentWindow = event.button.windowID != windowID;
					break;
				case SDL_MOUSEWHEEL:
					tryParentWindow = event.wheel.windowID != windowID;
					break;
				case SDL_TEXTEDITING:
					tryParentWindow = event.edit.windowID != windowID;
					break;
				case SDL_TEXTINPUT:
					tryParentWindow = event.text.windowID != windowID;
					break;
			}
			if (tryParentWindow) {
				if (!in_mode_) {
					_prevWindow->sdlRecvEvent(event);
				}
				return;
			}
		}
		if (window && hasDrawn) {
			if (TJSNativeInstance->CanDeliverEvents()) {
				switch (event.type) { 
					case SDL_FINGERDOWN: {
						//int w, h;
						//SDL_GetWindowSize(window, &w, &h);
						mouseLastX = curWindowW * event.tfinger.x;
						mouseLastY = curWindowH * event.tfinger.y;
						TVPPostInputEvent(new tTVPOnMouseDownInputEvent(TJSNativeInstance, curWindowW * event.tfinger.x, curWindowH * event.tfinger.y, tTVPMouseButton::mbLeft, 0));
					}
					case SDL_FINGERUP: {
						//int w, h;
						//SDL_GetWindowSize(window, &w, &h);
						mouseLastX = curWindowW * event.tfinger.x;
						mouseLastY = curWindowH * event.tfinger.y;
						TVPPostInputEvent(new tTVPOnClickInputEvent(TJSNativeInstance, curWindowW * event.tfinger.x, curWindowH * event.tfinger.y));
						TVPPostInputEvent(new tTVPOnMouseUpInputEvent(TJSNativeInstance, curWindowW * event.tfinger.x, curWindowH * event.tfinger.y, tTVPMouseButton::mbLeft, 0));
					}
					case SDL_FINGERMOTION: {
						//int w, h;
						//SDL_GetWindowSize(window, &w, &h);
						mouseLastX = curWindowW * event.tfinger.x;
						mouseLastY = curWindowH * event.tfinger.y;
						TVPPostInputEvent(new tTVPOnMouseMoveInputEvent(TJSNativeInstance, curWindowW * event.tfinger.x, curWindowH * event.tfinger.y, 0));
					}
					case SDL_MOUSEMOTION: {
						mouseLastX = event.motion.x;
						mouseLastY = event.motion.y;
						TVPPostInputEvent(new tTVPOnMouseMoveInputEvent(TJSNativeInstance, event.motion.x, event.motion.y, 0));
						break;
					}
					case SDL_MOUSEBUTTONDOWN: {
						tTVPMouseButton btn;
						bool hasbtn = false;
						switch(event.button.button) {
							case SDL_BUTTON_RIGHT:
								btn = tTVPMouseButton::mbRight;
								hasbtn = true;
								break;
							case SDL_BUTTON_MIDDLE:
								btn = tTVPMouseButton::mbMiddle;
								hasbtn = true;
								break;
							case SDL_BUTTON_LEFT:
								btn = tTVPMouseButton::mbLeft;
								hasbtn = true;
								break;
						}
						if (hasbtn) {
							mouseLastX = event.button.x;
							mouseLastY = event.button.y;
							TVPPostInputEvent(new tTVPOnMouseDownInputEvent(TJSNativeInstance, event.button.x, event.button.y, btn, 0));
						}
						break;
					}
					case SDL_MOUSEBUTTONUP: {
						tTVPMouseButton btn;
						bool hasbtn = false;
						switch(event.button.button) {
							case SDL_BUTTON_RIGHT:
								btn = tTVPMouseButton::mbRight;
								hasbtn = true;
								break;
							case SDL_BUTTON_MIDDLE:
								btn = tTVPMouseButton::mbMiddle;
								hasbtn = true;
								break;
							case SDL_BUTTON_LEFT:
								btn = tTVPMouseButton::mbLeft;
								hasbtn = true;
								break;
						}
						if (hasbtn) {
							mouseLastX = event.button.x;
							mouseLastY = event.button.y;
							TVPPostInputEvent(new tTVPOnClickInputEvent(TJSNativeInstance, event.button.x, event.button.y));
							TVPPostInputEvent(new tTVPOnMouseUpInputEvent(TJSNativeInstance, event.button.x, event.button.y, btn, 0));
						}
						break;
					}
					case SDL_KEYDOWN: {
						TVPPostInputEvent(new tTVPOnKeyDownInputEvent(TJSNativeInstance, event.key.keysym.sym, 0));
						break;
					}
					case SDL_KEYUP: {
						TVPPostInputEvent(new tTVPOnKeyUpInputEvent(TJSNativeInstance, event.key.keysym.sym, 0));
						break;
					}
					case SDL_JOYBUTTONDOWN:
					case SDL_JOYBUTTONUP: { //
						SDL_Keycode ssym = SDLK_UNKNOWN;
						switch(event.jbutton.button) {
							case 0: //A
								ssym = SDLK_RETURN;
								break;
							case 1: //B
								ssym = SDLK_ESCAPE;
								break;
							case 2: //X
								ssym = SDLK_LCTRL;
								break;
							case 3: //Y
								ssym = SDLK_QUOTE;
								break;
							//skip 4
							case 6: //L
								ssym = SDLK_SPACE;
								break;
							case 7: //R
								ssym = SDLK_LSHIFT;
								break;
							case 8: //ZL
								if (event.jbutton.type == SDL_JOYBUTTONDOWN)
									TVPPostInputEvent(new tTVPOnMouseDownInputEvent(TJSNativeInstance, 0, 0, tTVPMouseButton::mbLeft, 0));
								else {
									TVPPostInputEvent(new tTVPOnClickInputEvent(TJSNativeInstance, 0, 0));
									TVPPostInputEvent(new tTVPOnMouseUpInputEvent(TJSNativeInstance, 0, 0, tTVPMouseButton::mbLeft, 0));
								}
								return;
							case 9: //ZR
								if (event.jbutton.type == SDL_JOYBUTTONDOWN)
									TVPPostInputEvent(new tTVPOnMouseDownInputEvent(TJSNativeInstance, 0, 0, tTVPMouseButton::mbRight, 0));
								else {
									TVPPostInputEvent(new tTVPOnClickInputEvent(TJSNativeInstance, 0, 0));
									TVPPostInputEvent(new tTVPOnMouseUpInputEvent(TJSNativeInstance, 0, 0, tTVPMouseButton::mbRight, 0));
								}
								return;
							case 10: //PLUS
								if (event.jbutton.type == SDL_JOYBUTTONDOWN)
									TVPPostInputEvent(new tTVPOnMouseDownInputEvent(TJSNativeInstance, 0, 0, tTVPMouseButton::mbRight, 0));
								else {
									TVPPostInputEvent(new tTVPOnClickInputEvent(TJSNativeInstance, 0, 0));
									TVPPostInputEvent(new tTVPOnMouseUpInputEvent(TJSNativeInstance, 0, 0, tTVPMouseButton::mbRight, 0));
								}
								return;
							case 11: //MINUS
								ssym = SDLK_TAB;
								break;
							case 12: //LEFT
								ssym = SDLK_LEFT;
								break;
							case 13: //UP
								ssym = SDLK_UP;
								break;
							case 14: //RIGHT
								ssym = SDLK_RIGHT;
								break;
							case 15: //DOWN
								ssym = SDLK_DOWN;
								break;
							//skip 16-18
							default:
								ssym = SDLK_UNKNOWN;
								break;
						}
						if (ssym != SDLK_UNKNOWN) {
							if (event.jbutton.type == SDL_JOYBUTTONDOWN)
								TVPPostInputEvent(new tTVPOnKeyDownInputEvent(TJSNativeInstance, ssym, 0));
							else
								TVPPostInputEvent(new tTVPOnKeyUpInputEvent(TJSNativeInstance, ssym, 0));
						}
						break;
					}
					case SDL_MOUSEWHEEL: {
						TVPPostInputEvent(new tTVPOnMouseWheelInputEvent(TJSNativeInstance, 0, event.wheel.y, mouseLastX, mouseLastY));
						break;
					}
					case SDL_WINDOWEVENT_CLOSE:
					case SDL_QUIT: {
						TVPPostInputEvent(new tTVPOnCloseInputEvent(TJSNativeInstance));
					}
				}
			}
		}
	}

	// void SetZoomNumer(tjs_int n) { SetZoom(n, ZoomDenom); }
	// tjs_int GetZoomNumer() const { return ZoomNumer; }
	// void SetZoomDenom(tjs_int d) { SetZoom(ZoomNumer, d); }
	// tjs_int GetZoomDenom() const { return ZoomDenom; }

	// dummy function
	// TODO
};




//TODO: Frame update use tTVPApplication::Run, then iTVPTexture2D::RecycleProcess
//TODO: Rendering: On every frame copy from GetAdapterTexture
//TODO: Startup game at path ::Application->StartApplication(path);

bool sdlProcessEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (_currentWindowLayer) {
			_currentWindowLayer->sdlRecvEvent(event);
		}
	}
	::Application->Run();
	iTVPTexture2D::RecycleProcess();
	return true;
}

bool sdlProcessEventsForFrames(int frames) {
	for (int i = 0; i < frames; ++i) {
		sdlProcessEvents();
	}
}

u32 __nx_applet_exit_mode = 1;
extern int __system_argc;
extern char** __system_argv;

int main(int argc, char **argv) {
	_argc = __system_argc;
	_wargv = new tjs_char*[__system_argc];

	for (int i = 0; i < __system_argc; i += 1) {
		const tjs_char* warg;
		warg = ttstr(__system_argv[i]).c_str();

		tjs_char* warg_copy = new tjs_char[strlen(__system_argv[i]) + 1];
		warg_copy[strlen(__system_argv[i])] = TJS_W('\0');
		memcpy(warg_copy, warg, sizeof(tjs_char) * (strlen(__system_argv[i])));
		_wargv[i] = warg_copy;
	}

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GameController *controller = NULL;
	for (int i = 0; i < SDL_NumJoysticks(); ++i) {
	    if (SDL_IsGameController(i)) {
	        controller = SDL_GameControllerOpen(i);
	        if (controller) {
	            break;
	        }
	    }
	}

    romfsInit();
    socketInitializeDefault();
    nxlinkStdio();

	window = SDL_CreateWindow("Kirikiri for Switch", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, 0);
	renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	::Application = new tTVPApplication();
	::Application->StartApplication( _argc, _wargv );

    Uint32 startTime = 0;
    Uint32 endTime = 0;
    Uint32 delta = 0;
    short timePerFrame = 16; // miliseconds
    
	while (sdlProcessEvents()) {
		if (!startTime) {
            startTime = SDL_GetTicks(); 
        } else {
            delta = endTime - startTime; // how many ms for a frame
        }
        
        if (delta < timePerFrame) {
            SDL_Delay(timePerFrame - delta);
        }
        
        startTime = endTime;
        endTime = SDL_GetTicks();
	}
	SDL_Quit();
	return 0;
}




bool TVPGetKeyMouseAsyncState(tjs_uint keycode, bool getcurrent)
{
	// if (keycode >= sizeof(_scancode) / sizeof(_scancode[0])) return false;
	// tjs_uint8 code = _scancode[keycode];
	// _scancode[keycode] &= 1;
	// return code & (getcurrent ? 1 : 0x10);
	return false;
}



#undef st_atime
#undef st_ctime
#undef st_mtime

//int stat64(const char* __path, struct stat64* __buf) __INTRODUCED_IN(21); // force link it !
bool TVP_stat(const char *name, tTVP_stat &s) {
	struct stat t;
	// static_assert(sizeof(t.st_size) == 4, "");
	// static_assert(sizeof(t.st_size) == 8, "");
	bool ret = !stat(name, &t);
	s.st_mode = t.st_mode;
	s.st_size = t.st_size;
	s.st_atime = t.st_atime;
	s.st_mtime = t.st_mtime;
	s.st_ctime = t.st_ctime;
	return ret;
}


bool TVP_stat(const tjs_char *name, tTVP_stat &s) {
	tTJSNarrowStringHolder holder(name);
	return TVP_stat(holder, s);
}

// tjs_uint32 TVPGetRoughTickCount32()
// {
// 	// tjs_uint32 uptime = 0;
// 	// struct timespec on;
// 	// if (clock_gettime(CLOCK_MONOTONIC, &on) == 0)
// 	// 	uptime = on.tv_sec * 1000 + on.tv_nsec / 1000000;
// 	// return uptime;
// 	return SDL_GetTicks();
// }

bool TVPGetJoyPadAsyncState(tjs_uint keycode, bool getcurrent)
{
	// if (keycode >= sizeof(_scancode) / sizeof(_scancode[0])) return false;
	// tjs_uint8 code = _scancode[keycode];
	// _scancode[keycode] &= 1;
	return false;
}

void TVPExitApplication(int code) {
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
	exit(code);
}

void TVPRelinquishCPU() {
	SDL_Delay(0);
}

#include <string.h>

iWindowLayer *TVPCreateAndAddWindow(tTJSNI_Window *w) {
	TVPWindowLayer* ret = TVPWindowLayer::create(w);
	return ret;
}

void TVPConsoleLog(const ttstr &l, bool important) {
	fprintf(stderr, "%s", l.AsNarrowStdString().c_str());
}

namespace TJS {
	static const int MAX_LOG_LENGTH = 16 * 1024;
	void TVPConsoleLog(const tjs_char *l) {
		fprintf(stderr, "%s", ttstr(l).AsNarrowStdString().c_str());
	}

	void TVPConsoleLog(const tjs_nchar *format, ...) {
		va_list args;
		va_start(args, format);
		char buf[MAX_LOG_LENGTH];
		vsnprintf(buf, MAX_LOG_LENGTH - 3, format, args);
		fprintf(stderr, "%s", buf);
		va_end(args);
	}
}

tjs_uint32 TVPGetCurrentShiftKeyState()
{
	tjs_uint32 f = 0;

	// if (_scancode[VK_SHIFT] & 1) f |= ssShift;
	// if (_scancode[VK_MENU] & 1) f |= ssAlt;
	// if (_scancode[VK_CONTROL] & 1) f |= ssCtrl;
	// if (_scancode[VK_LBUTTON] & 1) f |= ssLeft;
	// if (_scancode[VK_RBUTTON] & 1) f |= ssRight;
	//if (_scancode[VK_MBUTTON] & 1) f |= TVP_SS_MIDDLE;

	return f;
}

ttstr TVPGetPlatformName()
{
	return "Horizon";
}

ttstr TVPGetOSName()
{
	return TVPGetPlatformName();
}

void TVPShowPopMenu(tTJSNI_MenuItem* menu) {
	// TODO: STUB
};


#include <dirent.h>
extern void TVPInitializeFont();

static ttstr TVPDefaultFaceNames;
extern void TVPAddSystemFontToFreeType( const std::string& storage, std::vector<tjs_string>* faces );
extern void TVPGetSystemFontListFromFreeType( std::vector<tjs_string>& faces );
static bool TVPIsGetAllFontList = false;
static ttstr TVPDefaultFontNameX;
void TVPGetAllFontList( std::vector<tjs_string>& list ) {
	TVPInitializeFont();
	if( TVPIsGetAllFontList ) {
		TVPGetSystemFontListFromFreeType( list );
	}
	TVPIsGetAllFontList = true;

#if 0
	DIR* dr;
	if( ( dr = opendir("romfs:/fonts/") ) != nullptr ) {
		fprintf(stderr, "Opened font directory\n");
		struct dirent* entry;
		while( ( entry = readdir( dr ) ) != nullptr ) {
			if( entry->d_type == DT_REG ) {
				std::string path(entry->d_name);
				std::string::size_type extp = path.find_last_of(".");
				if( extp != std::string::npos ) {
					std::string ext = path.substr(extp);
					if( ext == std::string(".ttf") || ext == std::string(".ttc") || ext == std::string(".otf") ) {
						// .ttf | .ttc | .otf
						fprintf(stderr, "Opened %s\n", path.c_str());
						std::string fullpath( std::string("romfs:/fonts/") + path );
						TVPAddSystemFontToFreeType( fullpath, &list );
					}
				}
			}
		}
		closedir( dr );
		TVPIsGetAllFontList = true;
	}
#endif
#if 0
	for( std::list<std::string>::const_iterator i = fontfiles.begin(); i != fontfiles.end(); ++i ) {
		FT_Face face = nullptr;
		std::string fullpath( std::string("/system/fonts/") + *i );
		FT_Open_Args args;
		memset(&args, 0, sizeof(args));
		args.flags = FT_OPEN_PATHNAME;
		args.pathname = fullpath.c_str();
		tjs_uint face_num = 1;
		std::list<std::string> facenames;
		for( tjs_uint f = 0; f < face_num; f++ ) {
			FT_Error err = FT_Open_Face( FreeTypeLibrary, &args, 0, &face);
			if( err == 0 ) {
				facenames.push_back( std::string(face->family_name) );
				std::string(face->style_name);	// スタイル名
				if( face->face_flags & FT_FACE_FLAG_SCALABLE ) {
					// 可変サイズフォントのみ採用
					if( face->num_glyphs > 2965 ) {
						// JIS第一水準漢字以上のグリフ数
						if( face->style_flags & FT_STYLE_FLAG_ITALIC ) {}
						if( face->style_flags & FT_STYLE_FLAG_BOLD ) {}
						face_num = face->num_faces;
						int numcharmap = face->num_charmaps;
						for( int c = 0; c < numcharmap; c++ ) {
							FT_Encoding enc = face->charmaps[c]->encoding;
							if( enc == FT_ENCODING_SJIS ) {
								// mybe japanese
							}
							if( enc == FT_ENCODING_UNICODE ) {
							}
						}
					}
				}
			}
			if(face) FT_Done_Face(face), face = nullptr;
		}
	}
#endif
}
static bool IsInitDefalutFontName = false;
static bool SelectFont( const std::vector<tjs_string>& faces, tjs_string& face ) {
	std::vector<tjs_string> fonts;
	TVPGetAllFontList( fonts );
	for( auto i = faces.begin(); i != faces.end(); ++i ) {
		auto found = std::find( fonts.begin(), fonts.end(), *i );
		if( found != fonts.end() ) {
			face = *i;
			return true;
		}
	}
	return false;
}
const tjs_char *TVPGetDefaultFontName() {
	if( IsInitDefalutFontName ) {
		return TVPDefaultFontNameX.c_str();
	}
	TVPDefaultFontNameX = TJS_W("Meiryo");
	IsInitDefalutFontName =  true;

	// コマンドラインで指定がある場合、そのフォントを使用する
	tTJSVariant opt;
	if(TVPGetCommandLine(TJS_W("-deffont"), &opt)) {
		ttstr str(opt);
		TVPDefaultFontNameX = str;
	} else {
		tjs_string face;
		std::vector<tjs_string> facenames{tjs_string(TJS_W("Meiryo"))};
		if( SelectFont( facenames, face ) ) {
			TVPDefaultFontNameX = face;
		}
	}
	return TVPDefaultFontNameX.c_str();
}
void TVPSetDefaultFontName( const tjs_char * name ) {
	TVPDefaultFontNameX = name;
}
const ttstr &TVPGetDefaultFaceNames() {
	if( !TVPDefaultFaceNames.IsEmpty() ) {
		return TVPDefaultFaceNames;
	} else {
		TVPDefaultFaceNames = ttstr( TVPGetDefaultFontName() );
		TVPDefaultFaceNames += ttstr(TJS_W(",Meiryo"));
		return TVPDefaultFaceNames;
	}
}
