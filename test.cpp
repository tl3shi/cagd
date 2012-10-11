#include <iostream>
#include "CP_PointVector.cpp"
using namespace std;

int main()
{

    CP_Vector3D p0(4.5, 3.75, 3),p1(-2, -1, 0), px; 
    px = p0 ^ p1;
    cout << "x=" << px.m_x << ", y=" << px.m_y << ", z=" << px.m_z << endl;
    px = 2 * px;
    cout << "x=" << px.m_x << ", y=" << px.m_y << ", z=" << px.m_z << endl;
    return 0;
}
