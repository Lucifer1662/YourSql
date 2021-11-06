#pragma once
#include "Page.h"
#include <vector>
#include <string>
#include "Row.h"
#include "fields/Schema.h"



struct ITable
{

	virtual void insert(Page& page, std::vector<std::string> values) {}
	virtual void insert(Page& page, Row& rowToCopy) {}
	virtual void insert(Row& rowToCopy) {}
	virtual void insert(std::vector<std::string> values) {}

	virtual void update(Page& page, size_t index, std::vector<std::string> values) {}


	virtual Schema& getSchema() = 0;

	virtual std::shared_ptr<Page> readPage(int i) { return {}; }
	virtual std::shared_ptr<Page> nextPage(std::shared_ptr<Page>& page) { return {}; };
	virtual std::shared_ptr<Page> lastPage() { return {}; }

	struct Iterator : Row {
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = Row;
		using pointer = Row*;  // or also value_type*
		using reference = Row&;  // or also value_type&

		Iterator(ITable& table) :table(table), Row(table.getSchema().generateFields()) {
			table.nextPage(page);
			setData(page->getStartData());
		}

		Iterator(ITable& table, std::shared_ptr<Page> page, char* data) :table(table),
			page(page), Row(table.getSchema().generateFields()) {
			setData(data);
		}

		Iterator(const Iterator&) = default;
		Iterator(Iterator&&) = default;
		Iterator& operator=(const Iterator&) = default;

		ITable& table;
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


	virtual Iterator begin() { return Iterator(*this); }
	virtual Iterator end() {
		auto page = lastPage();
		return Iterator(*this, page, page->getEndData());
	};



};



