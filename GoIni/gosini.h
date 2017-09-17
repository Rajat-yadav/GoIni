/********************************
/// (C) 2017 Go C++ Library.
/// 2017 All rights reserved
************************************/

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _GOINI_H
#define _GOINI_H

// C standard Headers
#include <cstdlib>
#include <cstring>

// C++ Standard Header Includes
#include <map>
#include <fstream>
#include <string>

#include "dll_def.h"

class GoIni
{
public:
	// Default constructor
	GOCALL GoIni(); 

	// Takes value as file name 
	// which will be used to complete read operations
	GOCALL GoIni(const char* value);

	// Virtual Destructor 
	// Needed because you can inherit from this class.
	GOCALL virtual ~GoIni(); 

	// Loads File for Reading
	// If the File is opened successfully then it returns true
	// Else it return false
	GOCALL bool loadFile(const char* file_name);

	// Parse and Return Char String From section_name and key_name
	// If no section key pair matches then it returns default value
	// which is specified by user
	GOCALL char* getCString(const char* section_name,const char* key_name,const char* default_value);

	// Works same as getCString but returns a std::string in place of char*
	GOCALL std::string getString(const char* section_name,const char* key_name,const std::string& default_value);
	
	// Disables inline comments 
	GOCALL void donotAllowComments();

	// Operator that can be used to push file name to GoIni instance
	// By this GoIni can open the file by value stored in its operand
	GOCALL inline GoIni& operator=(const char* right);

private:
	// Disables Copy Constructor
	GOCALL GoIni(const GoIni&);

	
	GOCALL char* returnStrip(char* s);
	GOCALL char* letterSkip(const char* s);

	GOCALL char* findCharsOrComments(const char* s, const char* chars);

	GOCALL void removeChar(char*s, char _char);
	
	// Configuration Variables
	bool GO_INI_ALLOW_INLINE_COMMENTS;
	bool GO_INI_ALLOW_SPECIAL_STRING_PARSING;

	std::fstream file;
	
	std::map<std::string, std::string> ini_data_handler;
};

// Comments Prefixs
#define GO_INI_COMMENTS_PREFIXS ";"


#endif // _GOINI_H
