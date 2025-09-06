#ifndef VALUE_H
#define VALUE_H

#include <string>

enum class ValueType{
    TYPE_INT,
    TYPE_BOOL,
    TYPE_CHAR,
    TYPE_STR,
};

class Value{
    private:
        ValueType _type;
        union{
            int _int_val;
            bool _bool_val;
            char _char_val;
            std::string _str_val;
        };
    public:
        template <typename T>
        Value(ValueType type, const T& value);
        template <typename T>
        void set_value(const T& value);
        template <typename T>
        T get_value();
        ~Value(){};
        ValueType get_type() {return this->_type;}
};

template <typename T>
Value::Value(ValueType type, const T& value){
    this->_type = type;
    this->set_value(value);
}

template <typename T>
void Value::set_value(const T& value){
    switch (this->_type){
        case ValueType::TYPE_INT:
            this->_int_val = value;
            break;
        case ValueType::TYPE_BOOL:
            this->_bool_val = value;
            break;
        case ValueType::TYPE_CHAR:
            this->_char_val = value;
            break;
        case ValueType::TYPE_STR:
            this->_str_val = value;
            break;
    }
}

#endif