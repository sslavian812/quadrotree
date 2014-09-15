#include <vector>
#include <cg/primitives/point.h>
#include <aabb.h>

using std::vector;
using cg::vector_2t;
using std::pair;
using std::make_pair;


typedef pair<pair<int,int>, int> p3i;
typedef pair<aabb<double>, p3i> cbox;


//TODO: все виснет, если два раза ткнуть в одну и ту же точку.

template <class T>
struct CompressedNode
{
public:
    int childrensize;
    aabb<T> boundary;
    point_2t<T> point;
    CompressedNode* children[4];
    p3i c;// magic numbers to indicate a node
    // this -> next : {2*x+_1, 2*y+_1, z+1}

    CompressedNode(aabb<T> box, point_2t<T> p, p3i coords = make_pair(make_pair(0,0),0))
    {
        this->boundary = box;
        this->point = p;
        c = coords;

        children[0]=children[1]=children[2]=children[3]=NULL;
        childrensize = 0;
    }

    p3i next(p3i p, int part)
    {
        switch(part)
        {
            case 0:
                return make_pair(make_pair(p.first.first*2+1, p.first.first*2+1), p.second+1);
            case 1:
                return make_pair(make_pair(p.first.first*2, p.first.first*2+1), p.second+1);
            case 2:
                return make_pair(make_pair(p.first.first*2, p.first.first*2), p.second+1);
            case 3:
                return make_pair(make_pair(p.first.first*2+1, p.first.first*2), p.second+1);
        }
        return make_pair(make_pair(0,0),0); // newer reached!!!!
    }

//TODO: проверить, что инсерт отдает вершину, которую пришлось создать, но не лист
    CompressedNode<T>* insert(point_2t<T> p)
    {
        if(!boundary.contains(p))
            return NULL;

        // в какую часть текущего узла попадаем
        int part = boundary.whatPart(p);


        // если лист, значит мы - корень-лист
        if(childrensize==0)
        {
            point_2t<T> p2 = point;
            if(p2 == p)
                return NULL; // вставлять не будем, но сделаем вид, что вставили.
                             // если возвращать NULL, то может что-нибудь сломаться
                             // TODO: если не NULL, все равно ведь падает из-за этого иногда
            int part2 = boundary.whatPart(p2);

            if(part != part2)
            {
                children[part]=new CompressedNode(boundary.getPart(part), p, next(c, part));
                ++childrensize;

                children[part2]=new CompressedNode(boundary.getPart(part2), p2, next(c, part2));
                ++childrensize;
                return this; // вернем корень-не_лист
            }
            else
            {
                // TODO: boilerplate code
                cbox area = getBest(p,p2);
                aabb<T> cur = area.first;
                p3i coords = area.second;
                // в этот момент p и p2 попадают в разные части aabb'са cur

                int k = cur.whatPart(p);
                int k2 = cur.whatPart(p2);

                CompressedNode<T> *n = new CompressedNode(cur.getPart(k), p, next(coords, k));
                CompressedNode<T> *n2 = new CompressedNode(cur.getPart(k2), p2, next(coords, k2));

                children[part] = new CompressedNode(cur, k, n, k2, n2, coords);
                childrensize++;
                return children[part]; // вернем родителя листа, где теперь точка.
            }

        }

        // not a leave


        // suitable part is empty
        if(children[part] == NULL)
        {
            children[part] =new CompressedNode(boundary.getPart(part), p, next(c, part));
            ++childrensize;
            return this;  // вернем родителя листа, где теперь точка.
        }
        else// not a leave and nesessary part is not empty
        {
            // если наш ребенок - лист.
            if(children[part]->childrensize == 0)
            {
                // TODO: boilerplate code

                // если ребенок - лист, его придется удалить
                // т.к. лист полноразмерный, а мог бы быть сжатый
                point_2t<T> p2 = children[part]->point;
                delete children[part];

                // TODO: boilerplate code
                cbox area = getBest(p,p2);
                aabb<T> cur = area.first;
                p3i coords = area.second;
                // в этот момент p и p2 попадают в разные части aabb'са cur

                int k = cur.whatPart(p);
                int k2 = cur.whatPart(p2);

                CompressedNode<T> *n = new CompressedNode(cur.getPart(k), p, next(coords, k));
                CompressedNode<T> *n2 = new CompressedNode(cur.getPart(k2), p2, next(coords, k2));

                children[part] = new CompressedNode(cur, k, n, k2, n2, coords);
                childrensize++;
                return children[part]; // вернем родителя листа, где теперь точка.
            }
            else // ребенок - не лист
            {
                // ребенок достаточного размера
                if(children[part]->boundary.contains(p))
                {
                    return children[part]->insert(p);
                        //return children[part];
                }
                else  // ребенок урезан!
                {
                    point_2t<T> p2 = children[part]->boundary.position; // точка из урезанного чилда
                    vector_2t<T> v(children[part]->boundary.dimension.x/2, children[part]->boundary.dimension.y/2);
                    p2 =p2 + v;
                            // TODO: заменить на .getcenter()

                    // TODO: boilerplate code
                    cbox area = getBest(p,p2);
                    aabb<T> cur = area.first;
                    p3i coords = area.second;
                    // в этот момент p и p2 попадают в разные части aabb'са cur

                    int k = cur.whatPart(p);
                    int k2 = cur.whatPart(p2);

                    CompressedNode<T> * n = new CompressedNode(cur.getPart(k), p, next(coords, k));
                    CompressedNode<T> * n2 = children[part];

                    children[part] = new CompressedNode(cur, k, n, k2, n2, coords);
                    childrensize++;

                    return children[part];
                }
            }
        }
        //return NULL;  // never reached!!
    }

