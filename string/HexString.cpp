#include "HexString.h"
#include <iostream>
#include <cctype>

using std::cout, std::endl;

HexString::HexString()
{
	delete[] _string;
	_length = 1;
	_string = new char[2];
	_string[0] = '0';
	_string[1] = '\0';

	cout << "Hex BaseString - Default constructor" << endl;
}

HexString::HexString(const HexString& obj)
	: BaseString(obj, false)
{
	cout << "Hex BaseString - Copy constructor: " << ++_copy_constructor_calls << endl;
}

HexString::HexString(const char* c_str)
	: HexString()
{
	const size_t lenght = strlen(c_str);
	char* string = new char[lenght + 1];

	cout << "Hex BaseString - Constructor with c-string" << endl;

	if (c_str[0] != '+' and c_str[0] != '-'
		and not (tolower(c_str[0]) >= 'a' and tolower(c_str[0]) <= 'f')
		and not isdigit(c_str[0])
		or (c_str[0] == '+' or c_str[0] == '-') and lenght == 1)
	{
		delete[] string;
		return;
	}

	string[0] = c_str[0];

	for (size_t i = 1; i < lenght + 1; i++)
	{
		if (tolower(c_str[i]) >= 'a' and tolower(c_str[i]) <= 'f'
			or isdigit(c_str[i]) or c_str[i] == '\0')
		{
			string[i] = c_str[i];
		}
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

int HexString::to_int() const
{
	int num = 0;
	for (size_t i = 0; i < _length; i++)
	{
		if (tolower(_string[i]) >= 'a' and tolower(_string[i]) <= 'f')
		{
			num += (tolower(_string[i]) - 'a' + 10) * static_cast<int>(pow(16, _length - i - 1));
		}
		else if (isdigit(_string[i]))
		{
			num += (tolower(_string[i]) - '0') * static_cast<int>(pow(16, _length - i - 1));
		}
	}

	cout << "Hex BaseString - To int method" << endl;

	return _string[0] == '-' ? -num : num;
}

bool HexString::is_positive() const
{
	cout << "Hex BaseString - Is positive method" << endl;

	return to_int() > 0;
}

HexString& HexString::operator=(const HexString& obj)
{
	cout << "Hex BaseString - operator =" << endl;

	if (this == &obj)
		return *this;
	BaseString::operator =(obj);
	return *this;
}

bool HexString::operator==(const HexString& obj) const
{
	cout << "Hex BaseString - operator ==" << endl;

	return to_int() == obj.to_int();
}

HexString HexString::operator+(const HexString& obj) const
{
	const int num = to_int() + obj.to_int();
	int length = num < 0 ? 1 : 0;

	int tmp = num;
	while (tmp != 0)
	{
		tmp /= 16;
		length++;
	}

	tmp = num;
	char* string = new char[length + 1];
	string[length] = '\0';
	for (int i = length - 1; i >= 0; i--)
	{
		string[i] = abs(tmp % 16) + (abs(tmp % 16) >= 10 ? 'a' - 10 : '0');
		tmp /= 16;
	}
	if (num < 0)
		string[0] = '-';

	cout << "Hex BaseString - operator +" << endl;

	return HexString(string);
}
