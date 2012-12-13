#include "stdafx.h"

class MonotoneTriangulation
{

public:

    static void Triangulate(
        vector<int> &triangle_point_indexes,
        const vector<Point> &contour_points,
        const vector<int> &mono_poly)
    {
        queue<Point> priority_points;
        int mono_poly_point_number = mono_poly->Count;
        for (int i = 0; i < mono_poly_point_number; i++)
        {
            Point current_point = contour_points[(*mono_poly)[i]];
            Point next_point = contour_points[(*mono_poly)[(i + 1) % mono_poly_point_number]];
            current_point.IsLeftChain = (current_point > next_point) ? true : false;
            priority_points.Enqueue(current_point);
        }

        stack<Point> current_triangle_points;
        for (int i = 0; i < 2; i++)
            current_triangle_points.Push(priority_points.Dequeue());

        while (priority_points.Count > 1)
        {
            Point top_priority_point = priority_points.Dequeue();
            Point top_current_triangle_point = current_triangle_points.Peek();

            if (top_priority_point.IsLeftChain != top_current_triangle_point.IsLeftChain)
            {
                while (current_triangle_points.Count > 1)
                {
                    Point current_top_point = current_triangle_points.Pop();
                    Point current_second_point = current_triangle_points.Peek();

                    if (same_direction_triangle 
                        && TriangulationUtility::GetTriangleOrientation(
                        top_priority_point.Position, 
                        current_top_point.Position, 
                        current_second_point.Position) < 0.0)
                    {
                        triangle_point_indexes.Add(top_priority_point.Index);
                        triangle_point_indexes.Add(current_top_point.Index);
                        triangle_point_indexes.Add(current_second_point.Index);
                    }
                    else
                    {
                        triangle_point_indexes.Add(top_priority_point.Index);
                        triangle_point_indexes.Add(current_second_point.Index);
                        triangle_point_indexes.Add(current_top_point.Index);
                    }
                }
                current_triangle_points.Pop();
                current_triangle_points.Push(top_current_triangle_point);
                current_triangle_points.Push(top_priority_point);
            }
            else
            {
                while (current_triangle_points.Count > 1)
                {
                    Point current_top_point = current_triangle_points.Pop();
                    Point current_second_point = current_triangle_points.Peek();
                    current_triangle_points.Push(current_top_point);

                    double first_point_coordinates[2], second_point_coordinates[2], third_point_coordinates[2];
                    first_point_coordinates[0] = top_priority_point.X;
                    first_point_coordinates[1] = top_priority_point.Y;
                    second_point_coordinates[0] = current_second_point.X;
                    second_point_coordinates[1] = current_second_point.Y;
                    third_point_coordinates[0] = current_top_point.X;
                    third_point_coordinates[1] = current_top_point.Y;

                    double area = Computational::External::RobustPredicates<real>::Instance.orient2d(
                        first_point_coordinates, second_point_coordinates, third_point_coordinates);
                    if ((area > 0.0 && current_top_point.IsLeftChain) || (area < 0 && !current_top_point.IsLeftChain))
                    {

                        triangle_point_indexes.Add(top_priority_point.Index);
                        triangle_point_indexes.Add(current_top_point.Index);
                        triangle_point_indexes.Add(current_second_point.Index);

                        current_triangle_points.Pop();
                    }
                    else
                        break;
                }
                current_triangle_points.Push(top_priority_point); 
            }
        }

        Point last_priority_point = priority_points.Dequeue();
        while (current_triangle_points.Count != 1)
        {
            Point current_top_point = current_triangle_points.Pop();
            Point current_second_point = current_triangle_points.Peek();


            triangle_point_indexes.Add(last_priority_point.Index);
            triangle_point_indexes.Add(current_top_point.Index);
            triangle_point_indexes.Add(current_second_point.Index);

        }
    }
};