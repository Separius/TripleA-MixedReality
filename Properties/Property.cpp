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

#include "Property.h"

Property::Property()
{
}

Property::Property(const std::string& key, const std::string& value)
  : m_Key(key), m_Value(value)
{
}
         
const std::string& Property::key() const
{
  return m_Key;
}

void Property::key(const std::string& aKey)
{
  m_Key = aKey;
}
  
const std::string& Property::value() const
{
  return m_Value;
}

void Property::value(const std::string& aValue) 
{
  m_Value = aValue;
}
