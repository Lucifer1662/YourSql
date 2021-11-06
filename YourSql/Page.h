#pragma once
#include <vector>
#include <optional>
#include <fstream>
struct Row;
struct Predicate;

class Page
{
	char* data;
	int length;
	std::string pagename;
public:

	static const size_t SIZE;
	Page(std::vector<char>&& data, std::string pagename): pagename(pagename){
		this->data = (char*)malloc(SIZE);
		memcpy(this->data, data.data(), data.size());
		length = data.size();
	}
	Page(std::vector<char> data, std::string pagename) : pagename(pagename) {
		this->data = (char*)malloc(SIZE);
		memcpy(this->data, data.data(), data.size());
		length = data.size();
	}
	Page() {
		this->data = (char*)malloc(SIZE);
		length = 0;
	}
	Page(Page&& page) : data(page.data), pagename(page.pagename), length(page.length)  {
		page.data = nullptr;
	}

	char* getStartData() {
		return (char*)data;
	}

	char* getEndData() {
		return data + length;
	}

	void save();

	void addLength(size_t amount) {
		this->length += amount;
	}

	static std::optional<Page> loadPage(const std::string& filename);

	~Page() {
		free(data);
	}

	bool next(Row& row, Predicate& predicate);
	bool valid(Row& row);
	
};



