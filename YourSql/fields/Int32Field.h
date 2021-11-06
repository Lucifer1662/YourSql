#pragma once
#include "Field.h"
#include <string>
#include "SchemaField.h"
#include <memory>
#include "../expression/FieldNumber.h"



class Int32Field : public Field {

public:
	std::string toString() override {
		return std::to_string(*data<int>());
	}

	virtual size_t contiguousSize() override {
		return sizeof(int);
	}

	virtual void setData(const std::string& string) override {
		int num = std::stoi(string);
		*data<int>() = num;
	};

	virtual std::shared_ptr<Value> value() override {
		return std::make_shared<FieldNumber<int>>(*this);
	}
};

class Int32SchemaField : public SchemaField {

public:
	virtual std::unique_ptr<Field> create() {
		return std::make_unique<Int32Field>();
	}

	virtual size_t contiguousSize() override {
		return sizeof(int);
	}
};
