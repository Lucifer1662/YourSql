#include <memory>
#include "../Table.h"
#include "../ExpressionFactory.h"
#include "../TableFactory.h"
#include "../RowConversion.h"

struct SqlStatement {

	virtual void operator()() = 0;
};

struct InsertStatement : public SqlStatement {
	std::shared_ptr<ITable> table;
	std::vector<std::string> values;


	void operator()() {
		table->insert(values);
	}

};


struct UpdateStatement : public SqlStatement {
	std::shared_ptr<ITable> table;
	std::vector<std::string> values;
	std::shared_ptr<PredicateCreator> predCreator;


	void operator()() {

		for (auto& row : *table) {
			row.updateData(values);
		}
	}

};


struct SelectStatement : public SqlStatement {
	std::shared_ptr<ITable> table;
	std::shared_ptr<ITable> outputTable;
	std::shared_ptr<PredicateCreator> predCreator;
	std::shared_ptr<RowConversion> rowConversion;

	SelectStatement(std::shared_ptr<ITable> table,
		std::shared_ptr<ITable> outputTable,
		std::shared_ptr<PredicateCreator> predCreator,
		std::shared_ptr<RowConversion> rowConversion
		) :
		table(table), outputTable(outputTable), predCreator(predCreator), rowConversion(rowConversion){}


	void operator()() {

		auto it = table->begin();
		auto pred = predCreator->create(it);
		for (; it != table->end(); it++) {
			if ((*pred)()) {
				outputTable->insert(it, *rowConversion);
			}
		}


	}



};

struct Join {
	std::shared_ptr<ITable> left;
	std::shared_ptr<ITable> right;
	std::shared_ptr<PredicateCreator> predCreator;

	void next() {
		
	}
};

struct InnerJoin {
	std::shared_ptr<ITable> left;
	std::shared_ptr<ITable> right;
	std::shared_ptr<PredicateCreator> predCreator;
	InnerJoin(std::shared_ptr<ITable> left,
		std::shared_ptr<ITable> right,
		std::shared_ptr<PredicateCreator> predCreator) : left(left), right(right), predCreator(predCreator) {}

	template<typename Func>
	void query(Func&& func) {
		auto it = left->begin();
		auto itr = right->begin();

		MultiRow row({ &it, &itr });

		auto pred = predCreator->create(row);
		for (; it != left->end(); it++) {
			for (; itr != right->end(); itr++) {
				if ((*pred)()) {
					func(row);
				}
			}
		}
	}
};

//struct SelectStatementJoins : public SqlStatement {
//	std::vector<std::shared_ptr<ITable>> tables;
//	std::shared_ptr<ITable> outputTable;
//	std::shared_ptr<PredicateCreator> predCreator;
//	std::shared_ptr<RowConversion> rowConversion;
//
//
//	SelectStatementJoins(std::vector<std::shared_ptr<ITable>> tables,
//		std::shared_ptr<ITable> outputTable,
//		std::shared_ptr<PredicateCreator> predCreator,
//		std::shared_ptr<RowConversion> rowConversion
//	) :
//		tables(tables), outputTable(outputTable), predCreator(predCreator), rowConversion(rowConversion) {}
//
//
//	void operator()() {
//
//		auto it = table->begin();
//		auto pred = predCreator->create(it);
//		for (; it != table->end(); it++) {
//			if ((*pred)()) {
//				outputTable->insert(it, *rowConversion);
//			}
//		}
//
//
//	}
//
//
//
//};


enum class StatementType {
	SELECT,
	UPDATE,
	INSERT,
	DELETE
};



struct SelectExpression {
	bool isDistinct = false;
	std::vector<std::string> fields;
	std::vector<std::string> tableNames;
	std::string joins;
	std::string whereClause;
};

struct SelectExpr {
	std::shared_ptr<Schema> outSchema;
	std::shared_ptr<ITable> table;


};


class SqlFactory {


public:

	std::shared_ptr<SqlStatement> createStatement(const SelectExpression& expression, std::shared_ptr<Table> outputTable) {
		/*auto table = TableFactory::getInstance().getTable(expression.tableNames[0]);

		auto schema = SchemaFactory().createFromSQL(expression);
		auto predCreator = PredicateCreatorFactory().create(expression.whereClause, table->getSchema());


		return std::make_shared<SelectStatement>(table, outputTable, predCreator);*/
	}
};