#include "Page.h"
#include "Row.h"
#include "expression/Predicate.h"

const size_t Page::SIZE = 4024;


bool Page::next(Row& row, Predicate& predicate) {
	auto end = getEndData();
	if (row.getData() < end) {
		if (predicate()) {
			return true;
		}
		else {
			row++;
		}
	}
	return false;
}

bool Page::valid(Row& row) {
	return row.getData() < getEndData();
}


void Page::save() {
	std::ofstream file(pagename, std::ios::binary);

	if (file.is_open()) {
		file.write((char*)data, length);
		file.close();
	}

}

std::optional<Page> Page::loadPage(const std::string& filename) {
	std::ifstream file(filename, std::ios::binary);

	if (file.is_open()) {
		std::vector<char> data(std::istreambuf_iterator<char>(file), {});
		file.close();

		return Page(data, filename);
	}
	return {};
}


