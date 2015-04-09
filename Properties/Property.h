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

#ifndef PROPERTY_H__
#define PROPERTY_H__

#include <string>

/**
* Class representing a relation of key = value
*/
class Property {
  
public:  
  /**
  * Contructs a default instance of a property
  */
  Property();
  
  /**
  * Constructs a instance of a property object with an initial
  * key and value
  * @param key The initial key 
  * @param value The inital value
  */
  Property(const std::string& key, const std::string& value);
  
  /**
  * Getter
  * @return The property key
  */
  const std::string& key() const;
  
  /**
  * Setter
  * @param aKey The new property key
  */  
  void key(const std::string& aKey);

  /**
  * Getter
  * @return The property value
  */  
  const std::string& value() const;
  
  /**
  * Setter
  * @param aValue The new property value
  */    
  void value(const std::string& aValue);
  
private:
        
  std::string m_Key;
  std::string m_Value;          
      
};

#endif // PROPERTY_H__
