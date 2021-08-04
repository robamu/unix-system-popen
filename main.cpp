#include "spdlog/spdlog.h"

#include <unistd.h>
#include <fcntl.h>

#include <cstdio>
#include <iostream>
#include <chrono>
#include <thread>
#include <cerrno>

int systemCall();
int popenCall();

int main() {
	spdlog::info("Starting popen/system test application..");
	systemCall();
	popenCall();
	spdlog::info("popen/system test done");
	return 0;
}

int systemCall() {
	// This will block
	spdlog::info("Blocking call to rust application..");
	int result = std::system("./rust-app");
	spdlog::info("std::system call result: {}", result);
	return 0;
}

int popenCall() {
	using namespace std::chrono_literals;
	spdlog::info("Non-blocking call to rust application..");
	FILE* file = popen("./rust-app", "r");
	spdlog::info("Still kicking!");
	std::array<char, 128> readBuf {};
	int filenum = fileno(file);
	// Use non-blocking read here
	int flags = fcntl(filenum, F_GETFL, 0);
	fcntl(filenum, F_SETFL, flags | O_NONBLOCK);
	while(true) {
		ssize_t readBytes = read(filenum, readBuf.data(), readBuf.size());
		if(readBytes < 0) {
			if(errno == EAGAIN) {
				spdlog::info("No data available. Sleeping a second..");
				std::this_thread::sleep_for(1000ms);
				continue;
			}
			else {
				spdlog::warn("Error {}: {}", errno, strerror(errno));
			}
		}
		else if(readBytes == 0) {
			spdlog::info("Program finished");
			break;
		}
		else {
			spdlog::info("Read line: {}", readBuf.data());
			std::memset(readBuf.data(), 0, readBuf.size());
		}
	}
	int result = pclose(file);
	spdlog::info("pclose call result: {}", result);
	return 0;
}
