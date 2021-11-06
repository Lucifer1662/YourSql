#pragma once
#include <vector>
#include "fields/Field.h"
#include <memory>



class Row
{
	std::unique_ptr<char*> data;
	std::vector<std::unique_ptr<Field>> fields;
	std::vector<size_t> offsets;
	size_t contiguousSize;
public:
	Row(std::vector<std::unique_ptr<Field>>&& fields) : fields(std::move(fields)) {
		auto& back = this->fields.back();
		this->contiguousSize = back->getOffset() + back->contiguousSize();		
	}

	void setData(char* data) {
		this->data = std::make_unique<char*>(data);
		for (auto& field : fields) {
			field->setBase(this->data.get());
		}
	}

	void* getData() const {
		return *data;
	}

	Field& getField(size_t fieldIndex) const {
		return *this->fields[fieldIndex];
	}

	size_t getContiguousSize() const {
		return contiguousSize;
	};

	Row& operator++(int) {
		*this->data += contiguousSize;
		return *this;
	}

	Row& operator+=(int amount) {
		*this->data += contiguousSize * amount;
		return *this;
	}

	bool validData(std::vector<std::string> values) const {
		int i = 0;
		for (auto& value : values) {
			if (!getField(i).validData(value)) {
				return false;
			}
			i++;
		}
		return true;
	}

	bool updateData(std::vector<std::string> values) {
		if (!validData(values)) {
			return false;
		}

		int i = 0;
		for (auto& value : values) {
			getField(i).setData(value);
			i++;
		}
		return true;
	}

	bool updateData(Row& row) {
		memcpy(*data, row.getData(), contiguousSize);
		return true;
	}

	std::string toString() {
		std::string string;
		for (size_t i = 0; i < fields.size(); i++) {
			string += getField(i).toString();
			
			if (i + 1 != fields.size())
				string += ",";
		}
		return string;
	}
	
};

