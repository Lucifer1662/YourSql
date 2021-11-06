#pragma once
#include "Predicate.h"
#include "Expression.h"


struct GreaterThan : public Predicate {
	std::shared_ptr<Expression> left;
	std::shared_ptr<Expression> right;


	bool operator()() override {
		auto& leftValue = (*left)();
		auto& rightValue = (*right)();
		return leftValue > rightValue;
	}
};