    bool contains(point_2t<T> p)
    {
        if(!boundary.contains(p))
            return false;

        if(childrensize==0 && p == point)
            return true;

        int part = boundary.whatPart(p);

        if(children[part]!= NULL)
                return children[part]->contains(p);
        return false;
    }

    vector<point_2t<T> > getPoints(aabb<T> box, T eps=0.0)
    {
        vector<point_2t<T> > res;
        if(! this->boundary.overlays(box))
            return res;

        if(childrensize == 0 && (box.contains(point) || (box+eps).contains(point)))
        {
            res.push_back(point);
            return res;
        }


        if(box.contains(this->boundary) || (box+eps).contains(this->boundary) ) // here is the profit of epsilon!!
            res = this->getPoints();
        else
        {
            for(int i=0 ; i<4; ++i)
            {
                if(children[i]!= NULL)
                {
                    vector<point_2t<T> > t = children[i]->getPoints(box, eps);
                    res.insert(res.end(), t.begin(), t.end());
                }
            }
        }
        return res;
    }


    vector<point_2t<T> > getPoints()
    {
        vector<point_2t<T> > res;

        if(childrensize == 0)
        {
            res.push_back(point);
            return res;
        }
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

    // локализует внутреннюю вершину, к которой можно подвесить данную точку как лист
    // вызывать от листа нельзя.
    CompressedNode<T>* find(point_2t<T> p)
    {
        if(! this->boundary.contains(p))
            return NULL;

        int part = this->boundary.whatPart(p);


        if(children[part]!= NULL &&
                children[part]->boundary.contains(p) &&
                children[part]->childrensize > 0)
                return children[part]->find(p);

        return this;  // квадрат, который пересекает точку, но сам её не содержит.
    }


private :
    CompressedNode(aabb<T> box, int part, CompressedNode<T>* node, int part2, CompressedNode<T>* node2, p3i coords)
    {
        boundary = box;
        c = coords;
        children[0]=children[1]=children[2]=children[3]=NULL;
        children[part] = node;
        children[part2] = node2;
        childrensize = 2;
    }

    cbox getBest(point_2t<T> p, point_2t<T> p2)
    {
        aabb<T> cur = this->boundary;
        int curpart = cur.whatPart(p);
        p3i coords = c;

        while(cur.whatPart(p) == cur.whatPart(p2))
        {
            curpart = cur.whatPart(p);
            coords = next(coords, curpart);
            cur = cur.getPart(curpart);
        }
        // в этот момент p и p2 попадают в разные части aabb'са cur
        return make_pair(cur, coords);
    }
};
