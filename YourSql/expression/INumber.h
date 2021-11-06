#pragma once
#include "Value.h"

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

	

};
