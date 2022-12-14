#pragma once

#include "BaseString.h"

class IdentifierString : public BaseString
{
public:
	IdentifierString();
	IdentifierString(const IdentifierString& obj);
	IdentifierString(const char* c_str);

	size_t find_char(const char c) const;

	IdentifierString& operator=(const IdentifierString& obj);
	bool operator>(const IdentifierString& obj) const;
	bool operator>=(const IdentifierString& obj) const;

private:
	static inline int _copy_constructor_calls = 0;
};

