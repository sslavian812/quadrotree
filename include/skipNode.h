#include<vector>
#include "cg/primitives/point.h"
#include "aabb.h"
#include "compressedNode.h"

using std::vector;

typedef pair<pair<int,int>, int> p3i;


template <class T>
struct SkipNode: public CompressedNode
{
public:
    //p3i c;// magic numbers to indicate a node
    // this -> next : {2*x+_1, 2*y+_1, z+1}
    //TODO: change magic coordinates to a class with method .next(int);


    SkipNode(aabb<T> box, point_2t p, p3i coords = make_pair(make_pair(0,0),0)) : CompressedNode(box, p)
    {
        c = coords;
    }

    SkipNode(double side = 2048, point_2t p, p3i coords = make_pair(make_pair(0,0),0)) : CompressedNode(side, p)
    {
        c = coords;
    }

//    p3i next(p3i p, int part)
//    {
//        switch(part)
//        {
//            case 0:
//                return make_pair(make_pair(p.first.first*2+1, p.first.first*2+1), p.second+1);
//            case 1:
//                return make_pair(make_pair(p.first.first*2, p.first.first*2+1), p.second+1);
//            case 2:
//                return make_pair(make_pair(p.first.first*2, p.first.first*2), p.second+1);
//            case 3:
//                return make_pair(make_pair(p.first.first*2+1, p.first.first*2), p.second+1);
//        }
//    }

    // локализует внутреннюю вершину, к которой можно подвесить данную точку как лист
    // вызывать от листа нельзя.
    SkipNode<T>* find(Point<T> p)
    {
        if(!boundary.contains(p))
            return NULL;

        int part = this->boundary.whatPart(p);


        if(children[part]!= NULL &&
                children[part]->boundary->contains(p) &&
                children[part]->childrensize > 0)
                return children[i]->find(p);

        return this;  // квадрат, который пересекает точку, но сам её не содержит.
    }


};
