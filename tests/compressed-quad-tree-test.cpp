#include <gtest/gtest.h>

#include <boost/assign/list_of.hpp>
#include <misc/random_utils.h>
#include <cg/io/point.h>

#include <compressedNode.h>

#include <vector>

#include <iostream>
using namespace std;


TEST(compressed_contains, random_point_2ts)
{

    vector<point_2t<double> > point_2ts;
    point_2ts.push_back(point_2t<double>(2,2));

    aabb<double> a(point_2t<double>(0,0), point_2t<double>(256, 256));
    CompressedNode<double> root(a, point_2t<double>(2,2));

    cout<<point_2ts[0]<<endl;
    EXPECT_TRUE(root.contains(point_2ts[0]));

    using util::uniform_random_real;
    uniform_random_real<double> r((double)0.0, (double)200.0);


    const int MAX = 10000;

    for(int i=0; i<MAX; ++i)
    {
        point_2t<double> p(r(), r());
        point_2ts.push_back(p);
        root.insert(p);
    }

    // existing point_2ts
    for(int i=0; i<point_2ts.size(); ++i)
    {
        //cout<<point_2ts[i]<<endl;
        EXPECT_TRUE(root.contains(point_2ts[i]));
    }


    // random point_2ts
    for(int i=0; i<MAX; ++i)
    {
        point_2t<double> p(r(), r());
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


