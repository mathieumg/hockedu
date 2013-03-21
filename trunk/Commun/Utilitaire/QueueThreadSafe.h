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
#include "../../Sources/C++/Reseau/FacadePortability.h"

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
    inline unsigned int size()const{return (unsigned int)mFile.size();}
private:
	void getMutex();
	void releaseMutex();
	HANDLE_MUTEX mMutex;
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
    FacadePortability::createMutex(mMutex);
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
void QueueThreadSafe<T>::releaseMutex()
{
    FacadePortability::releaseMutex(mMutex);
}

template <typename T>
void QueueThreadSafe<T>::getMutex()
{
    FacadePortability::takeMutex(mMutex);
}



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

