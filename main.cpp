#include <cstdio>
#include <iostream>

#include "spdlog/spdlog.h"

int systemCall();
int popenCall();

int main() {
	spdlog::info("Starting popen/system test application..");
	systemCall();
	popenCall();
	return 0;
}

int systemCall() {
	// This will block
	spdlog::info("Blocking call to rust application..");
	int result = std::system("./rust-app");
	spdlog::info("System call result: {}", result);
	return 0;
}

int popenCall() {
	spdlog::info("Non-blocking call to rust application..");
	FILE* file = popen("./rust-app", "r");
	spdlog::info("Still kicking!");
	pclose(file);
	return 0;
}
