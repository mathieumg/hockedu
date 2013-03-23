#pragma once

#include "Vecteur.h"

template<class T> inline T Min(T a, T b)
{
    return a<=b ? a : b;
}
template<class T> inline T Max(T a, T b)
{
    return a>=b ? a : b;
}
template< class T > inline T Clamp( const T X, const T min, const T max )
{
    return X<min ? min : X<max ? X : max;
}
template< class T > inline T Square( const T A )
{
    return A*A;
}

//
// A rectangular minimum bounding volume.
//
class BoundingBox
{
public:
    // Variables.
    Vecteur2 mMinPoint;
    Vecteur2 mMaxPoint;
    bool IsValid;

    // Constructors.
    BoundingBox() { Init(); }
    BoundingBox( const Vecteur2& InMin, const Vecteur2& InMax ) : mMinPoint(InMin), mMaxPoint(InMax), IsValid(1) {}
    BoundingBox( const Vecteur3& InMin, const Vecteur3& InMax ) : mMinPoint(InMin), mMaxPoint(InMax), IsValid(1) {}

    /** Utility function to build an AABB from Origin and Extent */
    static BoundingBox BuildAABB( const Vecteur2& Origin, const Vecteur2& Extent )
    {
        BoundingBox NewBox(Origin - Extent, Origin + Extent);
        return NewBox;
    }
    static BoundingBox BuildAABB( const Vecteur3& Origin, const Vecteur3& Extent )
    {
        BoundingBox NewBox(Origin - Extent, Origin + Extent);
        return NewBox;
    }

    // Accessors.
    Vecteur2& GetExtrema( int i )
    {
        return (&mMinPoint)[i];
    }
    const Vecteur2& GetExtrema( int i ) const
    {
        return (&mMinPoint)[i];
    }

    // Functions.
    void Init()
    {
        mMinPoint = mMaxPoint = Vecteur2(0,0);
        IsValid = 0;
    }

    // Too small for memcpy's
    inline BoundingBox& operator=( const BoundingBox &Other )
    {
        mMinPoint = Other.mMinPoint;
        mMaxPoint = Other.mMaxPoint;
        IsValid = Other.IsValid;
        return *this;
    }

    inline BoundingBox& operator+=( const Vecteur3 &Other )
    {
        return BoundingBox::operator+=( (const Vecteur2 &)Other );
    }
    inline BoundingBox& operator+=( const Vecteur2 &Other )
    {
        if( IsValid )
        {
            mMinPoint[VX] = ::Min( mMinPoint[VX], Other[VX] );
            mMinPoint[VY] = ::Min( mMinPoint[VY], Other[VY] );

            mMaxPoint[VX] = ::Max( mMaxPoint[VX], Other[VX] );
            mMaxPoint[VY] = ::Max( mMaxPoint[VY], Other[VY] );
        }
        else
        {
            mMinPoint = mMaxPoint = Other;
            IsValid = 1;
        }
        return *this;
    }
    BoundingBox operator+( const Vecteur3& Other ) const
    {
        return BoundingBox(*this) += Other;
    }
    BoundingBox operator+( const Vecteur2& Other ) const
    {
        return BoundingBox(*this) += Other;
    }
    BoundingBox& operator+=( const BoundingBox& Other )
    {
        if( IsValid && Other.IsValid )
        {
            mMinPoint[VX] = ::Min( mMinPoint[VX], Other.mMinPoint[VX] );
            mMinPoint[VY] = ::Min( mMinPoint[VY], Other.mMinPoint[VY] );

            mMaxPoint[VX] = ::Max( mMaxPoint[VX], Other.mMaxPoint[VX] );
            mMaxPoint[VY] = ::Max( mMaxPoint[VY], Other.mMaxPoint[VY] );
        }
        else if( Other.IsValid )
        {
            *this = Other;
        }
        return *this;
    }
    BoundingBox operator+( const BoundingBox& Other ) const
    {
        return BoundingBox(*this) += Other;
    }
    Vecteur2& operator[]( int i )
    {
        checkf(i>-1);
        checkf(i<2);
        if( i == 0 )		return mMinPoint;
        else				return mMaxPoint;
    }
    BoundingBox ExpandBy( float W ) const
    {
        return BoundingBox( mMinPoint - Vecteur2(W,W), mMaxPoint + Vecteur2(W,W) );
    }

    // Returns the midpoint between the min and max points.
    Vecteur2 GetCenter() const
    {
        return Vecteur2( ( mMinPoint + mMaxPoint ) * 0.5f );
    }
    // Returns the extent around the center
    Vecteur2 GetExtent() const
    {
        return 0.5f*(mMaxPoint - mMinPoint);
    }

