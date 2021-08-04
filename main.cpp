#include "spdlog/spdlog.h"

#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include <cstdio>
#include <iostream>
#include <chrono>
#include <thread>
#include <cerrno>

int systemCall();
int popenCallUsingFnctl();
int popenCallUsingPoll();
int popenCall(bool fnctl);

int main() {
	spdlog::info("Starting popen/system test application..");
	spdlog::info("================================");
	spdlog::info("Using regular system function..");
	spdlog::info("================================");
	systemCall();
	spdlog::info("================================");
	spdlog::info("Using popen with fnctl for non-blocking read..");
	spdlog::info("================================");
	popenCallUsingFnctl();
	spdlog::info("================================");
	spdlog::info("Using popen with poll for non-blocking read..");
	spdlog::info("================================");
	popenCallUsingPoll();
	spdlog::info("================================");
	spdlog::info("popen/system test done");
	spdlog::info("================================");
	return 0;
}

int systemCall() {
	// This will block
	spdlog::info("Blocking call to rust application..");
	int result = std::system("./rust-app");
	spdlog::info("std::system call result: {}", result);
	return 0;
}

int popenCallUsingFnctl() {
	return popenCall(true);
}

int popenCallUsingPoll() {
	return popenCall(false);
}

int handleReadWithFnctl(int filenum, std::array<char, 128>& readBuf);
int handleReadWithPoll(int filenum, std::array<char, 128>& readBuf);

int popenCall(bool fnctl) {
	using namespace std::chrono_literals;
	spdlog::info("Non-blocking call to rust application..");
	FILE* file = popen("./rust-app", "r");
	std::array<char, 128> readBuf {};
	int filenum = fileno(file);
	if(fnctl) {
		handleReadWithFnctl(filenum, readBuf);
	}
	else {
		handleReadWithPoll(filenum, readBuf);
	}
	int result = pclose(file);
	spdlog::info("pclose call result: {}", result);
	return 0;
}

int handleReadWithFnctl(int filenum, std::array<char, 128>& readBuf) {
	using namespace std::chrono_literals;
	// Configure file descriptor for non-blocking operation
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
	return 0;
}

int handleReadWithPoll(int filenum, std::array<char, 128>& readBuf) {
	using namespace std::chrono_literals;
	// Use poll instead to achieve the same result
	struct pollfd waiter {};
	waiter.fd = filenum;
	waiter.events = POLLIN;
	while(true) {
		bool done = false;
		int result = poll(&waiter, 1, 0);
		switch(result) {
		case(0): {
			spdlog::info("No data available. Sleeping a second..");
			std::this_thread::sleep_for(1000ms);
			continue;
		}
		case(1): {
			if (waiter.revents & POLLIN) {
				ssize_t readBytes = read(filenum, readBuf.data(),
						readBuf.size());
				if(readBytes == 0) {
					spdlog::warn("No bytes read after poll event..");
					break;
				}
				else if(readBytes > 0) {
					spdlog::info("Read line: {}", readBuf.data());
					std::memset(readBuf.data(), 0, readBuf.size());
				}
				else {
					spdlog::warn("Error {}: {}", errno, strerror(errno));
				}
			}
			else if(waiter.revents & POLLERR) {
				spdlog::warn("Poll error error");
			}
			else if(waiter.revents & POLLHUP) {
				spdlog::info("Writer closed its end");
				done = true;
			}
			break;
		}
		default: {
			spdlog::warn("Unknwon poll returnvalue {}", result);
		}
		}
		if(done) {
			break;
		}
	}
	return 0;
}
