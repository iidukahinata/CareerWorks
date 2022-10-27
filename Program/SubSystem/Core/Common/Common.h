/**
* @file  Common.h
* @brief ã§í ìIÇ»èàóùÇÇ‹Ç∆ÇﬂÇƒÇ¢ÇÈ
*
* @date	 2022/08/02 2022îNìxèâî≈
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
#define LOG(text)		DebugLog::Get().Log(text)
#define LOG_ERROR(text)	DebugLog::Get().Error(text, __FILE__, __LINE__)

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