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

#ifndef PROPERTY_READER_H__
#define PROPERTY_READER_H__

#include <iostream>
#include <string>

#include "Properties.h"

class PropertyReader {

public:
       
  Properties read(std::istream& os);
  Properties read(const std::string& filename);  
  
  void write(std::ostream& os, const std::string& message, const Properties& props) const;
  void write(std::ostream& os, Properties& props) const;

  void write(const std::string& filename, const std::string& message, const Properties& props) const;
  void write(const std::string& filename, const Properties& props) const;
      
};

#endif //  PROPERTY_READER_H__
