#pragma once
#include "Field.h"
#include <string>
#include <memory>
#include "../expression/FieldNumber.h"

class FixedStringField : public Field {

	size_t maxLength = 0;
public:
	FixedStringField(size_t maxLength) : maxLength(maxLength) {}


	std::string toString() override {
		size_t length = 0;
		char* str = getData();
		while (str[length] != 0 && length <= maxLength ) {
			length++;
		}
		return std::string((char*)getData(), length);
	}

	virtual size_t contiguousSize() override {
		return maxLength;
	}

	virtual void setData(const std::string& str) override {

		int i = 0;
		char* d = getData();
		while (i<maxLength && i <str.size()) {
			d[i] = str[i];
			i++;
		}
		if (i < maxLength) {
			d[i] = 0;
		}

	};

	virtual std::shared_ptr<Value> value() override {
		return std::make_shared<FieldNumber<int>>(*this);
	}
};


class FixedStringSchemaField : public SchemaField {
	size_t length;

public:
	FixedStringSchemaField(size_t length) : length(length) {}

	virtual std::unique_ptr<Field> create() override {
		return std::make_unique<FixedStringField>(length);
	}

	virtual size_t contiguousSize() override {
		return length;
	}


	
};
