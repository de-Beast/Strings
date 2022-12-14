#pragma once

#include <ostream>

class BaseString
{
public:
	BaseString();
	BaseString(const BaseString& obj, bool increment_call = true);
	BaseString(const char* c_str);
	BaseString(const char c);
	virtual ~BaseString();

	const char* c_str() const;
	bool is_empty() const;
	size_t length() const;

	BaseString& operator=(const BaseString& obj);

	friend std::ostream& operator<<(std::ostream& os, const BaseString& obj);

protected:
	char* _string;
	size_t _length;

private:
	static inline int _copy_constructor_calls = 0;
}; 
