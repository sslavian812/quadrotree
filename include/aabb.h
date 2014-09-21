#pragma once

#include "cg/primitives/point.h"
using cg::point_2t;

template <class T>
struct aabb
{
    point_2t<T> position;
    point_2t<T> dimension;

    aabb()
    {
        point_2t<T> p1(0,0);
        position=p1;
        dimension.x=dimension.y=T(2048);
    }
    aabb(T side)
    {
        point_2t<T> p1(0,0);
        position=p1;
        dimension.x=dimension.y=side;
    }

    aabb(point_2t<T> p, point_2t<T> d): position(p), dimension(d)
    {}

    static aabb<T> getByPoints(point_2t<T> leftUp, point_2t<T> rightDown)
    {
        point_2t<T> dim(rightDown.x-leftUp.x, leftUp.y - rightDown.y);
        point_2t<T> pos(leftUp.x, leftUp.y-dim.y);
        aabb<T> box(pos, dim);
        return box;
    }

    inline bool operator<(aabb<T> const & other) const
    {
       if (position == other.position)
          return dimension < other.dimension;
       else
          return position < other.position;
    }



    bool contains(point_2t<T> point)
    {
        if((point.x >= position.x) && (point.x < position.x + dimension.x))
            return (point.y >= position.y) && (point.y < position.y + dimension.y);
        return false;
    }

    bool contains(aabb<T> other)
    {
        if(other.position.x < this->position.x ||
                other.position.y < this->position.y)
            return false;

        T dx = other.dimension.x;
        T dy = other.dimension.y;
        T x = other.position.x;
        T y = other.position.y;

        point_2t<T> p0(x, y), p1(x+dx, y), p2(x, y+dy), p3(x+dx, y+dy);

        if(contains(p0) && contains(p1) && contains(p2) && contains(p3))  // TODO: substruct epsilon from p1 p2 and p3
            return true;
        else
            return false;
    }

    bool overlays(aabb<T> other)
    {
        //return (this->contains(other) || other->contains(*(this)));

        T dx = other.dimension.x;
        T dy = other.dimension.y;
        T x = other.position.x;
        T y = other.position.y;
        
        point_2t<T> p0(x, y), p1(x+dx, y), p2(x, y+dy), p3(x+dx, y+dy);
        if(contains(p0) || contains(p1) || contains(p2) || contains(p3))
            return true;
        dx = dimension.x;
        dy = dimension.y;
        x = position.x;
        y = position.y;
        p0=point_2t<T>(x, y), p1=point_2t<T>(x+dx, y), p2=point_2t<T>(x, y+dy), p3=point_2t<T>(x+dx, y+dy);
        if(other.contains(p0) || other.contains(p1) || other.contains(p2) || other.contains(p3))
            return true;

        return false;
    }

    aabb<T> operator+(T eps)
    {
        aabb<T> box(position, point_2t<T>(dimension.x+eps, dimension.y+eps));
        return box;
    }


    aabb getPart(int k) // k in {0,1,2,3}
    {
        switch(k)
        {
            case 0:
                return aabb<T>(point_2t<T>(position.x+dimension.x/2, position.y+dimension.y/2), point_2t<T>(dimension.x/2, dimension.y/2));
                break;
            case 1:
                return aabb<T>(point_2t<T>(position.x, position.y+dimension.y/2), point_2t<T>(dimension.x/2, dimension.y/2));
                break;
            case 2:
                return aabb<T>(point_2t<T>(position.x, position.y), point_2t<T>(dimension.x/2, dimension.y/2));
                break;
            case 3:
                return aabb<T>(point_2t<T>(position.x+dimension.x/2, position.y), point_2t<T>(dimension.x/2, dimension.y/2));
                break;
            default: //never reached!!!!
                return *this;
        }
    }

    int whatPart (point_2t<T> point)
    {
        if(point.x >= this->position.x+dimension.x/2)
            if(point.y >= this->position.y+dimension.y/2)
                return 0;
            else
                return 3;
        else
            if( point.y >= this->position.y+dimension.y/2)
                return 1;
            else
                return 2;
    }


};

