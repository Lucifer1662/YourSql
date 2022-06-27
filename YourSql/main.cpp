#include <iostream>
#include <fstream>
#include "Page.h"
#include <optional>
#include "Row.h"
#include "fields/SchemaFactory.h"
#include "expression/Number.h";
#include "expression/GreaterThan.h"
#include "ExpressionFactory.h"
#include <memory>
#include "Table.h"
#include "sql/SqlFactory.h"
#include "TableFactory.h"
#include "ConsoleTable.h"
#include "util.h"



void insert() {

}

void splitString(std::string_view str, char delim, std::vector<std::string>& result) {
	auto commaIndex = str.find_first_of(delim);

	while (commaIndex != std::string_view::npos) {
		result.push_back(std::string(str.substr(0, commaIndex)));
		str = str.substr(commaIndex+1);
		commaIndex = str.find_first_of(delim);
	}
	result.push_back(std::string(str));
}


void main() {

	std::string sql = "select age,* from user where age > 0";
	auto str = std::string_view(sql);
	trim(str);
	auto firstSpace = sql.find_first_of(' ');
	auto queryTypeString = str.substr(0, firstSpace);

	auto d1 = std::make_unique<int>(1);
	auto d2 = std::make_unique<int>(*d1);



	if (queryTypeString == "select") {
		str = str.substr(firstSpace);

		auto fromKeyword = std::string(" from");
		auto fromIndex = str.find(fromKeyword);
		auto argsString = str.substr(0, fromIndex);
		trim(argsString);
		SelectExpression expr;

		//process select args
		splitString(argsString, ',', expr.fields);


		


		auto fromString = str.substr(fromIndex);
		auto whereKeyword = std::string(" where");
		auto whereIndex = fromString.find(whereKeyword);
		auto tableNames = fromString.substr(fromKeyword.size(), whereIndex - fromKeyword.size());
		trim(tableNames);

		auto insertIntoKeyword = std::string("inner join");
		auto innerJoinIndex = str.find(insertIntoKeyword);
		if (innerJoinIndex != std::string_view::npos) {

		}


		splitString(tableNames, ',', expr.tableNames);
		auto table = TableFactory::getInstance().getTable(expr.tableNames[0]);

		auto whereString = fromString.substr(whereIndex + whereKeyword.size());
		trim(whereString);
		expr.whereClause = whereString;

		auto predCreator = PredicateCreatorFactory().create(expr.whereClause, table->getSchema());



		auto rowConverstion = std::make_shared<RowConversion>();
		auto schema = SchemaFactory().createFromSQL(expr, *rowConverstion);
		auto outputTable = std::make_shared<ConsoleTable>(schema);


		auto select = SelectStatement(table, outputTable, predCreator, rowConverstion);
		select();
		int w = 0;
	}

	{

		auto leftTable = TableFactory::getInstance().getTable("user");
		auto rightTable = TableFactory::getInstance().getTable("user");

		std::vector<std::string> names = { std::string("user1"), std::string("user2") };
		std::vector<Schema*> schemas = { &leftTable->getSchema(), &rightTable->getSchema() };
		auto combinedSchema = Schema(schemas, names);

		auto predCreator = PredicateCreatorFactory().create("user1.age > user2.age ", combinedSchema);


		InnerJoin join(leftTable, rightTable, predCreator);

		join.query([](auto& row) { std::cout << row.toString() << std::endl;	});
		int w = 0;
	}
	/*{
	expr.tableName = "user";
		expr.whereClause = "age > 15";
		expr.fields = { "*" };
		auto outputTable = std::make_shared<ConsoleTable>(sqlSchema);

		auto table = TableFactory::getInstance().getTable(expression.tableName);

		auto schema = SchemaFactory().createFromSQL(expression);
		auto predCreator = PredicateCreatorFactory().create(expression.whereClause, table->getSchema());

		SqlFactory().createStatement(expr, )
		
	}*/

	


	auto table = TableFactory::getInstance().getTable("user");
	if (table) {







		auto page = table->getFreePage();


		table->insert(*page, { "Hello world", "10" });
		table->update(*page, 0, { "ollo world", "20" });


		if (page) {
			Row row = table->genRow();
			row.setData(page->getStartData());
			auto end = page->getEndData();

			auto creator = PredicateCreatorFactory().create(("age > 15"), table->getSchema());

			auto pred = creator->create(row);

			while (page->next(row, *pred)) {
				std::cout << row.getField(0).toString() << "," << row.getField(1).toString() << std::endl;
				row++;
			}

			//sum a page
			Page sumPage;
			Row sumRow = table->genRow();
			sumRow.setData(sumPage.getStartData());
			row.setData(page->getStartData());

			size_t sumIndex = 1;

			auto sum = sumRow.getField(sumIndex).value();

			if (page->next(row, *pred)) {
				sum->copySelf(*row.getField(sumIndex).value());
				row++;

				while (page->next(row, *pred)) {
					sum->addSelf(*row.getField(sumIndex).value());

					row++;
				}
			}




			std::cout << sumRow.toString() << std::endl;





			row.setData(page->getStartData());


			while (row.getData() < end) {
				std::cout << row.getField(0).toString() << "," << row.getField(1).toString() << std::endl;
				row++;
			}
			page->save();
		}

	}





	int i = 0;


}
