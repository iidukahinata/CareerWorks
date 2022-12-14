/**
* @file  Common.h
* @brief 共通的な処理をまとめている
*
* @date	 2022/08/02 2022年度初版
*/
#pragma once


#ifdef _DEBUG
#ifdef _M_IX86 || _M_IX86_FP
#define DEBUG_BREAK _asm { int 3 }
#else
#define DEBUG_BREAK __debugbreak()
#endif // _M_IX86 || _M_IX86_FP
#else
#define DEBUG_BREAK
#endif

#define INLINE inline
#define FORCEINLINE __forceinline

#define ALIGN(N) __declspec(align(N))

#ifdef _DEBUG
#define LOG(text)		DebugLog::Log(text)
#define LOG_ERROR(text)	DebugLog::Error(text, __FILE__, __LINE__)

#define ASSERT(expr) \
	if (expr) {}		 \
	else {			     \
		LOG_ERROR(#expr);\
		DEBUG_BREAK;	 \
	}
#else
#define LOG(text)
#define LOG_ERROR(text)

#define ASSERT(expr)
#endif // _DEBUG

#define SAFE_DELETE(ptr) \
if(ptr)					 \
{						 \
	delete ptr;			 \
	ptr = nullptr;		 \
}

enum ThreadType
{
	MainThread,

	RenderingThread,

	TaskThread,

	LodingThread,

	MaxUseThread,
};