#include <QColor>
#include <QApplication>

#include <boost/optional.hpp>

#include <cg/visualization/viewer_adapter.h>
#include <cg/visualization/draw_util.h>

#include <cg/io/point.h>

#include <cg/primitives/point.h>


#include "skip-node.h"


using cg::point_2;
using cg::point_2f;

struct qdt_viewer : cg::visualization::viewer_adapter
{
   qdt_viewer()
   {
    root = new SkipNode<double>();
    pts =0;
   }

   void draw_node(SkipNode<double> *node, cg::visualization::drawer_type & drawer) const
   {

    drawer.set_color(Qt::green);
    for (int i=0; i < node->points.size(); ++i)
        {
            point_2 p(node->points[i].x, node->points[i].y);
            drawer.draw_point(p, 3);
        }

    if(node->points.size()==0)
    {

        drawer.set_color(Qt::white);

        point_2 p1(node->boundary.center.x+node->boundary.dimension.x, node->boundary.center.y);
        point_2 p2(node->boundary.center.x, node->boundary.center.y+node->boundary.dimension.y);
        point_2 p3(node->boundary.center.x-node->boundary.dimension.x, node->boundary.center.y);
        point_2 p4(node->boundary.center.x, node->boundary.center.y-node->boundary.dimension.y);

        drawer.draw_line(p1, p3);
        drawer.draw_line(p2, p4);

        for(int i=0; i<4; ++i)
        {
            if(node->children[i]!= NULL)
                draw_node(node->children[i], drawer);
        }

    }
   }

    // draws all the points and lines
   void draw(cg::visualization::drawer_type & drawer) const
   {
      draw_node(root, drawer);
   }

    // print in the corner
   void print(cg::visualization::printer_type & p) const
   {
      p.corner_stream() << "press mouse rbutton to add point" << cg::visualization::endl
                        << "points: " << pts << cg::visualization::endl;
   }


    // mouse rbutton click
   bool on_release(const point_2f & p)
   {
     root->insert(Point<double>(p.x, p.y));
     pts++;

      return true;
   }

private:

   SkipNode<double>* root;
   int pts;
};

int main(int argc, char ** argv)
{
    // TODO: specify canva's size!!
   QApplication app(argc, argv);
   qdt_viewer viewer;
   cg::visualization::run_viewer(&viewer, "skip-quad-tree");
}
