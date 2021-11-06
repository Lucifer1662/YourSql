#pragma once
#include <memory>
#include <exception>
#include "fields/Field.h"

enum class ValueType {
	Invalid,Int, Float, Long, Double, String, Date, DateTime
};

struct Value;

struct Expression
{
	virtual Value& operator()() = 0;
};


struct Value : public Expression {
	const ValueType type;

	Value(ValueType type) : type(type) {}
	Value(const Value&) = default;
	Value(Value&&) = default;
	Value& operator=(const Value&) = default;

	virtual bool operator>(const Value& value) = 0;
	virtual bool operator<(const Value& value) = 0;
	virtual Value& operator+(const Value& value) = 0;
	virtual Value& operator()() {
		return *this;
	}
};


struct Predicate
{
	virtual bool operator()() = 0;

};



template<typename T> ValueType getType();


template<> ValueType inline getType<int>() { return ValueType::Int; };
template<> ValueType inline getType<float>() { return ValueType::Float; };
template<> ValueType inline getType<long>() { return ValueType::Long; };
template<> ValueType inline getType<double>() { return ValueType::Double; };

template<typename T>
struct INumber : public Value {

	INumber() : Value(getType<T>()) {}
	INumber(const INumber&) = default;
	INumber(INumber&&) = default;
	INumber& operator=(const INumber&) = default;

	virtual T& value() = 0;

	virtual bool operator>(const Value& value) override {
		switch (value.type) {
		case ValueType::Int: return this->value() > ((INumber<int>&)value).value();
		case ValueType::Double: return this->value() > ((INumber<double>&)value).value();
		case ValueType::Float: return this->value() > ((INumber<float>&)value).value();
		case ValueType::Long: return this->value() > ((INumber<long>&)value).value();
		}

		throw std::exception("Invalid operatoration");
	}

	virtual bool operator<(const Value& value)  override {
		return false;
	}

	virtual Value& operator+(const Value& value)  override {
		return *this;
	}
};

template<typename T>
struct Number : public INumber<T> {

	T val;


	Number(T value) : val(value) {}
	
	virtual T& value() override {
		return this->val;
	}
};


template<typename T>
struct FieldNumber : public INumber<T> {

	Field& field;

	FieldNumber(Field& field) : field(field) {}
	FieldNumber(const FieldNumber&) = default;
	FieldNumber(FieldNumber&&) = default;
	FieldNumber& operator=(const FieldNumber&) = default;


	virtual T& value() override {
		return field.dataRef<T>();
	}
};





struct GreaterThan : public Predicate {
	std::shared_ptr<Expression> left;
	std::shared_ptr<Expression> right;


	bool operator()() override {
		auto& leftValue = (*left)();
		auto& rightValue = (*right)();
		return leftValue > rightValue;
	}
};

