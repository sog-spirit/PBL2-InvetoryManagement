#include "Database.h"
#include "Product.h"
#include <iostream>
#include <vector>
#include <conio.h>
#include <string.h>

void Database::InitializeDatabaseInstance() {
    sqlConnectionHandle = NULL;
    sqlStatementHandle = NULL;

    if (SQL_SUCCESS != SQLAllocHandle(
        SQL_HANDLE_ENV,
        SQL_NULL_HANDLE,
        &sqlEnvironmentHandle
    )) {
        std::cout << "ERROR alloc sqlEnvironmentHandle.\n";
        DestroyDatabaseInstance();
    }

    if (SQL_SUCCESS != SQLSetEnvAttr(
        sqlEnvironmentHandle,
        SQL_ATTR_ODBC_VERSION,
        (SQLPOINTER) SQL_OV_ODBC3,
        0
    )) {
        std::cout << "ERROR set sqlEnvironmentHandle.\n";
        DestroyDatabaseInstance();
    }

    if (SQL_SUCCESS != SQLAllocHandle(
        SQL_HANDLE_DBC,
        sqlEnvironmentHandle,
        &sqlConnectionHandle
    )) {
        std::cout << "ERROR alloc sqlConnectionHandle.\n";
        DestroyDatabaseInstance();
    }

    std::cout << "Connecting to SQL Server...\n";

    switch (SQLDriverConnect(
        sqlConnectionHandle,
        NULL,
        inConnectionString,
        SQL_NTS,
        outConnectionString,
        2048,
        NULL,
        SQL_DRIVER_NOPROMPT
    )) {
    case SQL_SUCCESS:
        std::cout << "Connect to SQL Server successfully.\n";
        break;

    case SQL_SUCCESS_WITH_INFO:
        std::cout << "Connect to SQL Server successfully.\n";
        break;

    case SQL_INVALID_HANDLE:
        std::cout << "ERROR connect to SQL Server.\n";
        DestroyDatabaseInstance();
        break;

    case SQL_ERROR:
        std::cout << "ERROR connect to SQL Server.\n";
        DestroyDatabaseInstance();
        break;

    default:
        std::cout << "ERROR connect to SQL Server.\n";
        DestroyDatabaseInstance();
        break;
    }

    if (SQL_SUCCESS != SQLAllocHandle(
        SQL_HANDLE_STMT,
        sqlConnectionHandle,
        &sqlStatementHandle
    )) {
        std::cout << "ERROR alloc sqlStatementHandle\n";
        DestroyDatabaseInstance();
    }
}

void Database::DestroyDatabaseInstance() {
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStatementHandle);
    SQLDisconnect(sqlConnectionHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, sqlConnectionHandle);
    SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvironmentHandle);
    std::cout << "\nDatabase instance destroyed successfully.\n";
    std::cout << "Closing program.\n";
    exit(0);
}

int Database::CreateInvoice() {
    SQLCHAR* query = (SQLCHAR*)"INSERT INTO INVOICE DEFAULT VALUES";

    return SQLExecDirectA(sqlStatementHandle, query, SQL_NTS);
}

void Database::GetInvoices() {
    SQLCHAR* query = (SQLCHAR*)"SELECT InvoiceID, INVOICE_DETAIL.ProductID, ProductName, Quantity, Price FROM INVOICE_DETAIL INNER JOIN PRODUCT ON INVOICE_DETAIL.ProductID = PRODUCT.ProductID";
    std::cout << "\nGetting all invoice informations.\n";

    if (SQL_SUCCESS != SQLExecDirectA(sqlStatementHandle, query, SQL_NTS)) {
        std::cout << "An error has occurred while getting all invoice informations.\n";
    }
    else {
        SQLINTEGER ptrSqlVersion;
        int invoiceId, productId, quantity, price;
        char productName[50];
        while (SQLFetch(sqlStatementHandle) == SQL_SUCCESS) {
            SQLGetData(sqlStatementHandle, 1, SQL_C_DEFAULT, &invoiceId, 1, &ptrSqlVersion);
            SQLGetData(sqlStatementHandle, 2, SQL_C_DEFAULT, &productId, 1, &ptrSqlVersion);
            SQLGetData(sqlStatementHandle, 3, SQL_C_CHAR, productName, sizeof(productName), &ptrSqlVersion);
            SQLGetData(sqlStatementHandle, 4, SQL_C_DEFAULT, &quantity, 1, &ptrSqlVersion);
            SQLGetData(sqlStatementHandle, 5, SQL_C_DEFAULT, &price, 1, &ptrSqlVersion);
            std::cout << "\nInvoice ID: " << invoiceId << "\nProduct ID: " << productId << "\nProduct name: " << productName << "\nQuantity: " << quantity << "\nPrice: " << price << "\n";
        }
    }
    SQLCancel(sqlStatementHandle);
}

