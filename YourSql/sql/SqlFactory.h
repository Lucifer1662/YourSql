#include <memory>
#include "../Table.h"
#include "../ExpressionFactory.h"


struct SqlStatement {
	
	virtual void operator()() = 0;
};

struct InsertStatement : public SqlStatement {
	Table table;
	std::vector<std::string> values;


	void operator()() {
		table.insert(values);
	}

};


struct UpdateStatement : public SqlStatement {
	Table table;
	std::vector<std::string> values;
	std::shared_ptr<PredicateCreator> predCreator;


	void operator()() {
		table.insert(values);
	}

};

struct SelectStatement : public SqlStatement {
	std::shared_ptr<Table> table;
	std::shared_ptr<Table> outputTable;
	std::shared_ptr<PredicateCreator> predCreator;

	SelectStatement(std::shared_ptr<Table> table,
		std::shared_ptr<Table> outputTable,
		std::shared_ptr<PredicateCreator> predCreator) :
		table(table), outputTable(outputTable), predCreator(predCreator) {}


	void operator()() {

		Row row = table->genRow();
		auto pred = predCreator->create(row);

		
		std::shared_ptr<Page> page;
			
		

		while (table->nextPage(page)) {
			row.setData(page->getStartData());

			while (page->next(row, *pred)) {
				outputTable->insert(row);
				row++;
			}
		}

	}

};


class SqlFactory {


public:


};