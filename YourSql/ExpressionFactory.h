#pragma once
#include "expression/Expression.h"
#include "Row.h"
#include <memory>
#include "fields/Schema.h"
#include "expression/GreaterThan.h"
#include "expression/FieldNumber.h"
#include "expression/Number.h"
#include "util.h"

struct CreateContext {
	std::vector<Row> rows;

};

struct ExpresionCreator {

	virtual std::shared_ptr<Expression> create(const Row& row) = 0;
	virtual std::shared_ptr<Expression> create(const MultiRow& row) = 0;

};

struct PredicateCreator {

	virtual std::shared_ptr<Predicate> create(const Row& row) = 0;
	virtual std::shared_ptr<Predicate> create(const MultiRow& row) = 0;

};

struct GreaterThanCreator : public PredicateCreator {
	std::shared_ptr<ExpresionCreator> left;
	std::shared_ptr<ExpresionCreator> right;

	virtual std::shared_ptr<Predicate> create(const Row& row) override {
		auto gt = std::make_shared<GreaterThan>();
		gt->left = left->create(row);
		gt->right = right->create(row);
		return gt;
	}

	virtual std::shared_ptr<Predicate> create(const MultiRow& row) override {
		auto gt = std::make_shared<GreaterThan>();
		gt->left = left->create(row);
		gt->right = right->create(row);
		return gt;
	}
};

struct FieldValueCreator : public ExpresionCreator {
	size_t index;

	FieldValueCreator(size_t index) :index(index) {}
	FieldValueCreator(const FieldValueCreator&) = default;
	FieldValueCreator(FieldValueCreator&&) = default;


	std::shared_ptr<Expression> create(const Row& row) {
		return row.getField(index).value();
	}

	std::shared_ptr<Expression> create(const MultiRow& row) {
		return row.getField(index).value();
	}
};


template<typename T>
T stringToValue(const std::string& string);

template<>
inline int stringToValue(const std::string& string) {
	return std::atoi(string.c_str());
}

template<>
inline  long stringToValue(const std::string& string) {
	return std::atol(string.c_str());
}

template<>
inline float stringToValue(const std::string& string) {
	return std::atof(string.c_str());
}

template<>
inline double stringToValue(const std::string& string) {
	return std::atof(string.c_str());
}



template<typename T>
struct ConstValueCreator : public ExpresionCreator {
	T value;

	ConstValueCreator(const std::string& name) : value(stringToValue<T>(name)){}
	ConstValueCreator(const ConstValueCreator&) = default;
	ConstValueCreator(ConstValueCreator&&) = default;


	virtual std::shared_ptr<Expression> create(const Row& row) override {
		return std::make_shared<Number<T>>(value);
	}

	virtual std::shared_ptr<Expression> create(const MultiRow& row) override {
		return std::make_shared<Number<T>>(value);
	}
};

class PredicateCreatorFactory
{
public:
	std::shared_ptr<PredicateCreator> create(std::string expr, const Schema& schema) const {
		//example
		//age > 5
		auto gtIndex = expr.find_first_of(">");
		auto creator = std::make_unique<GreaterThanCreator>();

		auto left = expr.substr(0, gtIndex);

		{
			trim(left);

			auto fieldIndexOpt = schema.indexOf(left);
			if (!fieldIndexOpt) {
				throw std::exception("No field");
			}
			creator->left = std::make_shared<FieldValueCreator>(*fieldIndexOpt);
		}

		{
			auto right = expr.substr(gtIndex + 1);
			trim(right);
			auto fieldIndexOpt = schema.indexOf(right);
			if (fieldIndexOpt) {
				creator->right = std::make_shared<FieldValueCreator>(*fieldIndexOpt);
			}
			else {
				creator->right = std::make_shared<ConstValueCreator<int>>(right);

			}
		}


		return creator;
	}
};

