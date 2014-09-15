#include<vector>
using namespace std;

#include "cg/primitives/point.h"
#include "aabb.h"

/*
template <class T>
struct Base
{};
*/

template <class T>
struct Node
{
public:

    bool haspoint;
	aabb<T> boundary;
	point_2t<T> point;
	Node* children[4];


    Node(aabb<T> box)
	{
		boundary = box;
        haspoint = false;
		children[0]=children[1]=children[2]=children[3]=NULL;
	}

    Node(double side=2048)
	{
		aabb<T> a;
        a.dimension=side;
		boundary=a;
        haspoint = false;
		children[0]=children[1]=children[2]=children[3]=NULL;	
	}

    vector<point_2t<T> > getPoints(aabb<T> box)
    {
        vector<point_2t<T> > res;
        if(! this->boundary.overlays(box))
            return res;

        if(box.contains(*(this)))
            res = this->getPoints();
        else
        {
            for(int i=0 ; i<4; ++i)
            {
                if(children[i]!= NULL)
                    insert(res.end(), (children[i]->getPoints(box)).begin(), (children[i]->getPoints(box)).end() );
            }
        }
        return res;
    }

    vector<point_2t<T> > getPoints(aabb<T> box, T eps)
    {
        vector<point_2t<T> > res;
        if(! this->boundary.overlays(box))
            return res;


        if(box->contains(*(this)) || (box+eps)->contains(*(this)) ) // here is the profit of epsilon!!
            res = this->getPoints();
        else
        {
            for(int i=0 ; i<4; ++i)
            {
                if(children[i]!= NULL)
                    insert(res.end(), (children[i]->getPoints(box, eps)).begin(), (children[i]->getPoints(box, eps)).begin() );
            }
        }
        return res;
    }


    vector<point_2t<T> > getPoints()
    {
        vector<point_2t<T> > res;

        if(haspoint)
            res.push_back(point);
        for(int i=0; i<4; ++i)
        {
            if(children[i]!= NULL)
            {
                vector<point_2t<T> > t = children[i]->getPoints();
                res.insert(res.end(), t.begin(), t.end());
            }
        }
        return res;
    }
	
	bool insert(point_2t<T> p)
	{
		if(!boundary.contains(p))
            return false;
					
		if(children[0]==NULL)  // in a leave
		{	
            if(!haspoint) // has place
			{
				point = p;
                return haspoint = true;
			}	
			else  // hasn't place
			{
				subdivide();  // is no a leave more
			}
		}
		
		// in internal node
        for(int i=0; i<4; ++i)
		{
			if(children[i]->insert(p))
				return true;
		}
		
		return false; // never reached!!!!
	}
	

	bool contains(point_2t<T> p)
	{
		if(!boundary.contains(p))
			return false;
		
        if(haspoint && p == point)
			return true;

		if(children[0] != NULL)
		
		for(int i=0; i<4; ++i)
		{
			if(children[i]->contains(p)) return true;
		}
		return false;
	}

private:
    void subdivide()
    {
        for(int i=0; i<4; ++i)
        {
            children[i] = new Node<T>(boundary.getPart(i));
        }

        for(int j=0; j<4; ++j)
        {
            if(children[j]->insert(point))
                break;
        }
        haspoint = false;
    }
};
