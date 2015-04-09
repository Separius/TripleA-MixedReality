/*
 * ClientException.h
 *
 *  Created on: Oct 24, 2008
 *      Author: brt
 */

#ifndef CLIENTEXCEPTION_H_
#define CLIENTEXCEPTION_H_

#include <stdexcept>
#include <string>

class ClientException : public std::exception {
public:
	ClientException(const std::string& what)
	{
		mWhat = what;
	}

	virtual const char * what() const throw()
	{
		return mWhat.c_str();
	}

	virtual ~ClientException() throw() {};

	std::string mWhat;
};


#endif /* CLIENTEXCEPTION_H_ */
