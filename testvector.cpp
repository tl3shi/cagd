#include <iostream>
#include <vector>
using namespace std;
int main()
{
    vector<int> p1,p2;
    p1.push_back(1);
    p1.push_back(2);
    p1.push_back(3);
    p2 = p1;
    cout << p2[2] << endl;
    p2[2] = 45;
    cout << p1[2] << endl;
    return 0;
}
