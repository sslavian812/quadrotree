//#include<vector>
//#include "point.h"
//#include "aabb.h"

//using std::vector;


//template <class T>
//struct SkipNode
//{
//    AABB<T> boundary;
//    vector<Point<T> > points;
//    SkipNode* children[4];
//    int childrensize;

//    SkipNode* backShortCut;
//    SkipNode* parent;
//    SkipNode* down;


//    const int SIZE = 1;

//    SkipNode(AABB<T> box, SkipNode* parent = NULL)
//    {
//        boundary = box;
//        children[0]=children[1]=children[2]=children[3]=NULL;
//        childrensize =0;
//        this->parent=backShortCut=parent;
//        down=NULL;
//    }

//    SkipNode(double side = 1024, SkipNode* parent = NULL)
//    {
//        AABB<T> a;
//        a.dimension.x=a.dimension.y=side;
//        boundary=a;
//        children[0]=children[1]=children[2]=children[3]=NULL;
//        childrensize =0;
//        this->parent=backShortCut=parent;
//        down=NULL;
//    }

//    SkipNode<T>* insert(Point<T> p)
//    {
//        //TODO: here I supoose having only one point per node!!!
//        // does not generalize on more points in a node!!!

//        if(!boundary.contains(p))
//            return NULL;

//        // в какую часть текущего узла попадаем
//        int part = boundary.whatPart(p);

//        // если лист и есть место
//        if(childrensize==0 && points.size() < SIZE)
//        {
//            points.push_back(p);
//            return this;
//        }


//        // лист, но места нет
//        if(childrensize==0)
//        {
//            Point<T> p2=points[0];
//            if(p2 == p)
//                return this; // вставлять не будем, но сделаем вид, что вставили.
//                            // если возвращать NULL, то может кое-то сломаться в скипдереве
//            int part2 = boundary.whatPart(p2);

//            if(part != part2)
//            {
//                children[part]=new SkipNode(boundary.getPart(part), this);
//                ++childrensize;
//                children[part]->insert(p);

//                children[part2]=new SkipNode(boundary.getPart(part2), this);
//                ++childrensize;
//                children[part2]->insert(p2);
//                points.resize(0);
//                return this;
//            }
//            else
//            {
//                points.resize(0);
//                children[part]=new SkipNode(boundary.getPart(part), this);
//                ++childrensize;
//                children[part]->insert(p2);

//                children[part]=children[part]->insert(p);
//                children[part]->backShortCut = this;
//                return children[part];
//            }
//        }

//        // not a leave

//        if(children[part] == NULL)
//        {
//            children[part] =new SkipNode(boundary.getPart(part), this);
//            ++childrensize;
//            children[part]->insert(p);
//            return this;
//        }


//        // not a leave and nesessary part is not empty
//        if(children[part]!=NULL)
//        {
//            if(children[part]->boundary.contains(p))
//            {
//                // ребенок нормального или достаточного размера
//                children[part]=children[part]->insert(p);
//                //children[part]->insert(p); // добавление точки ноду не убивает. неправда: убивает, но не так.

//                //children[part]->backShortCut = this;

//                // скорее всего этот иф никогжда не выполняется, но не факт.
//                if(childrensize == 1)
//                    return children[part];
//                else
//                    return this;
//            }
//            else
//            {
//                // ребенок урезан!
//                SkipNode *child = children[part]; // урезаный ребенок
//                SkipNode *i = child->parent;   // его родитель
//                while(!i->boundary.contains(p))      // ближайший, который покроет обоих
//                    i = i->parent;


//                children[part] = i;   // переброс шортката
//                children[part]->backShortCut = this;
//                        // куда попадет обрезанный
//                i->children[i->boundary.whatPart(child->boundary.center)]=child;
//                child->backShortCut = i;

//                    // куда попадет точка в "ближайшем покрывающем"
//                int ipart = (i->boundary).whatPart(p);
//                (i->children)[ipart]= new SkipNode((i->boundary).getPart(ipart), i);
//                (i->childrensize)++;
//                (i->children)[ipart] = (i->children[ipart])->insert(p);
//                (i->children)[ipart]->backShortCut = i;
//                return this;
//            }

//        }

//        return NULL;  // never reached!!
//    }

//    // ликализует внутреннюю вершину, к которой можно подвесить данную точку как лист
//    SkipNode<T>* find(Point<T> p)
//    {
//        if(!boundary.contains(p))
//            return NULL;

//        for(int i=0; i<points.size(); ++i)
//        {
//            if(p == points[i])
//                return this; // вернем лист (в том числе и корень-лист)
//        }

//        for(int i=0; i<4; ++i)
//        {
//            if(children[i]!= NULL && children[i]->boundary->contains(p))
//                return children[i]->find(p);
//        }

//        return this;  // квадрат, который пересекает точку, но сам её не содержит.
//    }

//    bool contains(Point<T> p)
//    {
//        if(!boundary.contains(p))
//            return false;

//        for(int i=0; i<points.size(); ++i)
//        {
//            if(p == points[i])
//                return true;
//        }

//        for(int i=0; i<4; ++i)
//        {

//            if(children[i]!= NULL && children[i]->contains(p))
//                return true;
//        }
//        return false;
//    }
//};
