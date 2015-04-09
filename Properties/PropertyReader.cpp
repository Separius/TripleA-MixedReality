/*
 * Written by José Grimaldo da Silva <jose.jgrimaldo@gmail.com>
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

#include <fstream>
#include <sstream>

#include "PropertyReader.h"

#include "BadPropertyFormatException.h"
#include "PropertyStreamError.h"

Properties PropertyReader::read(std::istream& is)
{
  Properties props;
  std::string line;

  while (std::getline(is, line))
  {
    // Skip a empty line
    if (line == "") continue;    

    // Skip a comment-line
    if (line[0] == '#') continue;

    std::string::size_type separator = line.find('=');
    
    if (separator == std::string::npos)
    {
     // There is no equal sign
      throw BadPropertyFormatException();
    }
    
    std::string lhs = line.substr(0, separator);
    std::string rhs = line.substr(separator + 1);

    // Left hand side is empty
    // i.e.: =value
    if (lhs.size() == 0) throw BadPropertyFormatException();

    props.setProperty(lhs, rhs); 
  }

  if (is.bad())
  {
    throw PropertyStreamError();
  }

  return props;
}

Properties PropertyReader::read(const std::string& filename)
{
  std::ifstream inputStream;
  inputStream.open(filename.c_str());
  
  if (!inputStream)
    throw PropertyStreamError();

  return read(inputStream);
}

void PropertyReader::write(std::ostream& os, const std::string& message, const Properties& props) const
{
  Properties::const_iterator it = props.begin(), end = props.end();
  
  std::istringstream lineStream(message);
  std::string line;

  // print the comments taking in account the end of lines
  while (std::getline(lineStream, line))
  {
    os << "#" << line << "\n";
  }

  for (; it != end; ++it)
  {    
    // Write property on the stream
    os << it->first << "=" << it->second << "\n";
  }

  if (os.bad())
    throw PropertyStreamError();
}
  
void PropertyReader::write(std::ostream& os, Properties& props) const
{
  if (!os)
   throw PropertyStreamError();

  write(os, "", props);
}

void PropertyReader::write(const std::string& filename, const std::string& message, const Properties& props) const
{
  std::ofstream outStream;
  outStream.open(filename.c_str());

  if (!outStream)
   throw PropertyStreamError();

  write(outStream, message, props);
}

void PropertyReader::write(const std::string& filename, const Properties& props) const
{
  write(filename, "", props);
}
