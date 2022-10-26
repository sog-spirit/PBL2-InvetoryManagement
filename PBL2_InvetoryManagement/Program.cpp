#include "Program.h"
#include <limits>

void Program::MainProgram() {
	Database databaseInstance;
	databaseInstance.InitializeDatabaseInstance();

	int userChoice;
	do {
		userChoice = Menu();

		switch (userChoice) {
		case ADD_PRODUCT:
			AddProduct(databaseInstance);
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
	int userChoice;
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
	std::cin >> userChoice;
	return userChoice;
}

void Program::AddProduct(Database &databaseInstance) {
	databaseInstance.GetCategories();
}