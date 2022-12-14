#include "StringDataBase.h"
#include "String.h"

StringDataBase::StringDataBase(const int size) : _strings(size), _strings_data(size)
{
	for (size_t i = 0; auto & str : _strings)
	{
		str = new BaseString();

		switch (i)
		{
			case 0:
			{
				delete _strings[0];
				_strings[0] = new BaseString("Hello");
				_strings_data[0].str_type = StringData::EStringType::BASE_STRING;
				break;
			}
			case 1:
			{
				delete _strings[1];
				_strings[1] = new IdentifierString("stick");
				_strings_data[1].str_type = StringData::EStringType::IDENTIFIER_STRING;
				break;
			}
			case 2:
			{
				delete _strings[2];
				_strings[2] = new IdentifierString("__init__");
				_strings_data[2].str_type = StringData::EStringType::IDENTIFIER_STRING;
				break;
			}
			case 3:
			{
				delete _strings[3];
				_strings[3] = new HexString("-a23f");
				_strings_data[3].str_type = StringData::EStringType::HEX_STRING;
				break;
			}
			case 4:
			{
				delete _strings[4];
				_strings[4] = new HexString("+4de5");
				_strings_data[4].str_type = StringData::EStringType::HEX_STRING;
				break;
			}
			case 5:
			{
				delete _strings[5];
				_strings[5] = new BaseString("lol");
				_strings_data[5].str_type = StringData::EStringType::BASE_STRING;
				break;
			}
		}
		i++;
	}
}

StringDataBase::~StringDataBase()
{
	for (auto& str : _strings)
	{
		delete str;
	}
}

void StringDataBase::init_instance(const int size)
{
	if (_instance == nullptr)
		_instance = new StringDataBase(size);
}

StringDataBase& StringDataBase::get_instance()
{
	if (_instance == nullptr)
		throw std::runtime_error("StringDataBase is not initialized");

	return *_instance;
}

size_t StringDataBase::size()
{
	return _instance->_strings.size();
}

size_t StringDataBase::get_test_index() const
{
	return _test_index;
}

void StringDataBase::set_test_operator(const ETestOperator test_method)
{
	_test_operator = test_method;
}

StringDataBase::ETestOperator StringDataBase::get_test_operator() const
{
	return _test_operator;
}

StringDataBase::StringData StringDataBase::get_string_data(const size_t index) const
{
	if (index >= _strings_data.size())
		throw std::out_of_range("StringDataBase::get_string_data: index is out of range");
		
	return _strings_data[index];
}

void StringDataBase::set_constructor_type(const size_t index, const StringData::EConstructorType constr_type)
{
	if (index < _strings_data.size())
		_strings_data[index].constr_type = constr_type;
}

void StringDataBase::set_string_type(const size_t index, const StringData::EStringType string_type)
{
	if (index < _strings_data.size())
		_strings_data[index].str_type = string_type;
}

BaseString* StringDataBase::operator[](const size_t index) const
{
	return _strings.at(index);
}

void StringDataBase::set_test_index(const size_t index)
{
	if (index < _strings.size())
		_test_index = index;
}
