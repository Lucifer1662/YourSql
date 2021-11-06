#pragma once
#include <memory>
#include <exception>

struct Value;
struct Expression
{
	virtual Value& operator()() = 0;
};