void Database::GetCategories() {
    SQLWCHAR* query = (SQLWCHAR*)L"SELECT * FROM CATEGORY";

    std::cout << "\nGetting all categories...\n";
    if (SQL_SUCCESS != SQLExecDirect(
        sqlStatementHandle,
        query,
        SQL_NTS
    )) {
        std::cout << "An error has occurred while getting all categories...\n";
    }
    else {
        SQLINTEGER ptrSqlVersion;
        int categoryId;
        char categoryName[50];
        while (SQLFetch(sqlStatementHandle) == SQL_SUCCESS) {
            SQLGetData(sqlStatementHandle, 1, SQL_C_DEFAULT, &categoryId, 1, &ptrSqlVersion);
            SQLGetData(sqlStatementHandle, 2, SQL_C_CHAR, categoryName, sizeof(categoryName), &ptrSqlVersion);
            std::cout << "\nCategory ID: " << categoryId << "\nCategory name: " << categoryName << "\n";
        }
    }
    SQLCancel(sqlStatementHandle);
}

void Database::AddProduct() {
    std::string categoryId, productName, productPrice;

    std::cout << "\nNhap ma loai: "; std::getline(std::cin >> std::ws, categoryId);
    std::cout << "Nhap ten san pham: "; std::getline(std::cin >> std::ws, productName);
    std::cout << "Nhap gia san pham: "; std::getline(std::cin >> std::ws, productPrice);

    std::string query = "INSERT INTO PRODUCT VALUES('"
        + categoryId + "','"
        + productName + "','"
        + productPrice + "',"
        + std::to_string(0) + ")";

    SQLCHAR* queryChar = (SQLCHAR*)query.c_str();

    if (SQL_SUCCESS != SQLExecDirectA(sqlStatementHandle, queryChar, SQL_NTS)) {
        std::cout << "\nAn error has ocurred while adding new product.\n";
    }
    else {
        std::cout << "\nProduct " << productName << " added successfully.\n";
    }
    SQLCancel(sqlStatementHandle);
}

void Database::GetProducts() {
    std::string query = "SELECT ProductID, ProductName, CategoryName, ProductPrice "
        "FROM PRODUCT INNER JOIN CATEGORY ON PRODUCT.CategoryID = CATEGORY.CategoryID "
        "WHERE IsDeleted = 0";

    SQLCHAR* queryChar = (SQLCHAR*)query.c_str();

    if (SQL_SUCCESS != SQLExecDirectA(sqlStatementHandle, queryChar, SQL_NTS)) {
        std::cout << "\nAn error has occured while getting products.\n";
    }
    else {
        SQLINTEGER ptrSqlVersion;
        int productId, productPrice;
        char productName[50], categoryName[50];

        while (SQLFetch(sqlStatementHandle) == SQL_SUCCESS) {
            SQLGetData(sqlStatementHandle, 1, SQL_C_DEFAULT, &productId, 1, &ptrSqlVersion);
            SQLGetData(sqlStatementHandle, 2, SQL_C_CHAR, productName, sizeof(productName), &ptrSqlVersion);
            SQLGetData(sqlStatementHandle, 3, SQL_C_CHAR, categoryName, sizeof(categoryName), &ptrSqlVersion);
            SQLGetData(sqlStatementHandle, 4, SQL_C_DEFAULT, &productPrice, 1, &ptrSqlVersion);
            std::cout << "\nProduct ID: " << productId << "\nProduct name: " << productName << "\nCategory name: " << categoryName << "\nProduct price: " << productPrice << "\n";
        }
    }
    SQLCancel(sqlStatementHandle);
}

