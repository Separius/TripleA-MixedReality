/*
 * Written by Jose Grimaldo da Silva <jose.jgrimaldo@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */


#ifndef UTIL_H_
#define UTIL_H_

#include <iostream>
#include <string>
#include <sstream>
#include <typeinfo>

#include <SDL/SDL_net.h>
#include <cmath>


/**
 * \brief Class badParameter
 *
 * Provides methods and attributes to handle with Soccer util.
 * related ODE stuff.
 */
class BadParameter : public std::exception {

public:
  /**
   * \brief Constructor 1.
   *
   * Do not receives parameters.
   */
  BadParameter() {
  }
  /**
   * \brief Constructor 2.
   *
   * Receives cause.
   *
   * @param cause an ODE pointer type string.
   */
  BadParameter(const std::string& cause)
    : m_What(cause)
  {
  }

  virtual const char * what() const throw()
  {
    return m_What.c_str();
  }

  /**
  * \brief Empty destructor.
  */
  virtual ~BadParameter() throw() {
  }

private:
    /**
     * \brief Stores error cause.
     */
	std::string m_What;

};

/**
 * \brief Convert values.
 */
template <typename TT, typename ST>
TT convert_to(const ST& source)
{
  std::ostringstream os;
  os << source;

  if (!os)  {
    throw BadParameter(std::string("Error converting ") + os.str()) ;
  }

  TT target;

  std::istringstream is(os.str());
  is >> target;

  if (!is) {
    throw BadParameter(std::string("Error converting ") + os.str() + " to "  + typeid(target).name());
  }

  return target;
}

/**
 * \brief Convert radians to degrees.
 *
 * @param radians: value in radians to convert.
 */
inline double ToDegrees(double radians)
{
	return (180 * radians) / M_PI;
}

/**
 * \brief Convert degrees to radians.
 * @param degree: value in degrees to convert.
 */
inline double ToRadians(double degree)
{
	return (M_PI * degree) / 180;
}


/**
 *
 * Utility function wrapping the action of:
 *
 *     	std::string request((char *) &packet->data[0]
 *          ,(char *)(&packet->data[0] + MyPacket->len));
 *
 **/
std::string ConvertSdlDataBuffer(UDPpacket * packet);

template <typename T>
std::string format_number(T& value, int width, int precision)
{
	std::ostringstream os;

	os.setf(std::ios_base::fixed);
	os.width(width);
	os.precision(precision);
	os << value;

	return os.str();
}

/*
int main(void)
{
  try {
    int i = convert_to<int>("3013");
    std::cout << i << "\n";

    double f = convert_to<double>("a442.3");
    std::cout << f << "\n";

  } catch (BadParameter& ex) {
    std::cout << ex.what() << "\n";
    return 1;
  }

  return 0;
}
*/

#endif /*UTIL_H_*/

