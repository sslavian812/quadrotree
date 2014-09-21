#include <QColor>
#include <QApplication>

#include <boost/optional.hpp>

#include <cg/visualization/viewer_adapter.h>
#include <cg/visualization/draw_util.h>
#include <cg/io/point.h>
#include <cg/primitives/point.h>


#include "skip-quad-tree.h"


using cg::point_2;
using cg::point_2f;

struct skt_viewer : cg::visualization::viewer_adapter
{
   skt_viewer()
   {
       tree = SkipQuadTree<double>(2048);
       pts =0;
       level = -1;
       insertmode = true;
       eps = 0.001;
       query = aabb<double>(0);
   }

   void draw_rect(aabb<double> box, cg::visualization::drawer_type & drawer, QColor color = Qt::white) const
   {
       drawer.set_color(color);

       point_2 p = box.position;
       double dx = box.dimension.x;
       double dy = box.dimension.y;

       point_2 p1(p.x+dx, p.y+dy);
       point_2 p2(p.x,   p.y+dy);
       point_2 p3(p.x,   p.y);
       point_2 p4(p.x+dx, p.y);

       drawer.draw_line(p1, p2);
       drawer.draw_line(p2, p3);
       drawer.draw_line(p3, p4);
       drawer.draw_line(p4, p1);
   }


   void draw_cross(aabb<double> box, cg::visualization::drawer_type & drawer, QColor color = Qt::white) const
   {
       drawer.set_color(color);

       point_2 p = box.position;
       double dx = box.dimension.x;
       double dy = box.dimension.y;

       point_2 p1(p.x+dx/2, p.y+dy);
       point_2 p2(p.x,       p.y+dy/2);
       point_2 p3(p.x+dx/2, p.y);
       point_2 p4(p.x+dx,   p.y+dy/2);

       drawer.draw_line(p1, p3);
       drawer.draw_line(p2, p4);
   }

   void draw_node(CompressedNode<double> *node, cg::visualization::drawer_type & drawer) const
   {
        if(node->childrensize)
        {
            draw_cross(node->boundary, drawer);

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
       if(level<0)
       {
           draw_rect(tree.max_area, drawer);
           return;
       }
        draw_rect(tree.trees[level]->boundary, drawer);
        draw_node(tree.trees[level], drawer);

        drawer.set_color(Qt::red);
        for(int i=0; i<to_highlight.size(); ++i)
        {
            drawer.draw_point(to_highlight[i], 4);
        }
        draw_rect(query, drawer, Qt::red);
   }

    // print in the corner
   void print(cg::visualization::printer_type & p) const
   {
      p.corner_stream() << "press mouse rbutton to add point" << cg::visualization::endl
                        << "+ - to change view level. current level is: "<< level << cg::visualization::endl
                        << (insertmode ? "INSERT" : "QUERY") << " MODE" << cg::visualization::endl
                        << "points: " << pts << cg::visualization::endl;
   }

   bool on_release(const point_2f & p)
   {
       if(insertmode)
           return false;

       //TODO: how to get drawerobject
       //TODO: add getting abb by LU RD

       //aabb<double> rect;
       //rect = rect.getByLURD(pending_point, p);
       //draw_rect(rect, drawer, Qt::blue);


       to_highlight = tree.getPoints(pending_point, p, eps);

       query = query.getByPoints(pending_point, p);
   }

   bool on_press(const cg::point_2f & p)
   {
       if(insertmode)
       {
           if(tree.insert(p))
               pts++;
           if(level<0)
               level = tree.getMaxLevel();
       }
       else
       {
           pending_point = p;
       }

       return true;
   }

   bool on_key(int key) {
       if (key == Qt::Key_Plus) {
           if(tree.getMaxLevel() > level)
           {
               ++level;
               printf("view level changed\n");
           }
           else
           {
               printf("on top already");
           }
       } else if (key == Qt::Key_Minus) {
           if(level > 0)
           {
               --level;
               printf("view level changed\n");
           }
           else
           {
               printf("on bottom already");
           }
       } else if (key == Qt::Key_I) {
           insertmode = true;
           query = aabb<double>(0);
           printf("INSERT MODE\n");
       } else if (key == Qt::Key_Q && level>=0) {
           insertmode = false;
           printf("QUERY MODE\n");
       } else return false;

       return true;
   }

private:

   SkipQuadTree<double> tree;
   int pts;
   int level;
   bool insertmode;
   point_2 pending_point;
   double eps;
   vector<point_2> to_highlight;
   aabb<double> query;
};

int main(int argc, char ** argv)
{
    // TODO: specify canva's size!!
   QApplication app(argc, argv);
   skt_viewer viewer;
   cg::visualization::run_viewer(&viewer, "skip-quad-tree");
}
