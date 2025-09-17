#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <variant>
#include <stdexcept>

enum class ValueType{
    TYPE_INT,
    TYPE_BOOL,
    TYPE_CHAR,
    TYPE_STR,
    TYPE_NAME,
    TYPE_VALTYPE,
    TYPE_NULL,
};

class Value{
    private:
        ValueType _type;
        std::variant<int, bool, char, std::string> _val;
    public:
        Value() : _type(ValueType::TYPE_NULL) {}
        template <typename T>
        Value(ValueType type, const T& value);
        template <typename T>
        void set_value(const T& value);
        const std::variant<int, bool, char, std::string>& get_value() const {return this->_val;}
        int as_int() const;
        static Value from_int(ValueType type, int val);
        ~Value(){};
        ValueType get_type() const {return this->_type;}
        std::string to_string() const;
        bool as_bool() const;
        char as_char() const;
        Value get_index(size_t index) const;
        size_t get_len() const;
        bool is_intergral() const;
        bool is_collection() const;
        bool operator==(const Value& rhs);
        bool operator!=(const Value& rhs);
        bool operator>(const Value& rhs);

};

template <typename T>
Value::Value(ValueType type, const T& value){
    this->_type = type;
    this->set_value(value);
}

template <typename T>
void Value::set_value(const T& value){
    this->_val = value;
}



#endif