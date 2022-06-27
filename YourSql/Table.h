#pragma once
#include <iostream>
#include <fstream>
#include <optional>
#include "Row.h"
#include "fields/SchemaFactory.h"
#include "expression/Number.h";
#include "expression/GreaterThan.h"
#include "ExpressionFactory.h"
#include <memory>
#include "Page.h"
#include <unordered_map>
#include "ITable.h"




class Table : public ITable {
	std::shared_ptr<Schema> schema;
	std::string tableName;
	std::unordered_map<size_t, std::shared_ptr<Page>> pages;
	size_t freePage;
public:
	Table(std::string tableName, std::shared_ptr<Schema> schema,
		std::unordered_map<size_t, std::shared_ptr<Page>> pages = {},
		size_t freePage = 0)
		: tableName(tableName), schema(schema), pages(pages), freePage(freePage) {}

	Table(Table&&) = default;

	void insert(Page& page, std::vector<std::string> values) {

		auto row = genRow();
		row.setData(page.getEndData());
		page.addLength(row.getContiguousSize());

		row.updateData(values);
	}

	void insert(Page& page, Row& rowToCopy) {

		auto row = genRow();
		row.setData(page.getEndData());
		page.addLength(row.getContiguousSize());
		row.updateData(rowToCopy);
	}


	void insert(Row& rowToCopy) {
		auto page = getFreePage();
		insert(*page, rowToCopy);
	}

	virtual void insert(Row& rowToCopy, RowConversion& rowConversion) override {
		auto page = getFreePage();
		auto row = genRow();
		row.setData(page->getEndData());
		page->addLength(row.getContiguousSize());
		rowConversion.convert(rowToCopy, row);
	}

	std::shared_ptr<Page> getFreePage() {
		return readPage(freePage);
	}

	void insert(std::vector<std::string> values) override {
		auto page = getFreePage();

		auto row = genRow();
		row.setData(page->getEndData());
		page->addLength(row.getContiguousSize());

		row.updateData(values);
	}

	

	void update(Page& page, size_t index, std::vector<std::string> values) {

		auto row = genRow();

		row.setData(page.getStartData());
		row += index;

		row.updateData(values);
	}


	std::shared_ptr<Page> readPage(int i) {
		auto pageIt = pages.find(i);
		if (pageIt != pages.end()) {
			if (!pageIt->second) {
				auto pageOpt = Page::loadPage(tableName + std::to_string(i) + ".data");
				if (pageOpt) {

					auto page = std::make_shared<Page>(std::move(*pageOpt));
					pages.insert_or_assign(i, page);
					return page;
				}
				return {};
			}
			return pageIt->second;
		}
		else {
			auto page = std::make_shared<Page>();
			pages.insert_or_assign(i, page);
			return page;
		}	
	}

	Row genRow() {
		auto fields = schema->generateFields();
		return Row(std::move(fields));
	}

	std::shared_ptr<Page> nextPage(std::shared_ptr<Page>& page) {
		if (page)
			page = {};
		else
			page = readPage(0);

		return page;
	}

	static std::optional<Table> createTable(std::string tableName);

	Schema& getSchema() { return *schema; }


	struct Iterator : Row {
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = Row;
		using pointer = Row*;  // or also value_type*
		using reference = Row&;  // or also value_type&

		Iterator(Table& table) :table(table), Row(table.getSchema().generateFields()) {
			table.nextPage(page);
			setData(page->getStartData());
		}

		Iterator(Table& table, std::shared_ptr<Page> page, char* data) :table(table),
			page(page), Row(table.getSchema().generateFields()) {
			setData(data);
		}

		Iterator(const Iterator&) = default;
		Iterator(Iterator&&) = default;
		Iterator& operator=(const Iterator&) = default;

		Table& table;
		std::shared_ptr<Page> page;

		reference operator*() { return *this; }
		pointer operator->() { return this; }

		// Prefix increment
		Iterator& operator++() {
			if (page->valid(*this)) {
				this->Row::operator++(1);
			}
			else {
				table.nextPage(page);
				setData(page->getStartData());
			}
			return *this;
		}

		// Postfix increment
		Iterator& operator++(int) { 
			if (page->valid(*this)) {
				this->Row::operator++(1);
			}
			else {
				table.nextPage(page);
				setData(page->getStartData());
			}
			return *this;
		}

		friend bool operator== (const Iterator& a, const Iterator& b) { return a.getData() == b.getData(); };
		friend bool operator!= (const Iterator& a, const Iterator& b) { return a.getData() != b.getData(); };

	};


	std::shared_ptr<Page> lastPage() {
		return readPage(freePage);	
	}

};
