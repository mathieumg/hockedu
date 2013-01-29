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
#define _WINSOCKAPI_
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
	QueueThreadSafe(unsigned int pMaxBufferSize = 0);
	bool pop(T&);
	bool push(T element);

	inline bool empty()const{return mFile.empty();}

private:
	void getMutex() const;
	void releaseMutex() const;

	HANDLE mMutex;
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
	mMutex = CreateMutex(NULL, FALSE, NULL);
}


template <typename T>
bool QueueThreadSafe<T>::push( T element )
{
	getMutex();
	bool wObjectAdded = false;
	if(mMaxBufferSize && (unsigned int)mFile.size() < mMaxBufferSize )
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

