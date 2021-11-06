#pragma once
#include <memory>
#include <unordered_map>
#include "Table.h"
#include <string>

class TableFactory
{
	static std::unique_ptr<TableFactory> instance;
	std::unordered_map<std::string, std::shared_ptr<Table>> dbTables;
public:
	static TableFactory& getInstance() {
		if (!instance) {
			instance = std::make_unique<TableFactory>();
		}
		return *instance;
	}

	std::shared_ptr<Table> getTable(const std::string& tableName) {
		auto tableIt = dbTables.find(tableName);
		if (tableIt != dbTables.end()) {
			return tableIt->second;
		}

		auto optSchema = SchemaFactory().create(tableName + ",schema");
		if (optSchema) {
			auto table = std::make_shared<Table>(Table(tableName, optSchema, { {0,{}} }, 0));
			dbTables.insert_or_assign(tableName, table);
			return table;
		}
		return {};
	}
};

