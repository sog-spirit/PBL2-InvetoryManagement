#pragma once

#include "Product.h"
#include <vector>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <stdlib.h>
#include <string>

class Database {
#define SQL_RESULT_LEN 2048
#define SQL_RETURN_CODE_LEN 2048

private:
	SQLHANDLE sqlConnectionHandle;
	SQLHANDLE sqlStatementHandle;
	SQLHANDLE sqlEnvironmentHandle;
	SQLWCHAR* inConnectionString = (SQLWCHAR*) L"DRIVER={SQL Server}; SERVER=LAPTOP-T6EK2SH6; DATABASE=PBL2; trusted=true;";
	SQLWCHAR outConnectionString[SQL_RESULT_LEN];

public:
	void InitializeDatabaseInstance();
	void DestroyDatabaseInstance();
	int CreateInvoice();
	void GetInvoices();
	void GetCategories();
	void AddProduct();
	void GetProducts();
	void RemoveProduct(int productId);
	void AddProductToInvoice(std::string productId, std::string quantity);
	void GetTotalSalesByYear(std::string year);
	void ProductMergeSortIntervals(std::vector<Product> &productList, int start, int middle, int end, bool (*compareFunction)(Product &, Product &));
	void ProductMergeSort(std::vector<Product>& productList, int start, int end, bool (*compareFunction)(Product &, Product &));
	void SortProductByPrice();
	static bool SortDescending(Product &product_a, Product &product_b);
	static bool SortAscending(Product& product_a, Product& product_b);
};