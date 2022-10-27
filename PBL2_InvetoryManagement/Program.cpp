#include "Program.h"
#include <limits>

void Program::MainProgram() {
	Database databaseInstance;
	databaseInstance.InitializeDatabaseInstance();

	int userChoice;
	do {
		userChoice = Menu();

		switch (userChoice) {
		case CREATE_INVOICE:
			CreateInvoice(databaseInstance);
			break;

		case GET_PRODUCT_LIST:
			GetProducts(databaseInstance);
			break;

		case GET_INVOICE_LIST:
			GetInvoices(databaseInstance);
			break;

		case ADD_PRODUCT:
			AddProduct(databaseInstance);
			break;

		case REMOVE_PRODUCT:
			RemoveProduct(databaseInstance);
			break;

		case TOTAL_SALES:
			break;

		case SORT_PRODUCT:

			break;

		case EXIT_PROGRAM:
			databaseInstance.DestroyDatabaseInstance();
			exit(0);
		default:
			break;
		}
	} while (userChoice != 8);
}

int Program::Menu() {
	int intUserChoice;
	std::string userChoice;

	std::cout << "\nQUAN LY KHO HANG\n\n";
	std::cout << "1. Lap hoa don.\n";
	std::cout << "2. Xem tat ca san pham.\n";
	std::cout << "3. Xem tat ca hoa don.\n";
	std::cout << "4. Them san pham.\n";
	std::cout << "5. Xoa san pham.\n";
	std::cout << "6. Tong doanh thu va so luong hang ban duoc.\n";
	std::cout << "7. Sap xep san pham theo gia.\n";
	std::cout << "8. Thoat.\n";
	std::cout << "\nLua chon: ";
	std::getline(std::cin >> std::ws, userChoice);
	intUserChoice = std::stoi(userChoice);
	return intUserChoice;
}

void Program::CreateInvoice(Database &databaseInstance) {
	std::string continueCreate, productId, quantity;
	
	if (databaseInstance.CreateInvoice() != SQL_SUCCESS) {
		std::cout << "\nAn error has occurred while creating invoice.\n";
		return;
	}

	do {
		std::cout << "\nNhap ID san pham can tao hoa don: ";
		std::getline(std::cin >> std::ws, productId);
		std::cout << "Nhap so luong: ";
		std::getline(std::cin >> std::ws, quantity);
		databaseInstance.AddProductToInvoice(productId, quantity);
		std::cout << "Tiep tuc them? [y/n]: ";
		std::getline(std::cin >> std::ws, continueCreate);
	} while (continueCreate.compare("y") == 0 ||
		continueCreate.compare("Y") == 0);
}

void Program::GetInvoices(Database &databaseInstance) {
	databaseInstance.GetInvoices();
}

void Program::AddProduct(Database &databaseInstance) {
	databaseInstance.GetCategories();
	databaseInstance.AddProduct();
}

void Program::GetProducts(Database &databaseInstance) {
	databaseInstance.GetProducts();
}

void Program::RemoveProduct(Database &databaseInstance) {
	int productId;
	std::string userChoice;

	std::cout << "\nNhap ID san pham can xoa: ";
	std::getline(std::cin >> std::ws, userChoice);
	productId = std::stoi(userChoice);
	databaseInstance.RemoveProduct(productId);
}

void Program::SortProduct(Database &databaseInstance) {

}