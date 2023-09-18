/* Chat.h Chat */

#pragma once
#include <string>
#include <memory>
#include <process.h>
#include <string.h>
#include <iostream>
#include <mysql.h>
#include <time.h>

#define MESSAGE_LENGTH 1024 // Максимальный размер буфера для данных

class Chat
{
private:
	std::string _name;
	std::string _login;
	std::string _password;
	std::string _from;
	std::string _to;
	std::string _text;
	std::string _date_time;

public:
	Chat() = default;
	~Chat() = default;
	void chatting(const std::string&);
	void logIn();
	void checkIn();
	char* socketRead();
	void socketWrite(const char*);
	char message[MESSAGE_LENGTH];
	int connection;
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;
	void mysqlopen();
	void mysqlclose();
};

std::string currDateTime();
