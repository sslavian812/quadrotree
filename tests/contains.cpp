#include <gtest/gtest.h>

#include <boost/assign/list_of.hpp>
#include <misc/random_utils.h>

#include <node.h>

#include <vector>


TEST(contains, manual_points)
{
    Node<double> root;

    // 0th
    root.insert(Point<double>(1,1));
    root.insert(Point<double>(2,2));
    root.insert(Point<double>(1,3));
    root.insert(Point<double>(4,1));
    root.insert(Point<double>(5,5));

    EXPECT_TRUE(root.contains(Point<double>(1,1)));
    EXPECT_TRUE(root.contains(Point<double>(2,2)));
    EXPECT_TRUE(root.contains(Point<double>(1,3)));
    EXPECT_TRUE(root.contains(Point<double>(4,1)));
    EXPECT_TRUE(root.contains(Point<double>(5,5)));



    // 1th
    root.insert(Point<double>(-1,1));
    root.insert(Point<double>(-2,2));
    root.insert(Point<double>(-1,3));
    root.insert(Point<double>(-4,1));
    root.insert(Point<double>(-5,5));

    EXPECT_TRUE(root.contains(Point<double>(-1,1)));
    EXPECT_TRUE(root.contains(Point<double>(-2,2)));
    EXPECT_TRUE(root.contains(Point<double>(-1,3)));
    EXPECT_TRUE(root.contains(Point<double>(-4,1)));
    EXPECT_TRUE(root.contains(Point<double>(-5,5)));


    // 2th
    root.insert(Point<double>(-1,-1));
    root.insert(Point<double>(-2,-2));
    root.insert(Point<double>(-1,-3));
    root.insert(Point<double>(-4,-1));
    root.insert(Point<double>(-5,-5));

    EXPECT_TRUE(root.contains(Point<double>(-1,-1)));
    EXPECT_TRUE(root.contains(Point<double>(-2,-2)));
    EXPECT_TRUE(root.contains(Point<double>(-1,-3)));
    EXPECT_TRUE(root.contains(Point<double>(-4,-1)));
    EXPECT_TRUE(root.contains(Point<double>(-5,-5)));


    // 3th
    root.insert(Point<double>(1,-1));
    root.insert(Point<double>(2,-2));
    root.insert(Point<double>(1,-3));
    root.insert(Point<double>(4,-1));
    root.insert(Point<double>(5,-5));

    EXPECT_TRUE(root.contains(Point<double>(1,-1)));
    EXPECT_TRUE(root.contains(Point<double>(2,-2)));
    EXPECT_TRUE(root.contains(Point<double>(1,-3)));
    EXPECT_TRUE(root.contains(Point<double>(4,-1)));
    EXPECT_TRUE(root.contains(Point<double>(5,-5)));
}

TEST(contains, random_points)
{

    vector<Point<double> > points;
    Node<double> root;


    using util::uniform_random_real;
    uniform_random_real<double> r((double)-200.0, (double)200.0);

    const int MAX = 1000;

    for(int i=0; i<MAX; ++i)
    {
        Point<double> p(r(), r());
        points.push_back(p);
        root.insert(p);
    }

    // existing points
    for(int i=0; i<points.size(); ++i)
    {
        EXPECT_TRUE(root.contains(points[i]));
    }


    // random points
    for(int i=0; i<MAX; ++i)
    {
        Point<double> p(r(), r());
        bool exists = false;
        for(int j=0; j<points.size(); ++j)
        {
            if(points[i] == p)
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


