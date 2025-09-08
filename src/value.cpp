#include <unordered_map>
#include <stdexcept>
#include <string>
#include "../inc/value.hpp"

bool Value::is_intergral(){
    std::unordered_map<ValueType, short> lookup = {
        {ValueType::TYPE_INT, 0},
        {ValueType::TYPE_CHAR, 0},
        {ValueType::TYPE_BOOL, 0}
    };
    return lookup.count(this->_type) != 0;
}

// converts any integral type variable to an integer, raises an error if this is not an integral type
int Value::as_int(){
    if (!this->is_intergral())
        throw std::runtime_error("Cannot convert non-integral type to int");
    return std::get<int>(this->_val);
}

// creates any integral type Value from an integer 
Value Value::from_int(ValueType type, int val){
    switch (type){
        case ValueType::TYPE_INT: return Value(type, val);
        case ValueType::TYPE_BOOL: return Value(type, static_cast<bool>(val));
        case ValueType::TYPE_CHAR: return Value(type, static_cast<char>(val));
    }
}

// returns a string representation of the value, used for printing
std::string Value::to_string(){
    switch (this->_type){
    case ValueType::TYPE_INT:
        return std::to_string(std::get<int>(this->_val));
    case ValueType::TYPE_STR:
    case ValueType::TYPE_NAME:
        return std::get<std::string>(this->_val);
        break;
    case ValueType::TYPE_BOOL:
        return (std::get<bool>(this->_val)) ? "TRUE" : "FALSE";
    case ValueType::TYPE_CHAR:
        return;
    case ValueType::TYPE_NULL:
        return "NULL";
    }
}
