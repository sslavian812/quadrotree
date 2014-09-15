#pragma once
#include <vector>
#include <cg/primitives/point.h>
#include <aabb.h>
#include <compressedNode.h>

using std::vector;

typedef pair<pair<int,int>, int> p3i;


template <class T>
struct SkipNode: CompressedNode<T>
{
public:



    SkipNode(aabb<T> box, point_2t<T> p, p3i coords = make_pair(make_pair(0,0),0)) : CompressedNode<T>(box, p, coords)
    {}


    // локализует внутреннюю вершину, к которой можно подвесить данную точку как лист
    // вызывать от листа нельзя.
    SkipNode<T>* find(point_2t<T> p)
    {
        if(! this->boundary.contains(p))
            return NULL;

        int part = this->boundary.whatPart(p);


        if(this->children[part]!= NULL &&
                this->children[part]->boundary.contains(p) &&
                this->children[part]->childrensize > 0)
                return this->children[part]->find(p);

        return this;  // квадрат, который пересекает точку, но сам её не содержит.
    }


};
