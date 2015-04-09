#include <Triangle.h>
#include <Vector.h>

/*Triangle :: Triangle(Point x ,Point y ,Point z )
{
    A = x;
    B = y;
    C = z;
}*/
void Triangle :: setByPoints(Point x ,Point y ,Point z )
{
    A = x;
    B = y;
    C = z;
}
Point Triangle :: getTriA()
{
    return A ;
}
Point Triangle :: getTriB()
{
    return B ;
}
Point Triangle :: getTriC()
{
    return C ;
}
bool Triangle :: checkTriangle(Point x)
{
    float sum ;
    Vector v1,v2,v3 ;
    v1.setByPoints(x ,A);
    v2.setByPoints(x ,B);
    v3.setByPoints(x ,C);
    sum = v1.getSumAngles(v1,v2,v3);
    if ( (sum > 355) && (sum < 365) )
	return ( true );
    else
	return ( false );
}