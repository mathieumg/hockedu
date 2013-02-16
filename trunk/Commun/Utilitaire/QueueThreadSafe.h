//////////////////////////////////////////////////////////////////////////////
/// @file QueueThreadSafe.h
/// @author Mathieu Parent
/// @date 2013-01-08
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <queue>


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
// Includes pour les mutex en WIN API
#define _WINSOCKAPI_
#include <Windows.h>
#else
#include <pthread.h>
// Includes pour mutex en linux

#endif

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
	QueueThreadSafe(unsigned int pMaxBufferSize = 0);
	bool pop(T&);
	bool push(T element);

	inline bool empty()const{return mFile.empty();}

private:
	void getMutex() const;
	void releaseMutex() const;
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	HANDLE mMutex;
	#else
	pthread_mutex_t mMutex;
	#endif
	std::queue<T> mFile;
	unsigned int mMaxBufferSize;
};





template <typename T>
bool QueueThreadSafe<T>::pop( T& pOutElem )
{
	getMutex();
	bool wObjectPresent = false;
	if(!mFile.empty())
	{
		pOutElem = mFile.front();
		mFile.pop();
		wObjectPresent = true;
	}
	releaseMutex();
	return wObjectPresent;
}


template <typename T>
QueueThreadSafe<T>::QueueThreadSafe(unsigned int pMaxBufferSize /*= 0 */):mMaxBufferSize(pMaxBufferSize)
{
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	mMutex = CreateMutex(NULL, FALSE, NULL);
	#else
	pthread_mutex_init (&mMutex, NULL);
	#endif
}


template <typename T>
bool QueueThreadSafe<T>::push( T element )
{
	getMutex();
	bool wObjectAdded = false;
	if(!mMaxBufferSize || (unsigned int)mFile.size() < mMaxBufferSize )
	{
		mFile.push(element);
		wObjectAdded = true;
	}
	releaseMutex();
	return wObjectAdded;
}

// template <typename T>
// T QueueThreadSafe<T>::pop()
// {
// 	getMutex();
// 	T buf = mFile.front();
// 	mFile.pop();
// 	releaseMutex();
// 	return buf;
//
// }

template <typename T>
void QueueThreadSafe<T>::releaseMutex() const
{
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	ReleaseMutex(mMutex);
	#else
	pthread_mutex_unlock (&mMutex);
	#endif
}

template <typename T>
void QueueThreadSafe<T>::getMutex() const
{
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	WaitForSingleObject(mMutex,INFINITE);
	#else
	pthread_mutex_lock (&mMutex);
	#endif
}



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