    Vecteur2 GetSize() const
    {
        return mMaxPoint - mMinPoint;
    }

    void GetCenterAndExtents( Vecteur2 & center, Vecteur2 & Extents ) const
    {
        Extents = GetExtent();
        center = mMinPoint + Extents;
    }

    bool Intersect( const BoundingBox & other ) const
    {
        if( mMinPoint[VX] > other.mMaxPoint[VX] || other.mMinPoint[VX] > mMaxPoint[VX] )
            return false;
        if( mMinPoint[VY] > other.mMaxPoint[VY] || other.mMinPoint[VY] > mMaxPoint[VY] )
            return false;
        return true;
    }

    // Returns true if the box passes in parameter is completely inside this box
    bool Contains( const BoundingBox& other ) const
    {
        return( other.mMinPoint[VX] > mMinPoint[VX] && other.mMaxPoint[VX] < mMaxPoint[VX] 
            && other.mMinPoint[VY] > mMinPoint[VY] && other.mMaxPoint[VY] < mMaxPoint[VY] );
    }

    // Checks to see if the location is inside this box
    bool IsInside( const Vecteur2& In ) const
    {
        return ( In[VX] > mMinPoint[VX] && In[VX] < mMaxPoint[VX]
            && In[VY] > mMinPoint[VY] && In[VY] < mMaxPoint[VY] );
    }
    bool IsInside( const Vecteur3& In ) const
    {
        return IsInside((const Vecteur2&)In);
    }

    /** Calculate volume of this box. */
    float GetVolume() const
    {
        return ((mMaxPoint[VX]-mMinPoint[VX]) * (mMaxPoint[VY]-mMinPoint[VY]) * (mMaxPoint[VZ]-mMinPoint[VZ]));
    }
    
    /** Util to calculate distance from a point to a bounding box */
    inline Vecteur2 ComputeNearestPoint(const Vecteur2& Point) const
    {
        return Vecteur2(
            Clamp(Point[VX], mMinPoint[VX], mMaxPoint[VX]),
            Clamp(Point[VY], mMinPoint[VY], mMaxPoint[VY]));
    }
    inline Vecteur2 ComputeNearestPoint(const Vecteur3& Point) const
    {
        return ComputeNearestPoint((const Vecteur2&) Point);
    }

    /** Util to calculate distance from a point to a bounding box */
    inline float ComputeSquaredDistanceToPoint( const Vecteur3& Point ) const
    {
        return ComputeSquaredDistanceToPoint( (const Vecteur2&) Point );
    }
    inline float ComputeSquaredDistanceToPoint( const Vecteur2& Point ) const
    {
        // Accumulates the distance as we iterate axis
        float DistSquared = 0.f;

        // Check each axis for min/max and add the distance accordingly
        // NOTE: Loop manually unrolled for > 2x speed up
        if (Point[VX] < mMinPoint[VX])
        {
            DistSquared += Square(Point[VX] - mMinPoint[VX]);
        }
        else if (Point[VX] > mMaxPoint[VX])
        {
            DistSquared += Square(Point[VX] - mMaxPoint[VX]);
        }

        if (Point[VY] < mMinPoint[VY])
        {
            DistSquared += Square(Point[VY] - mMinPoint[VY]);
        }
        else if (Point[VY] > mMaxPoint[VY])
        {
            DistSquared += Square(Point[VY] - mMaxPoint[VY]);
        }
        
        return DistSquared;
    }

    /** Return closest point on or inside the box to the given point in space. */
    Vecteur2 GetClosestPointTo(const Vecteur3& Point) const
    {
        return GetClosestPointTo((const Vecteur2&) Point);
    }
    Vecteur2 GetClosestPointTo(const Vecteur2& Point) const
    {
        // Start by considering the Point inside the Box.
        Vecteur2 ClosestPoint = Point;

        // Now clamp to inside box if it's outside.
        if( Point[VX] < mMinPoint[VX] )
        {
            ClosestPoint[VX] = mMinPoint[VX];
        }
        else if( Point[VX] > mMaxPoint[VX] )
        {
            ClosestPoint[VX] = mMaxPoint[VX];
        }

        // Now clamp to inside box if it's outside.
        if( Point[VY] < mMinPoint[VY] )
        {
            ClosestPoint[VY] = mMinPoint[VY];
        }
        else if( Point[VY] > mMaxPoint[VY] )
        {
            ClosestPoint[VY] = mMaxPoint[VY];
        }
        
        return ClosestPoint;
    }
};

