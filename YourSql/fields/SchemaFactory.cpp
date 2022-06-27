#include "SchemaFactory.h"
#include "../TableFactory.h"
#include "../sql/SqlFactory.h"

std::shared_ptr<Schema> SchemaFactory::createFromSQL(const SelectExpression& expression, RowConversion& rowConversion) {


	auto table = TableFactory::getInstance().getTable(expression.tableNames[0]);

	std::vector<std::shared_ptr<SchemaField>> schemaField;
	std::vector<std::string> names;
	names.reserve(expression.fields.size());
	schemaField.reserve(expression.fields.size());
	rowConversion.fromTo.reserve(expression.fields.size());

	auto& tableFields = table->getSchema().getSchemaFields();


	for (auto& field : expression.fields) {
		if (field == "*") {
			int i = 0;
			for (auto& tableField : tableFields) {
				schemaField.push_back(tableField);
				names.push_back(table->getSchema().nameOf(i));
				rowConversion.fromTo.push_back(i);
				i++;
			}
		}
		else {
			auto index = table->getSchema().indexOf(field);
			if (index) {
				schemaField.push_back(tableFields[*index]);
				rowConversion.fromTo.push_back(*index);
			}
			else {
				throw std::exception("Invalid");
			}
		}
	}

	return std::make_shared<Schema>(Schema(schemaField,expression.fields));
}
