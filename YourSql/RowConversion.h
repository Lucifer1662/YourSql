#pragma once
#include <vector>
#include "Row.h"

struct RowConversion {
	std::vector<size_t> fromTo;

	void convert(Row& from, Row& to) {
		for (size_t i = 0; i < fromTo.size(); i++)
		{
			to.getField(i).copyFromSelf(from.getField(fromTo[i]));
		}
	}


};


