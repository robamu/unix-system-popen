#include <iostream>

int main() {
	std::cout << "Hello world!" << std::endl;
	int result = std::system("./rust-app");
	std::cout << "System call result: " << result << std::endl;
	return 0;
}