void Database::RemoveProduct(int productId) {
    std::string query = "UPDATE PRODUCT SET IsDeleted = 1 WHERE ProductID = " + std::to_string(productId);

    SQLCHAR* queryChar = (SQLCHAR*)query.c_str();

    if (SQL_SUCCESS != SQLExecDirectA(sqlStatementHandle, queryChar, SQL_NTS)) {
        std::cout << "\nAn error has occurred while deleting product.\n";
    }
    else {
        std::cout << "\nProduct deleted successfully.\n";
    }
    SQLCancel(sqlStatementHandle);
}

void Database::AddProductToInvoice(std::string productId, std::string quantity) {
    int productPrice, price, invoiceId;
    std::string getProductQuery = "SELECT ProductPrice FROM PRODUCT WHERE ProductID = " + productId;
    SQLCHAR* queryChar = (SQLCHAR*)getProductQuery.c_str();

    if (SQL_SUCCESS != SQLExecDirectA(sqlStatementHandle, queryChar, SQL_NTS)) {
        std::cout << "\nAn error has occurred while adding product to invoice.\n";
        return;
    }
    else {
        while (SQLFetch(sqlStatementHandle) == SQL_SUCCESS) {
            SQLINTEGER ptrSqlVersion;
            SQLGetData(sqlStatementHandle, 1, SQL_C_DEFAULT, &productPrice, 1, &ptrSqlVersion);
        }
    }
    SQLCancel(sqlStatementHandle);

    std::string getInvoiceIdQuery = "SELECT MAX(InvoiceID) as InvoiceID FROM INVOICE";
    queryChar = (SQLCHAR*)getInvoiceIdQuery.c_str();

    if (SQL_SUCCESS != SQLExecDirectA(sqlStatementHandle, queryChar, SQL_NTS)) {
        std::cout << "\nAn error has occurred while adding product to invoice.\n";
        return;
    }
    else {
        while (SQLFetch(sqlStatementHandle) == SQL_SUCCESS) {
            SQLINTEGER ptrSqlVersion;
            SQLGetData(sqlStatementHandle, 1, SQL_C_DEFAULT, &invoiceId, 1, &ptrSqlVersion);
        }
    }
    SQLCancel(sqlStatementHandle);

    price = productPrice * std::stoi(quantity);    
    std::string addToInvoiceQuery = "INSERT INTO INVOICE_DETAIL (InvoiceID, ProductID, Quantity, Price) VALUES (" + std::to_string(invoiceId) + "," + productId + "," + quantity + "," + std::to_string(price) + ")";
    queryChar = (SQLCHAR*)addToInvoiceQuery.c_str();

    if (SQL_SUCCESS != SQLExecDirectA(sqlStatementHandle, queryChar, SQL_NTS)) {
        std::cout << "\nAn error has occurred while adding product to invoice.\n";
        return;
    }
    else {
        std::cout << "\nProduct added to invoice successfully.\n";
    }
    SQLCancel(sqlStatementHandle);
}

void Database::GetTotalSalesByYear(std::string year) {
    int productCount = 0, totalRevenue = 0, quantity, price;
    std::string query = "SELECT Quantity, Price, CreationDate FROM INVOICE_DETAIL INNER JOIN INVOICE ON INVOICE_DETAIL.InvoiceID = INVOICE.InvoiceID  WHERE YEAR(CreationDate) = " + year;
    SQLCHAR* queryChar = (SQLCHAR*) query.c_str();

    if (SQL_SUCCESS != SQLExecDirectA(sqlStatementHandle, queryChar, SQL_NTS)) {
        std::cout << "\nAn error has occurred while getting total sales in year " << year << ".\n";
        return;
    }
    else {
        while (SQLFetch(sqlStatementHandle) == SQL_SUCCESS)
        {
            SQLINTEGER ptrSqlVersion;
            SQLGetData(sqlStatementHandle, 1, SQL_C_DEFAULT, &quantity, 1, &ptrSqlVersion);
            SQLGetData(sqlStatementHandle, 2, SQL_C_DEFAULT, &price, 1, &ptrSqlVersion);
            productCount += quantity;
            totalRevenue += price;
        }
    }
    std::cout << "So luong san pham da ban: " << productCount << "\n";
    std::cout << "Tong doanh thu: " << totalRevenue << "\n";
}

