
#pragma once

template<typename ContainerType,int MaxSize>
class Flags
{
public:
    /// Make sure when using the initialiser to bitshift the values  
    /// ex : Flags f(1<<FLAG1 | 1<<FLAG2);
    Flags(ContainerType initialFlags = 0):mValue(initialFlags)
    {
        checkf(sizeof(ContainerType)*8 >= MaxSize, "Container for the flag is too small to hold all the flags");
    }
    __forceinline void SetFlag(const int can, const int number)
    {
        // if (f) mUpdateFlag |= POSITION; else mUpdateFlag &= ~POSITION;
        ContainerType flag = 1 << number;
        mValue ^= (-can ^ mValue) & flag;
    }
    __forceinline bool IsFlagSet(const int number) const
    {
        ContainerType flag = 1 << number;
        return !!(mValue & flag);
    }
    __forceinline bool operator==(Flags<ContainerType,MaxSize> f)
    {
        return mValue == f.mValue;
    }
    ContainerType mValue;
};