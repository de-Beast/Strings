#pragma once

#include "BaseString.h"

class HexString : public BaseString
{
public:
	HexString();
	HexString(const HexString& obj);
	HexString(const char* c_str);

	int to_int() const;
	bool is_positive() const;

	HexString& operator=(const HexString& obj);
	bool operator==(const HexString& obj) const;
	HexString operator+(const HexString& obj) const;

private:
	static inline int _copy_constructor_calls = 0;
};

