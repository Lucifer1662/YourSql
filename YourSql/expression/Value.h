#pragma once
#include "ValueType.h"
#include "Expression.h"




struct Value : public Expression {
	const ValueType type;

	Value(ValueType type) : type(type) {}
	Value(const Value&) = default;
	Value(Value&&) = default;
	Value& operator=(const Value&) = default;

	virtual bool operator>(const Value& value) = 0;
	virtual bool operator<(const Value& value) = 0;
	//virtual std::unique_ptr<Value> operator+(const Value& value) = 0;
	virtual Value& operator()() {
		return *this;
	}

	virtual void addSelf(const Value& value) = 0;
	virtual void copySelf(const Value& value) = 0;

	
};

