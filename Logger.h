/* Logger.h Chat */

#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <shared_mutex>

class Logger
{
private:
	std::string _text;
	std::string _date_time;
	std::shared_mutex mutex;

public:
	Logger(const std::string& date_time, const std::string& text);
	Logger();
	~Logger();
    std::fstream user_file = std::fstream("log.txt", std::ios::in | std::ios::out);
    friend std::fstream& operator >> (std::fstream& is, Logger& log);
	friend std::ostream& operator << (std::ostream& os, const Logger& log);
	void writelog(const std::string&);
    void readlog();
};

std::string currDate_Time();