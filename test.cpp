/*int inputHandler(const char* msg, const int& low, const int& high)
{
	int user_input = 0;
	try {
		printf("%s ? (%d - %d): ", msg, low, high);
		std::cin >> user_input;
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(1000, '\n');
			throw std::invalid_argument("Incorrect value");
		}
		if (user_input < low || user_input > high)
			throw std::invalid_argument("Incorrect value");

	}
	catch (const std::invalid_argument& e) {
		std::cout << e.what() << std::endl;
		inputHandler(msg, low, high);
	}
	return user_input;
}*/