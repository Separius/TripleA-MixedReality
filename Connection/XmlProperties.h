/*
 * XmlProperty.h
 *
 *  Created on: May 15, 2009
 *      Author: fagner
 */

#ifndef XMLPROPERTIES_H_
#define XMLPROPERTIES_H_

#include <vector>
#include <map>
#include <string>

#include <DeluxeML/XMLSerializable.h>
#include <DeluxeML/XMLSerializer.h>

class XmlProperties : public XMLSerializable {

	typedef std::map< std::string, std::string>  PropertySequence;
	typedef  PropertySequence::size_type         size_type;
	typedef PropertySequence::iterator           iterator;
	typedef PropertySequence::const_iterator     const_iterator;

public:

	XmlProperties();

    virtual bool const write(std::ostream* doc, unsigned int indent=0);
    virtual bool const read(const TLDocumentSlice& slice);

    void Add(const std::string& key, const std::string& value);
    void insert(iterator b, iterator e);
    void Erase(const std::string& key);
    bool Exists(const std::string& key) const;
    std::string Value(const std::string& k) const;

    iterator begin();
    const_iterator begin() const;

    iterator end();
    const_iterator end() const;

    size_type size() const;

    virtual std::string const getName() const;

private:

	std::map< std::string, std::string > m_Properties;
};

#endif /* XMLPROPERTIES_H_ */
