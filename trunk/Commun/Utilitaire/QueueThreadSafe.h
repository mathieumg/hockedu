//////////////////////////////////////////////////////////////////////////////
/// @file QueueThreadSafe.h
/// @author Mathieu Parent
/// @date 2013-01-08
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <queue>
#include <Windows.h>


///////////////////////////////////////////////////////////////////////////
/// @class QueueThreadSafe
/// @brief Classe Template qui represente une queue thread safe
///
/// @author Mathieu Parent
/// @date 2013-01-08
///////////////////////////////////////////////////////////////////////////
template <typename T>
class QueueThreadSafe {

public:
	QueueThreadSafe();
	T pop();
	T front();
	T back();

	void push(T element);

	inline size_t size() const {return mFile.size();}
	bool empty() const {return mFile.empty();}

private:
	void getMutex() const;
	void releaseMutex() const;

	HANDLE mMutex;
	std::queue<T> mFile;
};


template <typename T>
QueueThreadSafe<T>::QueueThreadSafe()
{
	mMutex = CreateMutex(NULL, FALSE, NULL);
}


template <typename T>
void QueueThreadSafe<T>::push( T element )
{
	getMutex();
	mFile.push(element);
	releaseMutex();
}

template <typename T>
T QueueThreadSafe<T>::back()
{
	getMutex();
	T element = mFile.back();
	releaseMutex();
	return element;
}

template <typename T>
T QueueThreadSafe<T>::front()
{
	getMutex();
	T element = mFile.front();
	releaseMutex();
	return element;
}

template <typename T>
T QueueThreadSafe<T>::pop()
{
	getMutex();
	T buf = mFile.front();
	mFile.pop();
	releaseMutex();
	return buf;
	
}



template <typename T>
void QueueThreadSafe<T>::releaseMutex() const
{
	ReleaseMutex(mMutex);
}

template <typename T>
void QueueThreadSafe<T>::getMutex() const
{
	WaitForSingleObject(mMutex,INFINITE);
}



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

