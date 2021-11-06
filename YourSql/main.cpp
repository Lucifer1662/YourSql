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





void insert() {

}

void main() {


	auto table = TableFactory::getInstance().getTable("user");
	if (table) {
		


		{
			auto sqlSchema = SchemaFactory().createFromSQL("name, age");
			auto outputTable = std::make_shared<Table>("", sqlSchema);
			auto predCreator = PredicateCreatorFactory().create("age > 15", table->getSchema());

			SelectStatement select(table, outputTable, predCreator);

			select();

			for (auto& row : *outputTable) {
				std::cout << row.getField(0).toString() << "," << row.getField(1).toString() << std::endl;
			}

			/*Row row = table->genRow();

			std::shared_ptr<Page> page;
			while (outputTable->nextPage(page)) {
				row.setData(page->getStartData());

				while (page->valid(row)) {
					std::cout << row.getField(0).toString() << "," << row.getField(1).toString() << std::endl;
					row++;
				}
			}*/
		}



		auto page = table->getFreePage();


		table->insert(*page, { "Hello world", "10"});
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
