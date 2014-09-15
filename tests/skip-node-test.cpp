#include <gtest/gtest.h>

#include <boost/assign/list_of.hpp>
#include <misc/random_utils.h>

#include <skipNode.h>

#include <vector>
#include <cg/primitives/point.h>
#include <cg/io/point.h>

#include <iostream>
using namespace std;


using cg::point_2t;
using cg::point_2;


TEST(skip_node_contains, random_point_2ts)
{

    vector<point_2 > point_2ts;
    point_2ts.push_back(point_2t<double>(2,2));

    aabb<double> a(point_2t<double>(0,0), point_2t<double>(256, 256));
    SkipNode<double> root(a, point_2t<double>(2,2));

    cout<<point_2ts[0]<<endl;
    EXPECT_TRUE(root.contains(point_2ts[0]));

    using util::uniform_random_real;
    uniform_random_real<double> r((double) 0.0, (double)200.0);

    const int MAX = 10000;

    for(int i=0; i<MAX; ++i)
    {
        point_2 p(r(), r());
        point_2ts.push_back(p);
        root.insert(p);
    }

    // existing point_2ts
    for(int i=0; i<point_2ts.size(); ++i)
    {
        EXPECT_TRUE(root.contains(point_2ts[i]));
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
            EXPECT_FALSE(root.contains(p));
        else
            EXPECT_TRUE(root.contains(p));

    }
}


