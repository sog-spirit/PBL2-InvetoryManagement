#pragma once
#include <iostream>
#include "Database.h"

class Program {
public:
	enum USER_CHOICE {
		CREATE_INVOICE = 1,
		GET_PRODUCT_LIST = 2,
		GET_INVOICE_LIST = 3,
		ADD_PRODUCT = 4,
		REMOVE_PRODUCT = 5,
		TOTAL_SALES = 6,
		SORT_PRODUCT = 7,
		EXIT_PROGRAM = 8
	};

	void static MainProgram();
	int static Menu();
	void static AddProduct(Database &instance);
};