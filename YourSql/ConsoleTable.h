#pragma once
#include "ITable.h"
#include "Page.h"
#include "Row.h"
#include "fields/Schema.h"
#include <iostream>

class ConsoleTable : public ITable
{

	std::shared_ptr<Schema> schema;
	std::unique_ptr<Page> page;
	Row row;

public:
	ConsoleTable(std::shared_ptr<Schema> schema)
		: schema(schema), row(schema->generateFields()), page(new Page()) {
		row.setData(page->getStartData());
	}

	ConsoleTable(ConsoleTable&&) = default;

	virtual Schema& getSchema() override;


	virtual void insert(Row& rowToCopy) {
		std::cout << rowToCopy.toString() << std::endl;
	}
	virtual void insert(std::vector<std::string> values) {
		for (size_t i = 0; i < values.size(); i++) {
			std::cout << values[i];

			if (i + 1 != values.size())
				std::cout << ",";
		}
		std::cout << std::endl;
	}

	virtual void insert(Row& rowToCopy, RowConversion& rowConversion) override {
		rowConversion.convert(rowToCopy, row);
		std::cout << row.toString() << std::endl;
	}

};

