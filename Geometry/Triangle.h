#include <FullLine.h>

class Triangle
{
private:
    
    Point A,B,C;

public:
    
    //Triangle(Point x,Point y,Point z);
    void setByPoints(Point x,Point y,Point z);
    bool checkTriangle(Point x);
    Point getTriA();
    Point getTriB();
    Point getTriC();
};
