
#include <angle_deg.h>

#include <algorithm>
#include <vector>

#define M_PI 3.14159265358979323846

namespace rcsc {

bool AngleDeg::isWithin( const AngleDeg & left, const AngleDeg & right ) const
{
    // left to right arc angle is less than 180 degree.
    if ( left.isLeftEqualOf( right ) )
    {
        if ( left.isLeftEqualOf( *this ) && this->isLeftEqualOf( right ) )
        {
            return true;
        }
    }
    // arc angle is more than 180 degree.
    else
    {
        // check out reverse side
        //if ( *this <= right || left <= *this )
        // == !(right < *this && *this < left)
        if ( this->isLeftEqualOf( right ) || left.isLeftEqualOf( *this ) )
        {
            return true;
        }
    }
    return false;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
AngleDeg::sinMinMax( const double & angle_err,
                     double * minsin,
                     double * maxsin ) const
{
    if ( angle_err < 0.0 || 180.0 < angle_err )
    {
        std::cerr << "AngleDeg::sinMinMax() invalid error range. "
                  << angle_err << std::endl;
        *minsin = -1.0;
        *maxsin = 1.0;
        return;
    }

    double mindir = this->degree() - angle_err;
    double maxdir = this->degree() + angle_err;

    std::vector< double > sol;
    sol.reserve( 4 );

    if ( ( mindir < -90.0 && -90.0 < maxdir )
         || ( mindir < 270.0 && 270.0 < maxdir )
         )
    {
        sol.push_back( -1.0 );
    }

    if ( ( mindir < 90.0 && 90.0 < maxdir )
         || ( mindir < -270.0 && -270.0 < maxdir )
         )
    {
        sol.push_back( 1.0 );
    }

    sol.push_back( AngleDeg::sin_deg( mindir ) );
    sol.push_back( AngleDeg::sin_deg( maxdir ) );

    *minsin = *std::min_element( sol.begin(), sol.end() );
    *maxsin = *std::max_element( sol.begin(), sol.end() );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
AngleDeg::cosMinMax( const double & angle_err,
                     double * mincos,
                     double * maxcos ) const
{
    if ( angle_err < 0.0 || 180.0 < angle_err )
    {
        std::cerr << "AngleDeg::cosMinMax() invalid error range. "
                  << angle_err << std::endl;
        *mincos = -1.0;
        *maxcos = 1.0;
        return;
    }

    double mindir = this->degree() - angle_err;
    double maxdir = this->degree() + angle_err;

    std::vector< double > sol;
    sol.reserve( 4 );

    if ( mindir < -180.0 && -180.0 < maxdir )
    {
        sol.push_back( -1.0 );
    }

    if ( mindir < 0.0 && 0.0 < maxdir )
    {
        sol.push_back( 1.0 );
    }

    sol.push_back( AngleDeg::cos_deg( mindir ) );
    sol.push_back( AngleDeg::cos_deg( maxdir ) );

    *mincos = *std::min_element( sol.begin(), sol.end() );
    *maxcos = *std::max_element( sol.begin(), sol.end() );
}


/*-------------------------------------------------------------------*/
/*!

 */
AngleDeg
AngleDeg::bisect( const AngleDeg & left,
                  const AngleDeg & right )
{
    AngleDeg result( left );
    AngleDeg rel( right - left );
    double half_deg = rel.degree() * 0.5;
    result += half_deg;

    if ( left.isLeftOf( right ) )
    {
        return result;
    }
    else
    {
        return result += 180.0;
    }
}

} // end of namespace
