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
public:
	Schema(std::vector<std::shared_ptr<SchemaField>> fields, std::vector<std::string> names):fields(fields) {
		size_t i = 0;
		for (auto& name : names) {
			fieldsByName[name] = i;
			i++;
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

	std::optional<size_t> indexOf(const std::string name) const {
		auto it = fieldsByName.find(name);
		if (it != fieldsByName.end())
			return it->second;
		return {};
	}

};

