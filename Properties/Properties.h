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

#ifndef PROPERTIES_H__
#define PROPERTIES_H__

#include <string>
#include <map>

#include "Property.h"

/**
*
* Represents a collection of properties
*
* All operations are guaranteed to have at most an amortized complexity of O(log(n))
*
* Usage:
*
* Properties props;
* props.setProperty("host", "localhost");
*
* if (props.propertyValue("host") == "localhost")
*   std::cout << "host mapped to localhost\n";
*
*/
class Properties {

  typedef std::map<std::string, std::string> property_map_t;

public:

  typedef property_map_t::iterator        iterator;
  typedef property_map_t::const_iterator  const_iterator;

public:

  /**
  *
  *  Returns a iterator to the first position of a list of properties
  *  In these properties the key is mapped as first, and the value as second
  *
  *  Usage: std::cout << it->first << " " << it->second << "\n";
  *
  *  @return A iterator to the beggining of the sequence
  */
  iterator begin();

  /**
  *
  *  Returns a const iterator to the first position of a list of properties
  *  In these properties the key is mapped as first, and the value as second
  *
  *  Usage: std::cout << it->first << " " << it->second << "\n";
  *
  *  @return A const iterator to the beggining of the sequence
  */
  const_iterator begin() const;

 /**
  *
  *  Returns a iterator to the last position of a list of properties
  *
  *  As a tip of advice, this iterator cannot be dereferenced
  *
  *  @return A iterator to the last position of the sequence
  */
  iterator end();

 /**
  *
  *  Returns a const iterator to the last position of a list of properties
  *
  *  As a tip of advice, this iterator cannot be dereferenced
  *
  *  @return A iterator to the last position of the sequence
  */
  const_iterator end() const;

  /**
  * Return the value of the Property with the specified key
  *
  * @depreciated
  *
  * @param The searched property key
  * @return A string with a reference to the property value, if no property
  * with the given key is found, a empty string is returned
  */
  std::string propertyValue(const std::string& key) const;

  /**
  * Return the value of the Property with the specified key
  *
  * This method is the same as "propertyValue(key)" but smaller
  *
  * @param The searched property key
  * @return A string with a reference to the property value, if no property
  * with the given key is found, a empty string is returned
  */
  std::string Get(const std::string& key) const;

  /**
  * Return the Property with the specified key
  *
  * @param The searched property key
  * @return A reference to the property, if no property with the given
  * key is found, a property with a empty string value is returned
  */
  Property property(const std::string& key) const;

  /**
  *
  * Sets a property with the specified key and value, if an already existing
  * property exists with the same key, the old value is overriden
  *
  * Notice that the old value is not returned.
  *
  * @param key The property key
  * @param value The property value
  *
  */
  void setProperty(const std::string& key, const std::string& value);

  /**
  *
  * Sets a property, if an already existing property exists
  * with the same key, the old value is overriden
  *
  * Notice that the old value is not returned.
  *
  * @param property The property to be set
  *
  */
  void setProperty(const Property& property);

  /**
  *
  * Erases a property with the given key, if the property there is no
  * property with such key no action is taken
  *
  *
  * @param key The key to the property being erased
  *
  */
  bool eraseProperty(const std::string& key);

  /**
  *
  * Find if a property with the given key exists
  *
  * @param key The key to the property being searched
  * @return true if the property exists, otherwise returns false
  */
  bool exists(const std::string& key) const;

  /**
  *
  * Return the size of the property list
  *
  * @return An integer with the number of properties
  */
  int size() const;

  /**
  *
  * Clears the property list
  *
  */
  void clear();

private:

  std::map<std::string, std::string> m_PropertyMap;

};

#endif // PROPERTIES_H__
