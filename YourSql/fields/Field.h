#pragma once
#include <string>
#include <memory>
#include "../expression/Value.h"


class Field
{
	size_t offset = 0;
	char** base = 0;

public:

	
	void setBase(char** base);

	void setOffset(size_t offset);

	char* getData() const;

	char** getBase() const;

	size_t getOffset() const;


	virtual ~Field();
	virtual bool validData(const std::string& string) { return true; };


	virtual std::string toString() = 0;

	virtual void setData(const std::string& string) = 0;

	virtual size_t contiguousSize() = 0;


	virtual std::shared_ptr<Value> value() = 0;


	template<typename T>
	T* data() const {
		return (T*)getData();
	}


	template<typename T>
	T& dataRef() const {
		return *data<T>();
	}
};

