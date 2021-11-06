#pragma once 
#include "INumber.h"
#include "../fields/Field.h"

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

	//virtual std::unique_ptr<Value> operator+(const Value& value)  override {

	//	/*switch (value.type) {
	//	case ValueType::Int: return INumber(this->value() + ((INumber<int>&)value).value());
	//	case ValueType::Double: return INumber(this->value() + ((INumber<double>&)value).value());
	//	case ValueType::Float: return INumber(this->value() + ((INumber<float>&)value).value());
	//	case ValueType::Long: return INumber(this->value() + ((INumber<long>&)value).value());
	//	}*/
	//	return {};

	//	//throw std::exception("Invalid operation");
	//}

	void addSelf(const Value& value) override {
		*field.data<T>() += ((FieldNumber<T>&)value).value();
	}

	virtual void copySelf(const Value& value) override {
		*field.data<T>() = ((FieldNumber<T>&)value).value();
	}
};