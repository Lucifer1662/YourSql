#pragma once
#include <memory>
#include <exception>
#include "INumber.h"
#include "NumberOperations.h"







template<typename T>
struct Number : public INumber<T> {
	T val;

	Number(T value) : val(value) {}
	
	virtual T& value() override {
		return this->val;
	}




	/*virtual std::unique_ptr<Value> operator+(const Value& value)  override {
		return add(*this, value);
	}*/

	void addSelf(const Value& value) {
		val += ((Number<T>&)value).value();
	}

	virtual void copySelf(const Value& value) override {
		val = ((FieldNumber<T>&)value).value();
	}


};






