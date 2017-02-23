// by Mark Featherstone (C) 2015 All Rights Reserved.
#pragma once
#ifndef ERROR_H
#define ERROR_H




//***********************************************************************
//we may want assert messages to go to the screen and/or get logged in an error file
namespace Assert
{
	typedef bool (*HandlerFnPtr)( const char* condition, const char* msg, const char* file, int line, void *pContext );

	HandlerFnPtr getHandler();
	void *getHandlerContext();
	void setHandler( HandlerFnPtr newHandler, void *pContext );

	bool report(const char* condition, const char* msg, const char* file, int line);
	bool reportFormatted(const char* condition, const char* file, int line, const char* msg, ...);
}

///this stuff is always present and never quiet, use it sparingly
#define MPOD_HALT() do { __debugbreak(); } while(0)
#define MPOD_VERIFY(cond) \
	do { if ((!(cond)) && Assert::report(#cond, 0, __FILE__, __LINE__)) \
	MPOD_HALT(); } while(0)

#define MPOD_VERIFY_MSG(cond, msg, ...) \
	do { if ((!(cond)) && Assert::reportFormatted(#cond, __FILE__, __LINE__, (msg), __VA_ARGS__)) \
	MPOD_HALT(); } while(0)

#define MPOD_VERIFY_FAIL(msg) \
	do { if (Assert::report(NULL, (msg), __FILE__, __LINE__)) \
	MPOD_HALT(); } while(0)

//asserts can disappear or go quiet in release builds, verifies do not
//#define REMOVE_ASSERTS
#ifdef REMOVE_ASSERTS
	///make the asserts vanish completely
	#define MPOD_ASSERT(condition) do { (void)sizeof(condition); } while(0)
	#define MPOD_ASSERT_MSG(condition, msg, ...) do { (void)sizeof(condition); (void)sizeof(msg); } while(0)
	#define MPOD_ASSERT_FAIL(msg) do { (void)sizeof(msg); } while(0)
#else
	#ifdef MPOD_FINAL
		///log the assert quietly and carry on
		#define MPOD_ASSERT(cond) \
		do { if ((!(cond)) && Assert::report(#cond, 0, __FILE__, __LINE__)) (void)sizeof(cond); } while(0)

		#define MPOD_ASSERT_MSG(cond, msg, ...) \
		do { if ((!(cond)) && Assert::reportFormatted(#cond, __FILE__, __LINE__, (msg), __VA_ARGS__)) (void)sizeof(cond); } while(0)

		#define MPOD_ASSERT_FAIL(msg) \
		do { if (Assert::report(NULL, (msg), __FILE__, __LINE__)) (void)sizeof(cond); } while(0)
	#else
		///stop everything and shout at the user
		#define MPOD_ASSERT(cond) \
		do { if ((!(cond)) && Assert::report(#cond, 0, __FILE__, __LINE__)) \
		MPOD_HALT(); } while(0)

		#define MPOD_ASSERT_MSG(cond, msg, ...) \
		do { if ((!(cond)) && Assert::reportFormatted(#cond, __FILE__, __LINE__, (msg), __VA_ARGS__)) \
		MPOD_HALT(); } while(0)

		#define MPOD_ASSERT_FAIL(msg) \
		do { if (MPOD::Assert::report(NULL, (msg), __FILE__, __LINE__)) \
		MPOD_HALT(); } while(0)
	#endif
#endif

#endif