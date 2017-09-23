#include "goini.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>

#include <sstream>
#include <iostream>

GoIni::GoIni()
	:GO_INI_ALLOW_INLINE_COMMENTS(true),
	GO_INI_ALLOW_SPECIAL_STRING_PARSING(true)
{

}

GoIni::GoIni(const char* value)
	: GO_INI_ALLOW_INLINE_COMMENTS(true),
	GO_INI_ALLOW_SPECIAL_STRING_PARSING(true)
{
	this->file_name = value;
}

GoIni::~GoIni()
{

}

bool GoIni::isReady()
{
	if (this->file_name == "" && this->error != "")
		return false;
	return true;
}

bool GoIni::loadFile(const char * file_name)
{
	this->file_name = file_name;
	return true;
}

char* GoIni::getCString(const char * section_name, const char * key_name, const char * default_value) {

	int track = 0;
	char line[200];
	std::fstream file(this->file_name);
	char section[50] = "";
	char* start;
	char* spec_start;
	char* end;
	char* name;
	char* value;

	if (!file.is_open())
	{
		std::stringstream errorbuf;
		errorbuf << "Couldn't Able to Open File: " << file_name;

		this->error = errorbuf.str();
		return _strdup(default_value);
	}

	while (file.getline(line, sizeof(line))) {
		track++;
		start = line;
		start = letterSkip(returnStrip(start));

		if (*start == ';' || *start == '#') {

		}
		else if (*start == '[') {
			end = findCharsOrComments(start + 1, "]");
			if (*end != ']')
			{
				std::string error = start;
				setError(error + " is not a valid section name. ] is missing. See online documentation of GoIni.", track);
				return _strdup(default_value);
			}
			else {
				*end = '\0';
				strncpy(section, start + 1, sizeof(section));
				section[sizeof(section) - 1] = '\0';
			}
		}
		else if (*start) {
			end = findCharsOrComments(start, "=:");
			if (*end == '=' || *end == ':') {
				*end = '\0';
				name = returnStrip(start);
				value = end + 1;
				value = letterSkip(value);
				returnStrip(value);

				if (this->GO_INI_ALLOW_SPECIAL_STRING_PARSING)
				{
					if (!strcmp(key_name, name) && !strcmp(section, section_name))
					{
						if (*value == '\"')
						{
							end = findCharsOrComments(value, "\"");
							while (*end == '\"')
							{
								spec_start = findCharsOrComments(value, "\\");
								if (*spec_start == '\\')
								{
									spec_start++;
									if (*spec_start == 'n')
									{
										removeChar(spec_start, 'n');
										*(spec_start - 1) = '\n';
									}
									else if (*spec_start == 't')
									{
										removeChar(spec_start, 't');
										*(spec_start - 1) = '\t';
									}
									else if (*spec_start == '0')
									{
										removeChar(spec_start, '0');
										*(spec_start - 1) = '\0';
									}
									else if (*spec_start == 'b')
									{
										removeChar(spec_start, 'b');
										*(spec_start - 1) = '\b';
									}
									else if (*spec_start == 'v')
									{
										removeChar(spec_start, 'v');
										*(spec_start - 1) = '\v';
									}
									else if (*spec_start == 'a')
									{
										removeChar(spec_start, 'a');
										*(spec_start - 1) = '\a';
									}
									else if (*spec_start == 'r')
									{
										removeChar(spec_start, 'r');
										*(spec_start - 1) = '\r';
									}
									else if (*spec_start == 'f')
									{
										removeChar(spec_start, 'f');
										*(spec_start - 1) = '\f';
									}
									else
										error = track;
								}
								spec_start = findCharsOrComments(value, "\\");
								if (*spec_start == '\\')
									continue;
								else removeChar(end, '\"');
							}
							removeChar(value, '\"');
						}
					}
				}

				if (this->GO_INI_ALLOW_INLINE_COMMENTS) {
					end = findCharsOrComments(value, NULL);
					if (*end)
						*end = '\0';
				}

				returnStrip(value);

				if (!strcmp(section, section_name) && !strcmp(name, key_name))
				{
					return _strdup(value);
				}
				else
				{
					if (!strcmp(section, section_name))
						this->error = "In File " + this->file_name + " in section " + section_name + " no key named " + key_name + " found.";
					else 
						this->error = "In File " + this->file_name + " no section named " + section_name + " found.";
				}
			}
		}
	}
		return _strdup(default_value);
}

std::string GoIni::getString(const char * section_name, const char * key_name, const std::string & default_value)
{
	std::string value = getCString(section_name, key_name, default_value.c_str());
	return value;
}

long GoIni::getInt(const char * section_name, const char * key_name, int default_value)
{
	std::stringstream buffer;
	buffer << default_value;
	std::string valuestr = getCString(section_name,key_name,buffer.str().c_str());
	const char* value = valuestr.c_str();
	char* end;
	long rvalue = strtol(value, &end, 0);
	return end > value ? rvalue : default_value;
}

bool GoIni::getBoolean(const char * section_name, const char * key_name, bool default_value)
{
	std::string valstr = getString(section_name,key_name,"");
	// Convert to lower case to make string comparisons case-insensitive
	std::transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);
	if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
		return true;
	else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
		return false;
	else
		return default_value;
}

double GoIni::getDouble(const char * section_name, const char * key_name, double default_value)
{
	std::string valstr = getString(section_name, key_name, "");
	const char* value = valstr.c_str();
	char* end;
	double n = strtod(value, &end);
	return end > value ? n : default_value;
}

std::string GoIni::getError() const
{
	return this->error;
}

void GoIni::donotAllowComments()
{
	this->GO_INI_ALLOW_INLINE_COMMENTS = false;
}

inline GoIni & GoIni::operator=(const char * right)
{
	this->file_name = right;
	return *(this);
}

char * GoIni::returnStrip(char * s)
{
	char* point = s + strlen(s);
	while (point > s && isspace((unsigned char)(*--point)))
		*point = '\0';
	return s;
}

char * GoIni::letterSkip(const char * s)
{
	while (*s && isspace((unsigned char)(*s)))
		s++;
	return (char*)s;
}

char * GoIni::findCharsOrComments(const char * s, const char * chars)
{
	if (this->GO_INI_ALLOW_INLINE_COMMENTS) {
		int was_space = 0;
		while (*s && (!chars || !strchr(chars, *s)) && !(was_space && strchr(GO_INI_COMMENTS_PREFIXS, *s)))
		{
			was_space = isspace((unsigned char)(*s));
			s++;
		}
		return (char*)s;
	}

	while (*s && (!chars || !strchr(chars, *s)))
		s++;

	return (char*)s;
}

void GoIni::removeChar(char * s, char _char)
{
	char* src = s;
	char* dest = s;
	int index = 0;
	size_t size = strlen(s);

	while (*src != _char && index != size)
	{
		src++;
		index++;
	}
	if (*src == _char)
	{
		memcpy(&dest[index], &dest[index + 1], strlen(dest - index));
		return;
	}
	return;
}

void GoIni::setError(const std::string & error, int line_num)
{
	std::stringstream buffer;
	buffer << line_num;
	this->error = "In File " + this->file_name + " At Line " + buffer.str() + " " + error;
	return void();
}
