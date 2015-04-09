
#ifndef RCSC_GEOM_ANGLEDEG_H
#define RCSC_GEOM_ANGLEDEG_H

#include <functional>
#include <iostream>
#include <cmath>
#include <Defines.h>

namespace rcsc {

class AngleDeg {
private:
    double M_degree;

public:

    AngleDeg()
        : M_degree( 0.0 )
      { }
    AngleDeg( const double & deg )
        : M_degree( deg )
      {
          normalize();
      }

    const
    AngleDeg & operator=( const double & deg )
      {
          M_degree = deg;
          return normalize();
      }

private:
    const
    AngleDeg & normalize()
      {
          if ( M_degree < -360.0 || 360.0 < M_degree )
          {
              M_degree = std::fmod( M_degree, 360.0 );
          }

          if ( M_degree < -180.0 )
          {
              M_degree += 360.0;
          }

          if ( M_degree > 180.0 )
          {
              M_degree -= 360.0;
          }

          return *this;
      }

public:
    const
    double & degree() const
      {
          return M_degree;
      }

    double abs() const
      {
          return std::fabs( degree() );
      }

    double radian() const
      {
          return degree() * DEGTORAD;
      }

    AngleDeg operator-() const
      {
          return AngleDeg( - degree() );
      }

    const AngleDeg & operator+=( const AngleDeg & angle )
      {
          M_degree += angle.degree();
          return normalize();
      }

    const AngleDeg & operator+=( const double & deg )
      {
          M_degree += deg;
          return normalize();
      }

    const AngleDeg & operator-=( const AngleDeg & angle )
      {
          M_degree -= angle.degree();
          return normalize();
      }

    const AngleDeg & operator-=( const double & deg )
      {
          M_degree -= deg;
          return normalize();
      }

    const AngleDeg & operator*=( const double & scalar )
      {
          M_degree *= scalar;
          return normalize();
      }

    const AngleDeg & operator/=( const double & scalar )
      {
          if ( std::fabs( scalar ) < EPSILON )
          {
              return * this;
          }
          M_degree /= scalar;
          return normalize();
      }

    bool isLeftOf( const AngleDeg & angle ) const
      {
          double diff = angle.degree() - this->degree();
          return ( ( 0.0 < diff && diff < 180.0 )
                   || diff < -180.0 );
      }

    bool isLeftEqualOf( const AngleDeg & angle ) const
      {
          double diff = angle.degree() - this->degree();
          return ( ( 0.0 <= diff && diff < 180.0 )
                   || diff < -180.0 );
      }

    bool isRightOf( const AngleDeg & angle ) const
      {
          double diff = this->degree() - angle.degree();
          return ( ( 0.0 < diff && diff < 180.0 )
                   || diff < -180.0 );
      }

    bool isRightEqualOf( const AngleDeg & angle ) const
      {
          double diff = this->degree() - angle.degree();
          return ( ( 0.0 <= diff && diff < 180.0 )
                   || diff < -180.0 );
      }

    double cos() const
      {
          return std::cos( degree() * DEGTORAD );
      }

    double sin() const
      {
          return std::sin( degree() * DEGTORAD );
      }

    double tan() const
      {
          return std::tan( degree() * DEGTORAD );
      }

    bool isWithin( const AngleDeg & left,
                   const AngleDeg & right ) const;

    void sinMinMax( const double & angle_err,
                    double * minsin,
                    double * maxsin ) const;

    void cosMinMax( const double & angle_err,
                    double * mincos,
                     double * maxcos ) const;

    inline static double normalize_angle( double dir )
      {
          if ( dir < -360.0 || 360.0 < dir )
          {
              dir = std::fmod( dir, 360.0 );
          }

          if ( dir < -180.0 )
          {
              dir += 360.0;
          }

          if ( dir > 180.0 )
          {
              dir -= 360.0;
          }

          return dir;
      }

    inline static double rad2deg( const double & rad )
      {
          return rad * RADTODEG;
      }

    inline static double deg2rad( const double & deg )
      {
          return deg * DEGTORAD;
      }

    inline static double cos_deg( const double & deg )
      {
          return std::cos( deg2rad( deg ) );
      }

    inline static double sin_deg( const double & deg )
      {
          return std::sin( deg2rad( deg ) );
      }

