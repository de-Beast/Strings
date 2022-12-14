#include "IdentifierString.h"
#include <iostream>

using std::cout, std::endl;

IdentifierString::IdentifierString()
{
	cout << "Identifier BaseString - Default constructor" << endl;
}

IdentifierString::IdentifierString(const IdentifierString& obj)
	: BaseString(obj, false)
{
	cout << "Identifier BaseString - Copy constructor: " << ++_copy_constructor_calls << endl;
}

IdentifierString::IdentifierString(const char* c_str)
{
	const size_t lenght = strlen(c_str);
	char* string = new char[lenght + 1];

	cout << "Identifier BaseString - Constructor with c-string" << endl;

	if (not isalpha(c_str[0]) and c_str[0] != '_')
	{
		delete[] string;
		return;
	}

	for (size_t i = 0; i < lenght + 1; i++)
	{
		if (isalnum(c_str[i]) or c_str[i] == '_' or c_str[i] == '\0')
			string[i] = c_str[i];
		else
		{
			delete[] string;
			return;
		}
	}
	delete[] _string;
	_length = lenght;
	_string = string;
}

size_t IdentifierString::find_char(const char c) const
{
	cout << "Identifier BaseString - Find char method" << endl;

	for (size_t i = 0; i < _length; i++)
		if (_string[i] == c)
			return i + 1;
	return 0;
}

IdentifierString& IdentifierString::operator=(const IdentifierString& obj)
{
	cout << "Identifier BaseString - operator =" << endl;

	if (this == &obj)
		return *this;
	BaseString::operator=(obj);
	return *this;
}

bool IdentifierString::operator>(const IdentifierString& obj) const
{
	cout << "Identifier BaseString - operator >" << endl;

	return _length > obj._length;
}

bool IdentifierString::operator>=(const IdentifierString& obj) const
{
	cout << "Identifier BaseString - operator >=" << endl;

	return _length >= obj._length;
}