bool Database::SortDescending(Product& product_a, Product& product_b) {
    return product_a.GetProductPrice() >= product_b.GetProductPrice();
}

bool Database::SortAscending(Product& product_a, Product& product_b) {
    return product_a.GetProductPrice() <= product_b.GetProductPrice();
}

void Database::ProductMergeSortIntervals(std::vector<Product>& productList, int start, int middle, int end, bool (*compareFunction)(Product &, Product &)) {
    std::vector<Product> temp;

    int i, j;
    i = start;
    j = middle + 1;

    while (i <= middle && j <= end) {
        if (compareFunction(productList[i], productList[j])) {
            temp.push_back(productList[i]);
            i++;
        }
        else {
            temp.push_back(productList[j]);
            j++;
        }
    }

    while (i <= middle) {
        temp.push_back(productList[i]);
        i++;
    }

    while (j <= end) {
        temp.push_back(productList[j]);
        j++;
    }

    for (int i = start; i <= end; ++i) {
        productList[i] = temp[i - start];
    }
}

void Database::ProductMergeSort(std::vector<Product>& productList, int start, int end, bool (*compareFunction)(Product &, Product &)) {
    if (start < end) {
        int middle = (start + end) / 2;
        ProductMergeSort(productList, start, middle, compareFunction);
        ProductMergeSort(productList, middle + 1, end, compareFunction);
        ProductMergeSortIntervals(productList, start, middle, end, compareFunction);
    }
}

void Database::SortProductByPrice() {
    std::string query = "SELECT ProductID, ProductName, CategoryName, ProductPrice "
        "FROM PRODUCT INNER JOIN CATEGORY ON PRODUCT.CategoryID = CATEGORY.CategoryID "
        "WHERE IsDeleted = 0";
    SQLCHAR* queryChar = (SQLCHAR*)query.c_str();
    
    std::vector<Product> productArray;

    if (SQL_SUCCESS != SQLExecDirectA(sqlStatementHandle, queryChar, SQL_NTS)) {
        std::cout << "\nAn error has occurred while sorting products.\n";
        return;
    }
    else {
        while (SQLFetch(sqlStatementHandle) == SQL_SUCCESS) {
            SQLINTEGER ptrSqlVersion;
            int productId, productPrice;
            char productName[50], categoryName[50];
            
            SQLGetData(sqlStatementHandle, 1, SQL_C_DEFAULT, &productId, 1, &ptrSqlVersion);
            SQLGetData(sqlStatementHandle, 2, SQL_C_CHAR, productName, sizeof(productName), &ptrSqlVersion);
            SQLGetData(sqlStatementHandle, 3, SQL_C_CHAR, categoryName, sizeof(categoryName), &ptrSqlVersion);
            SQLGetData(sqlStatementHandle, 4, SQL_C_DEFAULT, &productPrice, 1, &ptrSqlVersion);

            productArray.push_back(Product(
                productId,
                productPrice,
                productName,
                categoryName
            ));
        }
    }
    std::string userChoice;
    std::cout << "\nThu tu sap xep (1: tang dan, 2: giam dan): ";
    std::getline(std::cin >> std::ws, userChoice);

    switch (std::stoi(userChoice)) {
    case 1:
        ProductMergeSort(productArray, 0, productArray.size() - 1, SortAscending);
        break;
    case 2:
        ProductMergeSort(productArray, 0, productArray.size() - 1, SortDescending);
        break;
    }
    
    for (int i = 0; i < productArray.size(); ++i) {
        std::cout << "\nProduct ID: " << productArray[i].GetProductId() << "\nProduct name: " << productArray[i].GetProductName() << "\nCategory name: " << productArray[i].GetCategoryName() << "\nProduct price: " << productArray[i].GetProductPrice() << "\n";
    }
    SQLCancel(sqlStatementHandle);
}