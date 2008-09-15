// $Id: ArgSplitter.h,v 1.2 2002/03/21 21:52:18 noname Exp $

#ifndef _ARGSPLITTER_H_
#define _ARGSPLITTER_H_

#include <vector>
#include <string>

namespace syd {


std::vector<std::string> arg_split ( const char* argv );


class ArgSplitter {

	std::vector<std::string> args_;
	char rawArgs_[1024];

public:
	ArgSplitter ( const char* argv );

public:
	ArgSplitter& putRawArgs ( const char* argv );

public:
	bool empty() const;
	void printArgs(std::ostream& o) const;

	const std::vector<std::string> 	getArgs () const;

private:
	ArgSplitter ();

}; //ArgSplitter

} //syd


#endif //_ARGSPLITTER_H_