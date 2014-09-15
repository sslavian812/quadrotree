#include <QColor>
#include <QApplication>

#include <boost/optional.hpp>

#include <cg/visualization/viewer_adapter.h>
#include <cg/visualization/draw_util.h>

#include <cg/io/point.h>

#include <cg/primitives/point.h>


#include "compressedNode.h"


using cg::point_2;
using cg::point_2f;

struct qdt_viewer : cg::visualization::viewer_adapter
{
   qdt_viewer()
   {
       aabb<double> a(point_2t<double>(0,0), point_2t<double>(256, 256));
       root = new CompressedNode<double>(a, point_2t<double>(0,0));
       pts =1;
   }

   void draw_node(CompressedNode<double> *node, cg::visualization::drawer_type & drawer) const
   {
        if(node->childrensize)
        {
            drawer.set_color(Qt::white);
            point_2 p =node->boundary.position;
            double dx = node->boundary.dimension.x;
            double dy = node->boundary.dimension.y;

            point_2 p1(p.x+dx/2, p.y+dy);
            point_2 p2(p.x,       p.y+dy/2);
            point_2 p3(p.x+dx/2, p.y);
            point_2 p4(p.x+dx,   p.y+dy/2);

            drawer.draw_line(p1, p3);
            drawer.draw_line(p2, p4);

            for(int i=0; i<4; ++i)
            {
                if(node->children[i]!= NULL)
                    draw_node(node->children[i], drawer);
            }
        }
        else
        {
            drawer.set_color(Qt::green);
            drawer.draw_point(node->point, 3);
        }
    }

   // draws all the points and lines
   void draw(cg::visualization::drawer_type & drawer) const
   {
       drawer.set_color(Qt::white);
       point_2 p =root->boundary.position;
       double dx = root->boundary.dimension.x;
       double dy = root->boundary.dimension.y;

       point_2 p1(p.x+dx, p.y+dy);
       point_2 p2(p.x,   p.y+dy);
       point_2 p3(p.x,   p.y);
       point_2 p4(p.x+dx, p.y);

       drawer.draw_line(p1, p2);
       drawer.draw_line(p2, p3);
       drawer.draw_line(p3, p4);
       drawer.draw_line(p4, p1);

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
     void* v= root->insert(point_2t<double>(p.x, p.y));
     if(v != NULL)
        pts++;

      return true;
   }

private:

   CompressedNode<double>* root;
   int pts;
};

int main(int argc, char ** argv)
{
    // TODO: specify canva's size!!
   QApplication app(argc, argv);
   qdt_viewer viewer;
   cg::visualization::run_viewer(&viewer, "compressed-quad-tree");
}
