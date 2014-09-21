#include <gtest/gtest.h>

#include <boost/assign/list_of.hpp>
#include <misc/random_utils.h>
#include <cg/primitives/point.h>
#include <skip-quad-tree.h>
#include <vector>

using std::vector;

using cg::point_2;
using cg::point_2t;

const double eps = 0.1;

TEST(simple_contains, random_points)
{
    vector<point_2 > points;
    SkipQuadTree<double> tree(256);

    using util::uniform_random_real;
    uniform_random_real<double> r((double)0, (double)250.0);

    const int MAX = 10000;

    for(int i=0; i<MAX; ++i)
    {
        point_2 p(r(), r());
        points.push_back(p);
        tree.insert(p);
    }

    // заполнили как-нибудь дерево


    const int MAX_RECTS = 100;

    for(int k=0; k<MAX_RECTS; ++k)
    {
        aabb<double> rect;
        rect = rect.getByPoints(point_2(r(), r()), point_2(r(), r()));

        vector<point_2 > in_rect = tree.getPoints(rect, eps);

        for(int i=0; i<in_rect.size(); ++i)
        {
            EXPECT_TRUE((rect+eps).contains(in_rect[i]));
        }
    }
}
