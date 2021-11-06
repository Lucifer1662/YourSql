#pragma once
#include <optional>
#include <fstream>
#include <string>
#include "Schema.h"
#include <vector>
#include "Int32Field.h"
#include "FixedStringField.h"
#include <memory>

class SchemaFactory {
public:
	std::shared_ptr<Schema> create(std::string filename) {
		return std::make_shared<Schema>(Schema(
			{
				std::make_shared<FixedStringSchemaField>(20),
				std::make_shared<Int32SchemaField>(),
			},
			{"name", "age"}
			)
		);
	}

	std::shared_ptr<Schema> createFromSQL(std::string sql) {
		return std::make_shared<Schema>(Schema(
			{
				std::make_shared<FixedStringSchemaField>(20),
				std::make_shared<Int32SchemaField>(),
			},
			{ "name", "age" }
			)
			);
	}
};