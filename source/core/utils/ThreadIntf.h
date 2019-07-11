//---------------------------------------------------------------------------
/*
	TVP2 ( T Visual Presenter 2 )  A script authoring tool
	Copyright (C) 2000 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
// Thread base class
//---------------------------------------------------------------------------
#ifndef ThreadIntfH
#define ThreadIntfH
#include "tjsNative.h"

#include <functional>
#include <SDL.h>

//---------------------------------------------------------------------------
// tTVPThreadPriority
//---------------------------------------------------------------------------
enum tTVPThreadPriority
{
	ttpIdle, ttpLowest, ttpLower, ttpNormal, ttpHigher, ttpHighest, ttpTimeCritical
};
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// tTVPThread
//---------------------------------------------------------------------------
class tTVPThread
{
protected:
	SDL_Thread* Thread;
	SDL_threadID ThreadId;
private:
	bool Terminated;

	SDL_mutex *Mtx;
	SDL_cond *Cond;
	bool ThreadStarting;

	static int StartProc(void * arg);

public:
	tTVPThread();
	virtual ~tTVPThread();

	bool GetTerminated() const { return Terminated; }
	void SetTerminated(bool s) { Terminated = s; }
	void Terminate() { Terminated = true; }

protected:
	virtual void Execute() {}

public:
	void StartTread();

	void WaitFor() { if (Thread) SDL_WaitThread(Thread, nullptr); Thread = nullptr; }

	SDL_ThreadPriority _Priority;
	tTVPThreadPriority GetPriority();
	void SetPriority(tTVPThreadPriority pri);

	SDL_Thread* GetHandle() const { return Thread; } 	
	SDL_threadID GetThreadId() const { if (ThreadId) return ThreadId; else return SDL_ThreadID(); }  
};
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// tTVPThreadEvent
//---------------------------------------------------------------------------
class tTVPThreadEvent
{
	SDL_mutex *Mtx;
	SDL_cond *Cond;
	bool IsReady;

public:
	tTVPThreadEvent() : IsReady(false) { Mtx = SDL_CreateMutex(); Cond = SDL_CreateCond(); }
	virtual ~tTVPThreadEvent() { SDL_DestroyCond(Cond); SDL_DestroyMutex(Mtx); }

	void Set() {
		SDL_LockMutex(Mtx);
		IsReady = true;
		SDL_CondBroadcast(Cond);
		SDL_UnlockMutex(Mtx);
	}

	bool WaitFor( tjs_uint timeout ) {
		SDL_LockMutex(Mtx);
		if( timeout == 0 ) {
			while (!IsReady) {
				SDL_CondWait(Cond, Mtx);
			}
			IsReady = false;
			SDL_UnlockMutex(Mtx);
			return true;
		} else {
			bool result = false;
			while (!IsReady) {
				int tmResult = SDL_CondWaitTimeout(Cond, Mtx, timeout);
				if (tmResult == SDL_MUTEX_TIMEDOUT) {
					result = IsReady;
					break;
				}
			}
			IsReady = false;
			SDL_UnlockMutex(Mtx);
			return result;
		}
	}
};
//---------------------------------------------------------------------------


/*[*/
const tjs_int TVPMaxThreadNum = 8;
#if 0
typedef void (TJS_USERENTRY *TVP_THREAD_TASK_FUNC)(void *);
typedef void * TVP_THREAD_PARAM;
#endif
typedef const std::function<void(int)> &TVP_THREAD_TASK_FUNC;
/*]*/

TJS_EXP_FUNC_DEF(tjs_int, TVPGetProcessorNum, ());
TJS_EXP_FUNC_DEF(tjs_int, TVPGetThreadNum, ());
TJS_EXP_FUNC_DEF(void, TVPBeginThreadTask, (tjs_int num));
#if 0
TJS_EXP_FUNC_DEF(void, TVPExecThreadTask, (TVP_THREAD_TASK_FUNC func, TVP_THREAD_PARAM param));
#endif
TJS_EXP_FUNC_DEF(void, TVPExecThreadTask, (int numThreads, TVP_THREAD_TASK_FUNC func));
TJS_EXP_FUNC_DEF(void, TVPEndThreadTask, ());

#endif
