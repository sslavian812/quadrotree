#include <gtest/gtest.h>

#include <boost/assign/list_of.hpp>
#include <misc/random_utils.h>

#include <node.h>
#include <vector>


TEST(simple_contains, manual_points)
{
    vector<Point<double> > rpoints;
    vector<Point<double> > points;
    Node<double> root;

    using util::uniform_random_real;
    uniform_random_real<double> r((double)0, (double)200.0);

    const int MAX = 10000;

    for(int i=0; i<MAX; ++i)
    {
        Point<double> p(r(), r());
        rpoints.push_back(p);
        root.insert(p);
    }

    for(int i=0; i<MAX; ++i)
    {
        Point<double> p(-r(), -r());
        points.push_back(p);
        root.insert(p);
    }

    bool flag = false;

    // existing points

    vectort<Point<double> > rect = root.getPoints(AABB<double>(Point<double>(100, 100), Point<double>(100, 100)));

    for(int i=0; i<rpoints.size(); ++i)
    {
        EXPECT_TRUE(root.contains(points[i]));
    }





}
