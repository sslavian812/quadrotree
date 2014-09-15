#include <cg/primitives/point.h>
#include <aabb.h>
#include <skipNode.h>
#include <vector>
#include <map>
#include <misc/random_utils.h>

using std::vector;
using util::uniform_random_real;
using std::map;
typedef pair<pair<int,int>, int> p3i;

// TODO: с такими координатами как сейчас - все упадет. переделать на даблы.

template <class T>
class SkipQuadTree
{

private:
    vector<SkipNode<T> * > trees;
    vector< map<p3i, SkipNode<T>* > > index;
    aabb<T> max_area;
    uniform_random_real<double> r;

    void push(point_2t<T> p)
    {
        trees.push_back(new SkipNode<T>(max_area, p));
        index[trees.size()-1][trees.back()->c] = trees.back();
    }

    bool contains_impl(point_2t<T> p, int curlevel, p3i coords)
    {
        SkipNode<T> * curnode = index[curlevel][coords];
                curnode = curnode->find(p);

        //TODO: проверить, бывает ли так, что вернулся NULL;
        if(curnode == NULL)
            return false;   // так быть не может, но мало ли

        //TODO: проверить, бывает ли так
        if(curnode->childrensize == 0 && cur->point == p)
                return true;  // так вроде тоже не бывает, так как лист вернуть нельзя

        // проверим, что точка в одном из чилдов-листьев.
        int part = curnode->boundary->whatPart(p);
        if(curnode->children[part] != NULL &&
                curnode->children[part]->childrensize == 0 &&
                curnode->children[part]->point == p)
            return true;

        if(curlevel == 0)
            return false;

        p3i coords = curnode->c;

        return contains_impl(p, curlevel-1, coords);
    }

    bool insert_impl(point_2t<T> p, int level, p3i coords)
    {
        SkipNode<T> * cur = index[level][coords];
        cur = cur->find(p);
        if(level == 0)
        {
            cur->insert(p);
            return true;
            //TODO: посмотреть, что вернет инсерт и отвечать задесь вставилась\не вставилась
        }
        else
        {
            if(insert_impl(p, level-1, cur->c) && r()>0)
            {
                CompressedNode<T> * temp_node = cur->insert(p);
                // нода - родитель листа с вставленой точкой.

                if(temp_node == NULL)
                    return false; // TODO: правда ли это так не бывает?

                index[level][temp_node->c] = temp_node;
                    // или добавится нова вершина или ничего не поменяется.
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    vecotor<point_2t<T> > getPoints(aabb<T> box, double eps=0.0, int level, SkipNode<T>* node)
    {
        vecotor<point_2t<T> > res;
        if(!box.overlays(node->boundary))
            return res;
        if(box.contains(node->boundary) || (box+eps).contains(node->boundary))
            return index[0][node->c]->getPoints();
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
                if(node->children[i]->c.second == node->c.second+1) // near depth or no more levels
                {
                    vecotor<point_2t<T> > v = getPoints(box, eps, level, node->children[i]);
                    std::insert(res.end(), v.begin(), v.end());
                    childUsed[i] = true;
                    ++used;
                }
            }

            if(level != 0)
                node = index[level-1][node->c];
            else
                break;
        }
        return res;
    }

public:
    SkipQuadTree(T side)
    {
        max_area = aabb<T>(side);
        r = uniform_random_real<double>(-1.0, 1.0);
    }

    bool contains(point_2t<T> p); // OK
    bool insert(point_2t<T> p); // OK
    vecotor<point_2t<T> > getPoints(aabb<T> box, double eps=0.0); // OK
    vecotor<point_2t<T> > getPoints(point_2t<T> leftUp, point_2t<T> rightDown, double eps=0.0); // OK


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

        return contains_impl(p, curlevel, trees[curlevel]->c);
    }

    bool insert(point_2t<T> p)
    {
        int curlevel = trees.size()-1;
        if(curlevel < 0)
            return false;

        bool flag = insert_impl(p, curlevel, trees[curlevel]->c);
        if(r()>0)
            push(p);
        return flag;
    }

    vecotor<point_2t<T> > getPoints(aabb<T> box, double eps=0.0)
    {
        int level = trees.size()-1;
        if(level<0)
            return vecotor<point_2t<T> >;
        SkipNode<T> * node = tree[level];
        return getPoints_impl(box, eps, level, node);
    }

    vecotor<point_2t<T> > getPoints(point_2t<T> leftUp, point_2t<T> rightDown, double eps=0.0)
    {
        point_2t<T> dim = point_2t(rightDown.x-leftUp.x, leftUp.y - rightDown.y);
        point_2t<T> pos = point_2t(leftUp.x, leftUp.y-dim.y);
        aabb<T> box(pos, dim);
        return getPoints(box, eps);
    }
};
