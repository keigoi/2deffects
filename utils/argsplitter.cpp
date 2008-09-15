
#include "utils/argsplitter.h"
#include <iostream>

using namespace std;

namespace syd {

vector<string> arg_split(const char* argv) {

	if( argv == NULL ) return vector<string>();

	string arg_str(argv);
	vector<string> result;

	while( !arg_str.empty() ) {

		if ( arg_str[0] == ' ' ) {
			//ƒJƒ‰“Ç‚Ý
			int i;
			for(i=1; i < arg_str.size(); i++) {
				if( arg_str[i] != ' ') {
					break;
				}
			}
			
			if( i == arg_str.size() ) { break; }
			arg_str = arg_str.substr( i, string::npos );

		} else if( arg_str[0] =='\"' ) {
		
			int pos = arg_str.find( '\"', 1 );

			if( pos == string::npos ) {
	
				result.push_back( arg_str.substr( 0, pos ) );
				break;
			}

			result.push_back( arg_str.substr( 0, pos+1 ) );
			
			arg_str = arg_str.substr( pos+1, string::npos );
			
		} else {

			int pos = arg_str.find( ' ' );
			result.push_back( arg_str.substr( 0, pos ) );

			if(pos == string::npos ) break;

			arg_str = arg_str.substr( pos+1, string::npos );
			
		}

	} // while( !arg_str.empty() )

	return result;

} // ArgSplit

ArgSplitter::ArgSplitter(const char* argv) {
	putRawArgs(argv);
} // constructor

ArgSplitter& ArgSplitter::putRawArgs(const char* argv) {
	strcpy(rawArgs_, argv);
	args_ = arg_split(argv);
	return *this;
} // putRawArgs

void ArgSplitter::printArgs(std::ostream& o) const {

	vector<string>::const_iterator it = args_.begin();

	o << "Raw Argument  : " << rawArgs_ << endl;

	o << "Number of Args: " << args_.size() << endl;

	for(int i=0; it!=args_.end(); it++,i++) {
		o << "Args No. " << i << ": " << it->c_str() << endl;
	}
	
} // printArgs

const std::vector<std::string> ArgSplitter::getArgs() const {
	return args_;
}
  
bool ArgSplitter::empty() const {
	return args_.empty();
}

} //syd

