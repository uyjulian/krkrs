//---------------------------------------------------------------------------
/*
	TVP2 ( T Visual Presenter 2 )  A script authoring tool
	Copyright (C) 2000 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
// Thread base class
//---------------------------------------------------------------------------
#define NOMINMAX
#include "tjsCommHead.h"

#include "ThreadIntf.h"
#include "MsgIntf.h"

#include <SDL.h>


//---------------------------------------------------------------------------
// tTVPThread : a wrapper class for thread
//---------------------------------------------------------------------------
tTVPThread::tTVPThread()
 : Thread(nullptr), Terminated(false), ThreadStarting(false)
{
	Mtx = SDL_CreateMutex();
	Cond = SDL_CreateCond();
}
//---------------------------------------------------------------------------
tTVPThread::~tTVPThread()
{
	if( Thread != nullptr ) {
		SDL_DetachThread(Thread);
		Thread = nullptr;
	}
	SDL_DestroyCond(Cond);
	SDL_DestroyMutex(Mtx);
}
//---------------------------------------------------------------------------
int tTVPThread::StartProc(void * arg)
{
	// スレッドが開始されたのでフラグON
	tTVPThread* _this = (tTVPThread*)arg;
	_this->ThreadId = SDL_ThreadID();
	SDL_LockMutex(_this->Mtx);
	_this->ThreadStarting = true;
	SDL_CondBroadcast(_this->Cond);
	SDL_UnlockMutex(_this->Mtx);
	(_this)->Execute();

	// Execute();
	return 0;
}
//---------------------------------------------------------------------------
void tTVPThread::StartTread()
{
	if( Thread == nullptr ) {
		Thread = SDL_CreateThread(tTVPThread::StartProc, "tTVPThread", this);
		if (Thread == nullptr) {
			TVPThrowInternalError;
		}
		SDL_LockMutex(Mtx);
		while (!ThreadStarting) {
        	SDL_CondWait(Cond, Mtx);
    	}
    	SDL_UnlockMutex(Mtx);
	}
}
//---------------------------------------------------------------------------
tTVPThreadPriority tTVPThread::GetPriority()
{
	switch(_Priority)
	{
	case SDL_THREAD_PRIORITY_LOW:			return ttpIdle;
	case SDL_THREAD_PRIORITY_NORMAL:		return ttpNormal;
	case SDL_THREAD_PRIORITY_HIGH:			return ttpTimeCritical;
	}

	return ttpNormal;
}
//---------------------------------------------------------------------------
void tTVPThread::SetPriority(tTVPThreadPriority pri)
{
	SDL_ThreadPriority npri = SDL_THREAD_PRIORITY_NORMAL;
	switch(pri)
	{
	case ttpIdle:			npri = SDL_THREAD_PRIORITY_LOW;			break;
	case ttpLowest:			npri = SDL_THREAD_PRIORITY_LOW;			break;
	case ttpLower:			npri = SDL_THREAD_PRIORITY_LOW;			break;
	case ttpNormal:			npri = SDL_THREAD_PRIORITY_NORMAL;		break;
	case ttpHigher:			npri = SDL_THREAD_PRIORITY_NORMAL;		break;
	case ttpHighest:		npri = SDL_THREAD_PRIORITY_NORMAL;		break;
	case ttpTimeCritical:	npri = SDL_THREAD_PRIORITY_HIGH;		break;
	}
	if(SDL_SetThreadPriority(npri) < 0)
	{
		printf("SetThreadPriority error:%s\n",SDL_GetError());
	}
	_Priority = npri;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
tjs_int TVPDrawThreadNum = 1;
//---------------------------------------------------------------------------
tjs_int TVPGetProcessorNum( void )
{
	return SDL_GetCPUCount();
}
//---------------------------------------------------------------------------
tjs_int TVPGetThreadNum( void )
{
	tjs_int threadNum = TVPDrawThreadNum ? TVPDrawThreadNum : TVPGetProcessorNum();
	threadNum = std::min( threadNum, TVPMaxThreadNum );
	return threadNum;
}
//---------------------------------------------------------------------------
static void TJS_USERENTRY DummyThreadTask( void * ) {}
//---------------------------------------------------------------------------
#if 0
class DrawThreadPool;
class DrawThread : public tTVPThread {
	std::mutex mtx;
	std::condition_variable cv;
	TVP_THREAD_TASK_FUNC  lpStartAddress;
	TVP_THREAD_PARAM lpParameter;
	DrawThreadPool* parent;
protected:
	virtual void Execute();

public:
	DrawThread( DrawThreadPool* p ) : parent( p ), lpStartAddress( nullptr ), lpParameter( nullptr ) {}
	void SetTask( TVP_THREAD_TASK_FUNC func, TVP_THREAD_PARAM param ) {
		std::lock_guard<std::mutex> lock( mtx );
		lpStartAddress = func;
		lpParameter = param;
		cv.notify_one();
	}
};
//---------------------------------------------------------------------------
class DrawThreadPool {
	std::vector<DrawThread*> workers;
#ifdef _WIN32
	std::vector<tjs_int> processor_ids;
#endif
	std::atomic<int> running_thread_count;
	tjs_int task_num;
	tjs_int task_count;
private:
	void PoolThread( tjs_int taskNum );

public:
	DrawThreadPool() : running_thread_count( 0 ), task_num( 0 ), task_count( 0 ) {}
	~DrawThreadPool() {
		for( auto i = workers.begin(); i != workers.end(); ++i ) {
			DrawThread *th = *i;
			th->Terminate();
			th->SetTask( DummyThreadTask, nullptr );
			th->WaitFor();
			delete th;
		}
	}
	inline void DecCount() { running_thread_count--; }
	void BeginTask( tjs_int taskNum ) {
		task_num = taskNum;
		task_count = 0;
		PoolThread( taskNum );
	}
	void ExecTask( TVP_THREAD_TASK_FUNC func, TVP_THREAD_PARAM param ) {
		if( task_count >= task_num - 1 ) {
			func( param );
			return;
		}
		running_thread_count++;
		DrawThread* thread = workers[task_count];
		task_count++;
		thread->SetTask( func, param );
		std::this_thread::yield();
	}
	void WaitForTask() {
		int expected = 0;
		while( false == std::atomic_compare_exchange_weak( &running_thread_count, &expected, 0 ) ) {
			expected = 0;
			std::this_thread::yield();	// スレッド切り替え(再スケジューリング)
		}
	}
};
//---------------------------------------------------------------------------
void DrawThread::Execute() {
	while( !GetTerminated() ) {
		{
			std::unique_lock<std::mutex> uniq_lk( mtx );
			cv.wait( uniq_lk, [this] { return lpStartAddress != nullptr; } );
		}
		if( lpStartAddress != nullptr ) ( lpStartAddress )( lpParameter );
		lpStartAddress = nullptr;
		parent->DecCount();
	}
}
//---------------------------------------------------------------------------
void DrawThreadPool::PoolThread( tjs_int taskNum ) {
	tjs_int extraThreadNum = TVPGetThreadNum() - 1;

#ifdef _WIN32
	if( processor_ids.empty() ) {
#ifndef TJS_64BIT_OS
		DWORD processAffinityMask, systemAffinityMask;
		::GetProcessAffinityMask( GetCurrentProcess(), &processAffinityMask, &systemAffinityMask );
		for( tjs_int i = 0; i < MAXIMUM_PROCESSORS; i++ ) {
			if( processAffinityMask & ( 1 << i ) )
				processor_ids.push_back( i );
		}
#else
		ULONGLONG processAffinityMask, systemAffinityMask;
		::GetProcessAffinityMask( GetCurrentProcess(), (PDWORD_PTR)&processAffinityMask, (PDWORD_PTR)&systemAffinityMask );
		for( tjs_int i = 0; i < MAXIMUM_PROCESSORS; i++ ) {
			if( processAffinityMask & ( 1ULL << i ) )
				processor_ids.push_back( i );
		}
#endif
		if( processor_ids.empty() )
			processor_ids.push_back( MAXIMUM_PROCESSORS );
	}
#endif

	// スレッド数がextraThreadNumに達していないので(suspend状態で)生成する
	while( (tjs_int)workers.size() < extraThreadNum ) {
		DrawThread* th = new DrawThread( this );
		th->StartTread();
#ifdef _WIN32
		::SetThreadIdealProcessor( th->GetHandle(), processor_ids[workers.size() % processor_ids.size()] );
#elif !defined( ANDROID )
		// for pthread(!android)
		cpu_set_t cpuset;
		CPU_ZERO( &cpuset );
		CPU_SET( workers.size(), &cpuset );
		int rc = pthread_setaffinity_np( th->GetHandle(), sizeof( cpu_set_t ), &cpuset );
#endif
		workers.push_back( th );
	}
	// スレッド数が多い場合終了させる
	while( (tjs_int)workers.size() > extraThreadNum ) {
		DrawThread *th = workers.back();
		th->Terminate();
		running_thread_count++;
		th->SetTask( DummyThreadTask, nullptr );
		th->WaitFor();
		workers.pop_back();
		delete th;
	}
}
//---------------------------------------------------------------------------
static DrawThreadPool TVPTheadPool;
//---------------------------------------------------------------------------
void TVPBeginThreadTask( tjs_int taskNum ) {
	TVPTheadPool.BeginTask( taskNum );
}
//---------------------------------------------------------------------------
void TVPExecThreadTask( TVP_THREAD_TASK_FUNC func, TVP_THREAD_PARAM param ) {
	TVPTheadPool.ExecTask( func, param );
}
//---------------------------------------------------------------------------
void TVPEndThreadTask( void ) {
	TVPTheadPool.WaitForTask();
}
#endif
//---------------------------------------------------------------------------
void TVPExecThreadTask(int numThreads, TVP_THREAD_TASK_FUNC func)
{
  if (numThreads == 1) {
    func(0);
    return;
  }
#pragma omp parallel for schedule(static)
  for (int i = 0; i < numThreads; ++i)
	  func(i);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