    inline static double tan_deg( const double & deg )
      {
          return std::tan( deg2rad( deg ) );
      }

    inline static double acos_deg( const double & cosine )
      {
          return ( cosine >= 1.0
                   ? 0.0
                   : ( cosine <= -1.0
                       ? 180.0
                       : rad2deg( std::acos( cosine ) ) ) );
      }

    inline static double asin_deg( const double & sine )
      {
          return ( sine >= 1.0
                   ? 90.0
                   : ( sine <= -1.0
                       ? -90.0
                       : rad2deg( std::asin( sine ) ) ) );
      }

    inline static double atan_deg( const double & tangent )
      {
          return rad2deg( std::atan( tangent ) );
      }

    inline static double atan2_deg( const double & y, const double & x )
      {
          return ( ( x == 0.0 && y == 0.0 )
                   ? 0.0
                   : rad2deg( std::atan2( y, x ) ) );
      }

    static AngleDeg bisect( const AngleDeg & left, const AngleDeg & right );

    std::ostream & print( std::ostream & os ) const
      {
          return os << degree();
      }

    std::ostream & printRound( std::ostream & os, const double & step = 0.1 ) const
      {
          return os << rint( degree() / step ) * step;
      }

    class DegreeCmp : public std::binary_function< AngleDeg, AngleDeg, bool > {
    public:
        result_type operator()( const first_argument_type & lhs,
                                const second_argument_type & rhs ) const
          {
              return lhs.degree() < rhs.degree();
          }
    };

};

class AngleIsWithin
    : public std::unary_function< AngleDeg, bool > {
    const AngleDeg M_left; //!< left threshold
    const AngleDeg M_right; //!< right threshold
public:
    AngleIsWithin( const AngleDeg & left, const AngleDeg & right )
        : M_left( left )
        , M_right( right )
      { }

    result_type operator()( const argument_type & angle ) const
      {
          return angle.isWithin( M_left, M_right );
      }
};


}

inline const rcsc::AngleDeg
operator+( const rcsc::AngleDeg & lhs,
           const rcsc::AngleDeg & rhs )
{
    return rcsc::AngleDeg( lhs ) += rhs;
}

inline const rcsc::AngleDeg operator+( const rcsc::AngleDeg & lhs, const double & rhs )
{
    return rcsc::AngleDeg( lhs ) += rhs;
}

inline const rcsc::AngleDeg operator+( const double & lhs, const rcsc::AngleDeg & rhs )
{
    return rcsc::AngleDeg( rhs ) += lhs;
}

inline const rcsc::AngleDeg operator-( const rcsc::AngleDeg & lhs, const rcsc::AngleDeg & rhs )
{
    return rcsc::AngleDeg( lhs ) -= rhs;
}

inline const rcsc::AngleDeg operator-( const rcsc::AngleDeg & lhs, const double & rhs )
{
    return rcsc::AngleDeg( lhs ) -= rhs;
}

inline const rcsc::AngleDeg operator-( const double & lhs, const rcsc::AngleDeg & rhs )
{
    return rcsc::AngleDeg( lhs ) -= rhs;
}

inline bool operator!=( const rcsc::AngleDeg & lhs, const rcsc::AngleDeg & rhs )
{
    return std::fabs( lhs.degree() - rhs.degree() ) > EPSILON;
}

inline bool operator!=( const rcsc::AngleDeg & lhs, const double & rhs )
{
    return std::fabs( lhs.degree() - rhs ) > EPSILON;
}

inline bool operator!=( const double & lhs, const rcsc::AngleDeg & rhs )
{
    return std::fabs( lhs - rhs.degree() ) > EPSILON;
}

inline bool operator==( const rcsc::AngleDeg & lhs, const rcsc::AngleDeg & rhs )
{
    return std::fabs( lhs.degree() - rhs.degree() ) < EPSILON;
}

inline bool operator==( const rcsc::AngleDeg& lhs, const double& rhs )
{
    return std::fabs( lhs.degree() - rhs ) < EPSILON;
}

inline bool operator==( const double & lhs, const rcsc::AngleDeg & rhs )
{
    return std::fabs( lhs - rhs.degree() ) < EPSILON;
}

inline std::ostream & operator<<( std::ostream & os, const rcsc::AngleDeg & a )
{
    return a.print( os );
}

#endif
