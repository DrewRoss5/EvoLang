#include <unordered_map>
#include <stdexcept>
#include <string>
#include <iostream>
#include "../inc/value.hpp"

// associates the four main value types with their names as strings, using the numeric enum values as an index
const char* TYPE_ARR[]{
    "int",
    "bool",
    "char",
    "string"
};

// returns true if the value is an integral type
bool Value::is_intergral() const{
    std::unordered_map<ValueType, short> lookup = {
        {ValueType::TYPE_INT, 0},
        {ValueType::TYPE_CHAR, 0},
        {ValueType::TYPE_BOOL, 0}
    };
    return lookup.count(this->_type) != 0;
}

// converts any integral type variable to an integer, raises an error if this is not an integral type
int Value::as_int() const{
    if (!this->is_intergral())
        throw std::runtime_error("Cannot convert non-integral type to int");
    switch (this->_type){
        case ValueType::TYPE_INT: return std::get<int>(this->_val);
        case ValueType::TYPE_CHAR: return static_cast<int>(std::get<char>(this->_val));
        case ValueType::TYPE_BOOL: return static_cast<int>(std::get<bool>(this->_val));
    }
    
}

/// returns a string representation of the value, used for printing or conversion
std::string Value::to_string() const{
    switch (this->_type){
        case ValueType::TYPE_INT:
            return std::to_string(std::get<int>(this->_val));
        case ValueType::TYPE_STR:
        case ValueType::TYPE_NAME:
            return std::get<std::string>(this->_val);
        case ValueType::TYPE_BOOL:
            return (std::get<bool>(this->_val)) ? "TRUE" : "FALSE";
        case ValueType::TYPE_CHAR:
            return std::string(1, std::get<char>(this->_val));
        case ValueType::TYPE_VALTYPE:
            return TYPE_ARR[std::get<int>(this->_val)];
        case ValueType::TYPE_NULL:
            return "";
    }
    return "ERROR TYPE";
}

// converts a value to a boolen, if the value is not an integral type, this will throw an error
bool Value::as_bool() const{
    if (!this->is_intergral())
        throw std::runtime_error("Invalid type for boolean conversion");
    return this->as_int() != 0;
}

// converts a value to a character, if the value is not a integer, this will throw an error
char Value::as_char() const{
    if (this->_type == ValueType::TYPE_STR)
        return std::get<std::string>(this->_val)[0];
    return static_cast<char>(std::get<int>(this->_val));
}

// creates any integral type Value from an integer 
Value Value::from_int(ValueType type, int val){
    switch (type){
        case ValueType::TYPE_INT: return Value(type, val);
        case ValueType::TYPE_BOOL: return Value(type, static_cast<bool>(val));
        case ValueType::TYPE_CHAR: return Value(type, static_cast<char>(val));
    }
    return Value(ValueType::TYPE_NULL, "");
}


// returns the value at a given index, raises an error if this is not a collection type, or if the index is out of range
Value Value::get_index(size_t index) const{
    if (!this->is_collection())
        throw std::runtime_error("Cannot get an index of a non-collection type.");
    // when arrays are implemented, this will be changed to a switch case, but for now, this function always runs for string values
    std::string str = std::get<std::string>(this->_val);
    if (index >= str.size())
        throw std::out_of_range("Index out of range");
    return Value(ValueType::TYPE_CHAR, str.at(index));
}

// returns the length of this value if it's a collection type, otherwise raises an error
size_t Value::get_len() const{
    if (!this->is_collection())
        throw std::runtime_error("Cannot get the length of a non-collection type.");
    std::string str = std::get<std::string>(this->_val);
    return str.length();
}

/* 
    returns true if this value is a collection type, at the moment, this only returns true if
    the Value is a string, however, this is it's own function to account for future array support
*/
bool Value::is_collection() const{
    return this->_type == ValueType::TYPE_STR;
}

// returns if two values are equal in both type and value
bool Value::operator==(const Value& rhs){
    if (this->_type != rhs._type)
        return false;
    return this->_val == rhs._val;
}

// returns if two values are inequal in type and/or value
bool Value::operator!=(const Value& rhs){
    return !(*this == rhs);
}

// greater than comparison for values, do not call this unless you've ensured both types are integrals
bool Value::operator>(const Value& rhs){
    return this->as_int() > rhs.as_int();
}