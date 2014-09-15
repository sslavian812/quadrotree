//#include <gtest/gtest.h>

//#include <boost/assign/list_of.hpp>
//#include <misc/random_utils.h>
//#include <cg/primitives/point.h>
//#include <skip-quad-tree.h>
//#include <vector>

//using std::vector;

//using cg::point_2;
//using cg::point_2t;

//TEST(simple_contains, manual_points)
//{
//    vector<point_2 > rpoints;
//    vector<point_2 > points;

//    SkipQuadTree tree(256);



//    using util::uniform_random_real;
//    uniform_random_real<double> r((double)0, (double)250.0);

//    const int MAX = 10000;

//    for(int i=0; i<MAX; ++i)
//    {
//        point_2 p(r(), r());
//        points.push_back(p);
//        root.insert(p);
//    }

//    // заполнили как-нибудь дерево




//    vectort<point_2 > rect = root.getPoints(AABB<double>(point_2(100, 100), point_2(100, 100)));

//    for(int i=0; i<rpoints.size(); ++i)
//    {
//        EXPECT_TRUE(root.contains(points[i]));
//    }





//}
