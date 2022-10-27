#pragma once
#include <string>

class Product {
private:
	int ProductId, ProductPrice;
	char ProductName[50], CategoryName[50];

public:
	Product(int productId, int productPrice, char productName[], char categoryName[]);
	int GetProductId();
	int GetProductPrice();
	char* GetProductName();
	char* GetCategoryName();
	Product& operator= (const Product& product);
};