#include <gtest/gtest.h>

#include <boost/assign/list_of.hpp>
#include <misc/random_utils.h>

#include <skip-quad-tree.h>

#include <vector>
#include <cg/primitives/point.h>
#include <cg/io/point.h>

#include <iostream>
using namespace std;


using cg::point_2t;
using cg::point_2;


TEST(skip_tree_contains, random_point_2ts)
{

    vector<point_2 > point_2ts;

    SkipQuadTree<double> tree(256);
    point_2t<double> p(2,2);

//    point_2ts.push_back(p);
//    cout<<"insering: "<<p<<endl;
//    tree.insert(p);

//    cout<<p<<endl;
//    EXPECT_TRUE(tree.contains(p));


    using util::uniform_random_real;
    uniform_random_real<double> r((double) 0.0, (double)200.0);

    const int MAX = 10000;

    for(int i=0; i<MAX; ++i)
    {
        point_2 p(r(), r());
        point_2ts.push_back(p);
       // cout<<"insering: "<<p<<endl;
        tree.insert(p);
    }

    // existing point_2ts
    for(int i=0; i<point_2ts.size(); ++i)
    {
        EXPECT_TRUE(tree.contains(point_2ts[i]));
    }


    // random point_2ts
    for(int i=0; i<MAX; ++i)
    {
        point_2 p(r(), r());
        bool exists = false;
        for(int j=0; j<point_2ts.size(); ++j)
        {
            if(point_2ts[i] == p)
            {
                exists = true;
                break;
            }
        }

        if(!exists)
            EXPECT_FALSE(tree.contains(p));
        else
            EXPECT_TRUE(tree.contains(p));

    }
}


