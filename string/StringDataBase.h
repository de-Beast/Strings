#pragma once
#include <vector>

#include "BaseString.h"

class StringDataBase
{
public:
	struct StringData
	{
		enum class EStringType
		{
			EMPTY,
			BASE_STRING,
			IDENTIFIER_STRING,
			HEX_STRING,
		} str_type = EStringType::EMPTY;

		enum class EConstructorType
		{
			C_STR,
			COPY,
			CHAR,
		} constr_type = EConstructorType::C_STR;
	};

	enum class ETestOperator
	{
		ASSIGNMENT,
		ADDITION,
		BIGGER,
		BIGGER_OR_EQUAL,
		EQUALITY
	};

	StringDataBase() = delete;
	~StringDataBase();

	static void init_instance(const int size);
	static StringDataBase& get_instance();
	static size_t size();

	__declspec(property(get = get_test_index, put = set_test_index)) size_t Test_index;
	void set_test_index(const size_t index);
	size_t get_test_index() const;

	__declspec(property(get = get_test_operator, put = set_test_operator)) ETestOperator Test_operator;
	void set_test_operator(const ETestOperator test_method);
	ETestOperator get_test_operator() const;

	template<class Str> requires std::derived_from<Str, BaseString>
	void set_string(const size_t index, Str* str);

	StringData get_string_data(const size_t index) const;
	void set_constructor_type(const size_t index, const StringData::EConstructorType constr_type);
	void set_string_type(const size_t index, const StringData::EStringType string_type);

	BaseString* operator[](const size_t index) const;

private:
	explicit StringDataBase(const int size);

	inline static StringDataBase* _instance = nullptr;

	std::vector<BaseString*> _strings;
	std::vector<StringData> _strings_data;
	size_t _test_index = -1;
	ETestOperator _test_operator = ETestOperator::ASSIGNMENT;
};

template <class Str> requires std::derived_from<Str, BaseString>
void StringDataBase::set_string(const size_t index, Str* str)
{
	if (index < _strings.size())
	{
		delete _strings[index];
		_strings[index] = str;
	}
}
