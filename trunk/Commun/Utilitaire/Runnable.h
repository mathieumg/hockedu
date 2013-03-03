#ifndef RUNNABLE_H
#define RUNNABLE_H

#include <functional>
class Runnable
{
public:
    Runnable(std::function<void (Runnable*)> function, bool keepAlive = false):
        mFunction(function), mbKeepAlive(keepAlive)
    {

    }
    inline void Run()
    {
        mFunction(this);
    }

    /// Accessors of mbKeepAlive
    inline bool KeepAlive() const { return mbKeepAlive; }
    inline void setKeepAlive(bool val) { mbKeepAlive = val; }
private:
    std::function<void (Runnable*)> mFunction;
    bool mbKeepAlive;
};

#endif