#pragma once
#include "Field.h"
#include <memory>

class SchemaField {
public:
	virtual std::unique_ptr<Field> create() = 0;
	virtual size_t contiguousSize() = 0;
};
