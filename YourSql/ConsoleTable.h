#pragma once
#include "ITable.h"

class ConsoleTable : public ITable
{
	// Inherited via ITable
	virtual Schema& getSchema() override;
};

