#include "goini.h"

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
	this->file.open(value, std::ios::in|std::ios::out);
}

GoIni::~GoIni()
{

}

bool GoIni::loadFile(const char * file_name)
{
	if (this->file.is_open()) {
		this->file.close();
		this->file.clear();
	}

	this->file.open(file_name, std::ios::in | std::ios::out);

	if (!this->file.is_open())
		return false;

	return true;
}

char* GoIni::getCString(const char * section_name, const char * key_name, const char * default_value) {

	int track = 0;
	int error = 0;
	char line[250];
	char section[50] = "";
	char* start;
	char* spec_start;
	char* end;
	char* name;
	char* value;

	this->file.seekg(0);

	while (this->file.getline(line, sizeof(line))) {
		track++;
		start = line;
		start = letterSkip(returnStrip(start));

		if (*start == ';' || *start == '#') {

		}
		else if (*start == '[') {
			end = findCharsOrComments(start + 1, "]");
			if (*end != ']')
				error = track;
			else {
				*end = '\0';
				strncpy_s(section, start + 1, sizeof(section));
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
					if (!strcmp(key_name,name),!strcmp(section,section_name))
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
									if (*spec_start = 'n')
									{
										removeChar(spec_start, 'n');
										*(spec_start-1) = '\n';
									}
								}
								spec_start = findCharsOrComments(value, "\\");
								if (*spec_start == '\\')
									continue;
								else break;
							}
							removeChar(value, '\"');
							removeChar(end, '\"');
						}
					}
				}

				if (this->GO_INI_ALLOW_INLINE_COMMENTS) {
					end = findCharsOrComments(value, NULL);
					if (*end)
						*end = '\0';
				}

				returnStrip(value);

				if (!strcmp(name,key_name))
				{
					if (!strcmp(section,section_name))
						break;
				}
			}
		  
		}
	}
	
	if (strcmp(name,key_name))
		return _strdup(default_value);

		return _strdup(value);
}

std::string GoIni::getString(const char * section_name, const char * key_name, const std::string & default_value)
{
	std::string value = getCString(section_name, key_name, default_value.c_str());
	return value;
}

void GoIni::donotAllowComments()
{
	this->GO_INI_ALLOW_INLINE_COMMENTS = false;
}

inline GoIni & GoIni::operator=(const char * right)
{
	(this->file.is_open()) ? this->file.close(), this->file.clear() : 0;
	this->file.open(right, std::ios::in | std::ios::out);
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
	size_t comp = 0;
	while (*src != _char && size >= comp)
	{
		src++;
		comp++;
		index++;
	}
	if (*src == _char)
	{
		memmove(&dest[index], &dest[index + 1], strlen(dest - index));
		return;
	}
	return;
}
