#pragma once
#include <vector>
#include <memory>
#include "Field.h"
#include "SchemaField.h"
#include <unordered_map>
#include <optional>

class Schema {
	std::vector<std::shared_ptr<SchemaField>> fields;
	std::unordered_map<std::string, size_t> fieldsByName;
	std::vector<std::string> indexToName;
public:
	Schema(std::vector<std::shared_ptr<SchemaField>> fields, std::vector<std::string> names):fields(fields) {
		size_t i = 0;
		indexToName.reserve(names.size());
		for (auto& name : names) {
			fieldsByName[name] = i;
			indexToName.emplace_back(name);
			i++;
		}
	}

	Schema(std::vector<Schema*> schemas, std::vector<std::string> tableNames) {
		size_t j = 0;
		for (auto schema : schemas) {
			size_t i = 0;
			for (auto& field : schema->fields) {
				auto name = tableNames[j] + "." + schema->nameOf(i);
				fieldsByName.insert_or_assign(name, fields.size());
				fields.push_back(field);
				indexToName.emplace_back(name);
				i++;
			}
			j++;
		}
	}



	std::vector<std::unique_ptr<Field>> generateFields() const {
		std::vector<std::unique_ptr<Field>> fields;
		fields.reserve(this->fields.size());
		size_t offset = 0;
		for (auto& fieldSchema : this->fields) {
			fields.push_back(fieldSchema->create());
			fields.back()->setOffset(offset);
			offset += fieldSchema->contiguousSize();
		}
		return fields;
	}

	std::vector<std::shared_ptr<SchemaField>>& getSchemaFields() {
		return fields;
	}

	std::optional<size_t> indexOf(const std::string& name) const {
		auto it = fieldsByName.find(name);
		if (it != fieldsByName.end())
			return it->second;
		return {};
	}

	std::string nameOf(size_t index) const {
		return indexToName[index];
	}

};

