#include "Field.h"

void Field::setBase(char** base) {
	this->base = base;
}

void Field::setOffset(size_t offset) {
	this->offset = offset;
}

char* Field::getData() const {
	return *base + offset;
}

char** Field::getBase() const {
	return base;
}

size_t Field::getOffset() const {
	return offset;
}

Field::~Field() {}
