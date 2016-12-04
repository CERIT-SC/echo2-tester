//
//  Optional.h
//  optional
//
//  Created by Miloš Šimek on 03.06.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#ifndef optional_Optional_h
#define optional_Optional_h

#include <stdexcept>
#include <utility>
#include <string>
using namespace std;

class OptionalExpeption;
class Opt { public: enum OptionalNoValue { NoValue }; };


template <class Type>
class Optional : public Opt {
    bool hasValue = false;
    Type value;
    
public:
    Optional();
    Optional(const Type& value);
    Optional(const Optional&);
    Optional(const Optional&&);
    Optional(const OptionalNoValue&);
    
    operator bool() const;
    Optional& operator=(const Optional&);
    Optional& operator=(const Type&);
    Optional& operator=(const OptionalNoValue&);
    
    Type& operator*(); //access to internal value
    const Type& operator*() const;
};


class OptionalExpeption : public logic_error {
public:
    OptionalExpeption(string m) : logic_error(m) {};
};



//IMPLEMENTATION

//constructors
template <class Type>
Optional<Type>::Optional() {}

template <class Type>
Optional<Type>::Optional(const Type& value) {
    hasValue = true;
    this->value = value;
}

template <class Type>
Optional<Type>::Optional(const Optional& opt) {
    *this = opt;
}

template <class Type>
Optional<Type>::Optional(const Optional&& opt) {
    *this = std::move(opt);
}

template <class Type>
Optional<Type>::Optional(const OptionalNoValue&) {
    hasValue = false;
}


//operators

template <class Type>
Optional<Type>::operator bool() const {
    return hasValue;
}

template <class Type>
Optional<Type>& Optional<Type>::operator=(const Optional<Type>& opt) {
    if (opt.hasValue) {
        hasValue = true;
        value = *opt;
    } else {
        hasValue = false;
    }
    
    return *this;
}

template <class Type>
Optional<Type>& Optional<Type>::operator=(const Type& value) {
    hasValue = true;
    this->value = value;
    
    return *this;
}

template <class Type>
Optional<Type>& Optional<Type>::operator=(const OptionalNoValue&) {
    hasValue = false;
    
    return *this;
}


template <class Type>
Type& Optional<Type>::operator*() {
    if (!hasValue) throw OptionalExpeption("optional_no_value");
    
    return value;
}

template <class Type>
const Type& Optional<Type>::operator*() const {
    if (!hasValue) throw OptionalExpeption("optional_no_value");
    
    return value;
}

#endif
