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

#include <string>

#include "Properties.h"

Properties::iterator Properties::begin()
{
  return m_PropertyMap.begin();
}

Properties::const_iterator Properties::begin() const
{
  return m_PropertyMap.begin();
}

Properties::iterator Properties::end()
{
  return m_PropertyMap.end();
}

Properties::const_iterator Properties::end() const
{
  return m_PropertyMap.end();
}

std::string Properties::propertyValue(const std::string& key) const
{
  std::map<std::string, std::string>::const_iterator it = m_PropertyMap.find(key);

  if (it == m_PropertyMap.end())
    return "";

  return it->second;
}

std::string Properties::Get(const std::string& key) const
{
	return propertyValue(key);
}

Property Properties::property(const std::string& key) const
{
  std::map<std::string, std::string>::const_iterator it = m_PropertyMap.find(key);

  if (it == m_PropertyMap.end())
    return Property();

  return Property(it->first, it->second);
}

void Properties::setProperty(const std::string& key, const std::string& value)
{
  m_PropertyMap[key] = value;
}

void Properties::setProperty(const Property& property)
{
  setProperty(property.key(), property.value());
}

bool Properties::eraseProperty(const std::string& key)
{
  if (!exists(key))
   return false;

  m_PropertyMap.erase(key);
  return true;
}

bool Properties::exists(const std::string& key) const
{
  return m_PropertyMap.end() != m_PropertyMap.find(key);
}

int Properties::size() const
{
  return m_PropertyMap.size();
}

void Properties::clear()
{
  m_PropertyMap.clear();
}

