#include "Product.h"

Product::Product(int productId, int productPrice, char productName[], char categoryName[]) {
	ProductId = productId;
	ProductPrice = productPrice;
	strncpy_s(ProductName, productName, sizeof(ProductName));
	strncpy_s(CategoryName, categoryName, sizeof(CategoryName));
}

int Product::GetProductId() {
	return ProductId;
}

int Product::GetProductPrice() {
	return ProductPrice;
}

char* Product::GetProductName() {
	return ProductName;
}
char* Product::GetCategoryName() {
	return CategoryName;
}

Product& Product::operator= (const Product& product) {
	ProductId = product.ProductId;
	ProductPrice = product.ProductPrice;
	strncpy_s(ProductName, product.ProductName, sizeof(ProductName));
	strncpy_s(CategoryName, product.CategoryName, sizeof(CategoryName));

	return *this;
}