#include "BaseString.h"
#include <iostream>

using std::cout, std::endl;

BaseString::BaseString()
	: _length{0}
{
	_string = new char[1];
	_string[0] = '\0';

	cout << "BaseString - Default constructor" << endl;
}

BaseString::BaseString(const BaseString& obj, const bool increment_call)
{
	_length = obj._length;
	_string = new char[_length + 1];
	for (size_t i = 0; i < _length + 1; i++)
		_string[i] = obj._string[i];

	if (increment_call)
		cout << "BaseString - Copy constructor: " << ++_copy_constructor_calls << endl;
}

BaseString::BaseString(const char* c_str)
{
	_length = strlen(c_str);
	_string = new char[_length + 1];
	for (size_t i = 0; i < _length + 1; i++)
		_string[i] = c_str[i];
	cout << "BaseString - Constructor with c-string" << endl;
}

BaseString::BaseString(const char c)
{
	_length = 1;
	_string = new char[2];
	_string[0] = c;
	_string[1] = '\0';
	cout << "BaseString - Constructor with char" << endl;
}

BaseString::~BaseString()
{
	delete[] _string;
	cout << "BaseString - Destructor" << endl;
}

const char* BaseString::c_str() const
{
	return _string;
}

size_t BaseString::length() const
{
	cout << "BaseString - Length method" << endl;
	return _length;
}

bool BaseString::is_empty() const
{
	cout << "BaseString - Is empty method" << endl;
	return _length == 0;
}

BaseString& BaseString::operator=(const BaseString& obj)
{
	cout << "BaseString - operator =" << endl;

	if (this == &obj)
		return *this;

	_length = obj._length;
	_string = new char[_length + 1];
	for (int i = 0; i < _length + 1; i++)
		_string[i] = obj._string[i];

	return *this;
}

std::ostream& operator<<(std::ostream& os, const BaseString& obj)
{
	return os << obj._string;
}
