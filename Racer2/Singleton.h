// by Mark Featherstone (C) 2015 All Rights Reserved.
#ifndef SINGLETON_H
#define SINGLETON_H

#include "Error.h"

/*
We often want only one instance of an object, especially if we are creating manager classes
to hold onto groups of objects and manage their lifetime. A singleton is a "design pattern"
designed to do just that - ensure there is only ever one instance of a specific class.

As we'll use it again and again, we don't want to repeat ourselves, this template and the
two macros help us write less code to get the job done.
Example - declaration
---------------------
	#include "Singleton.h"
	class ModeManager : public Singleton<ModeManager>
	{
	public:
		//add all your functions and data
	};
	SINGLETON_GET(ModeManager);

Example - in use
----------------
	new ModeManager;
	....
	GetModeManager()->DoStuff();
	....
	delete GetModeManager();

*/



#define SINGLETON_GET(aClass)																\
	inline aClass* Get##aClass()															\
	{																						\
		MPOD_ASSERT_MSG( aClass::Get()!=0, "%s singleton doesn't exist", #aClass);			\
		return aClass::Get();																\
	}

template<class T> class Singleton
{
public:
	Singleton()
	{
		MPOD_ASSERT_MSG( spSingleton == 0, "Singleton already exists");
		spSingleton = static_cast<T*>(this);
	}

	virtual ~Singleton()
	{
		MPOD_ASSERT_MSG( spSingleton != 0, "Singleton doesn't exist");
		spSingleton = 0;
	}

	static T* Get()	{ return spSingleton; }

private:
	static T*	spSingleton;
};

template<typename T> T* Singleton<T>::spSingleton = 0;

#endif
