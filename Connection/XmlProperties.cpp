/*
 * XmlProperty.cpp
 *
 *  Created on: May 15, 2009
 *      Author: fagner
 */

#include "XmlProperties.h"

XmlProperties::XmlProperties()
{

}


/*virtual*/
bool const
XmlProperties::write(std::ostream* doc, unsigned int indent)
{
	iterator it;
	for (it = m_Properties.begin(); it != m_Properties.end() ; ++it)
	{
		XMLSerializer::writeLeafNode(doc, it->first, it->second, NULL, indent);
	}

	return true;
}

/*virtual*/
bool const
XmlProperties::read(const TLDocumentSlice& slice)
{
	std::vector<XMLItem> items;
    XMLSerializer::itemize(slice, items);

    std::vector<XMLItem>::iterator it;
	for (it = items.begin(); it != items.end() ; ++it)
	{
		m_Properties[it->getName()] = it->getContent();
	}

	return true;
}

void
XmlProperties::Add(const std::string& key, const std::string& value)
{
	m_Properties[key] = value;
}

void
XmlProperties::Erase(const std::string& key)
{
	m_Properties.erase(key);
}

bool
XmlProperties::Exists(const std::string& key) const
{
	return m_Properties.find(key) != m_Properties.end();
}

std::string
XmlProperties::Value(const std::string& k) const
{
	const_iterator it = m_Properties.find(k);
	return (it == end() ? "" : it->second);
}

void
XmlProperties::insert(iterator b, iterator e)
{
	m_Properties.insert(b, e);
}

XmlProperties::iterator
XmlProperties::begin()
{
	return m_Properties.begin();
}

XmlProperties::const_iterator
XmlProperties::begin() const
{
	return m_Properties.begin();
}

XmlProperties::iterator
XmlProperties::end()
{
	return m_Properties.end();
}

XmlProperties::const_iterator
XmlProperties::end() const
{
	return m_Properties.end();
}

XmlProperties::size_type
XmlProperties::size() const
{
	return m_Properties.size();
}

/*virtual*/
std::string const
XmlProperties::getName() const
{
	return "properties";
}
