#include <cg/primitives/point.h>
#include <aabb.h>
#include <compressedNode.h>
#include <vector>
#include <map>
#include <misc/random_utils.h>
#include <algorithm>
#include <utility>

#include <boost/noncopyable.hpp>

using namespace std;

using std::vector;
using util::uniform_random_real;
using std::map;
//typedef pair<pair<int,int>, int> p3i;

// TODO: с такими координатами как сейчас - все упадет. переделать на даблы.

template <class T>
class SkipQuadTree// : boost::noncopyable
{
public:
    vector<CompressedNode<T> * > trees;
    aabb<T> max_area;

    // TODO: add get methods to temporaly public fields

private:
    vector< map<aabb<T>, CompressedNode<T>* > > index;

    uniform_random_real<double> *r;

    void push(point_2t<T> p)
    {
        CompressedNode<T>* node = new CompressedNode<T>(max_area, p);
        trees.push_back(node);
        index.push_back(map<aabb<T>, CompressedNode<T>* >());
        int level = trees.size()-1;
        aabb<T> coords = node->boundary;
        index[level][coords] = node;
    }

    bool contains_impl(point_2t<T> p, int curlevel, aabb<T> coords)
    {
        CompressedNode<T> * curnode = index[curlevel][coords];
                curnode = curnode->find(p);

        //TODO: проверить, бывает ли так, что вернулся NULL;
        if(curnode == NULL)
            return false;   // так быть не может, но мало ли

        //TODO: проверить, бывает ли так
        if(curnode->childrensize == 0 && curnode->point == p)
                return true;  // так вроде тоже не бывает, так как лист вернуть нельзя

        // проверим, что точка в одном из чилдов-листьев.
        int part = curnode->boundary.whatPart(p);
        if(curnode->children[part] != NULL &&
                curnode->children[part]->childrensize == 0 &&
                curnode->children[part]->point == p)
            return true;

        if(curlevel == 0)
            return false;

        aabb<T> curcoords = curnode->boundary;

        return contains_impl(p, curlevel-1, curcoords);
    }

    bool insert_impl(point_2t<T> p, int level, aabb<T> coords)
    {
        CompressedNode<T> * cur = index[level][coords];
        cur = cur->find(p);

        if(cur == NULL)
            return false; // так быть не должно, но мало ли

        if(level == 0)
        {
            cur = cur->insert(p);
            if(cur != NULL)
                index[level][cur->boundary] = cur;
            return (cur != NULL);
        }
        else
        {
            if(insert_impl(p, level-1, cur->boundary) && (*r)()>0)
            {
                CompressedNode<T> * temp_node = cur->insert(p);
                // нода - родитель листа с вставленой точкой.

                if(temp_node == NULL)
                    return false; // TODO: правда ли это так не бывает?

                index[level][temp_node->boundary] = temp_node;
                    // или добавится новая вершина или ничего не поменяется.
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    vector<point_2t<T> > getPoints_impl(aabb<T> box, double eps, int level, CompressedNode<T>* node)
    {
        vector<point_2t<T> > res;
        if(!box.overlays(node->boundary))
            return res;
        if(box.contains(node->boundary) || (box+eps).contains(node->boundary))
            return index[0][node->boundary]->getPoints();
        if(level == 0)
            return node->getPoints(box, eps);

        bool childUsed[4] = {false, false, false, false};
        int used=0;

        while(used!=4)
        {
            for(int i=0; i<4; ++i)
            {
                if(childUsed[i] || node->children[i] == NULL)
                    continue;
                //if(node->children[i]->c.second == node->c.second+1) // near depth or no more levels
                if(node->children[i]->boundary.dimension.x == node->boundary.dimension.x*2) // near depth or no more levels
                {
                    vector<point_2t<T> > v = getPoints_impl(box, eps, level, node->children[i]);
                    res.insert(res.end(), v.begin(), v.end());
                    childUsed[i] = true;
                    ++used;
                }
            }

            if(level != 0)
                node = index[level-1][node->boundary];
            else
                break;
        }
        return res;
    }

public:


    SkipQuadTree(T side=2048)
    {
        max_area = aabb<T>(side);
        r = new uniform_random_real<double>(-1.0, 1.0);
    }

    bool contains(point_2t<T> p)
    {
        int curlevel = trees.size()-1;

        while(curlevel >=0 && trees[curlevel]->childrensize == 0)
        {
            if(trees[curlevel]->point == p)
                return true;
            curlevel--;
        }

        if(curlevel < 0)
            return false;

        return contains_impl(p, curlevel, trees[curlevel]->boundary);
    }

    bool insert(point_2t<T> p)
    {
        int curlevel = trees.size()-1;
        if(curlevel < 0)
        {
            if(max_area.contains(p))
            {
                push(p);
                return true;
            }
            else
                return false;
        }

        if(contains(p))
            return false;

        aabb<T> coords = trees[curlevel]->boundary;
        bool flag = insert_impl(p, curlevel, coords);
        if(flag && (*r)()>0)
            push(p);
        return true;
    }

    vector<point_2t<T> > getPoints(aabb<T> box, double eps=0.0)
    {
        int level = trees.size()-1;
        if(level<0)
        {
            vector<point_2t<T> > res;
            return res;
        }
        CompressedNode<T> * node = trees[level];
        return getPoints_impl(box, eps, level, node);
    }

    vector<point_2t<T> > getPoints(point_2t<T> leftUp, point_2t<T> rightDown, double eps=0.0)
    {
//        point_2t<T> dim(rightDown.x-leftUp.x, leftUp.y - rightDown.y);
//        point_2t<T> pos(leftUp.x, leftUp.y-dim.y);
//        aabb<T> box(pos, dim);
        aabb<T> box;
        box = box.getByPoints(leftUp, rightDown);
        return getPoints(box, eps);
    }

    int getMaxLevel()
    {
        return trees.size()-1;
    }
};
