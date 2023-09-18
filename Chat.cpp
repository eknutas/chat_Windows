/* Chat.cpp Chat */

#include "Chat.h"

void Chat::chatting(const std::string& _from)
{
	std::string str = "\033[2J\033[1;1H";	// Clearing the console
	str += "Hello, " + _from + '\n';
	mysql_query(&mysql, "SELECT * FROM messages"); // Делаем запрос к таблице
	_to = "all";
	if (res = mysql_store_result(&mysql)) {
		while (row = mysql_fetch_row(res)) {
		if ((row[1] == _from) || (row[2] == _from) || (row[2] == _to)) {
			str += "\nFrom: ";
			str += row[1];
			str += "     To: ";
			str += row[2];
			str += "     When: ";
			str += row[4];
			str += "\nMessage: ";
			str += row[3];
			str += '\n';}
		}
	}
	str += "\nEnter the recipient (all for everyone): ";
	socketWrite(str.c_str());
	_to = socketRead();
	socketWrite("Enter your message: ");
	_text = socketRead();
	_date_time = currDateTime();
	str = "INSERT INTO messages(id, from_, to_, text_, date_time_) values(default, '" + _from + "', '" + _to + "', '" + _text + "', '" + _date_time + "')";
	mysql_query(&mysql, str.c_str());
}

void Chat::logIn()
{
	mysql_query(&mysql, "SELECT * FROM users"); // Делаем запрос к таблице
	if (mysql_fetch_row(mysql_store_result(&mysql)) == 0) {
		throw "No user, registration required!";
	}
	socketWrite("Enter login: ");
	_login = socketRead();
	socketWrite("Enter password: ");
	_password = socketRead();
	mysql_query(&mysql, "SELECT * FROM users"); // Делаем запрос к таблице
	if (res = mysql_store_result(&mysql)) {
		while (row = mysql_fetch_row(res)) {
			if (_login == row[2] && _password == row[3]) {	// Login and password verification
				chatting(row[1]);	// Enter the chat
				return;
			}
		}
	}
	throw "Invalid login or password!";
}

void Chat::checkIn()
{
	socketWrite("Enter a new username: ");
	_name = socketRead();
	if (_name == "all") {
		throw "This name is busy! Please enter another name.";
	}
	mysql_query(&mysql, "SELECT * FROM users"); // Делаем запрос к таблице
	if (res = mysql_store_result(&mysql)) {
		while (row = mysql_fetch_row(res)) {
			if (_name == row[1]) {
				throw "This name is busy! Please enter another name.";}
			}
	}
	socketWrite("Enter new user login: ");
	_login = socketRead();
	mysql_query(&mysql, "SELECT * FROM users"); // Делаем запрос к таблице
	if (res = mysql_store_result(&mysql)) {
		while (row = mysql_fetch_row(res)) {
			if (_login == row[2]) {
				throw "This login is busy! Please enter another login.";}
			}
	}
	socketWrite("Enter the new user's password: ");
	_password = socketRead();
	std::string str = "INSERT INTO users(id, name_, login_, password_) values(default, '" + _name + "', '" + _login + "', '" + _password + "')";
	mysql_query(&mysql, str.c_str());
}

char* Chat::socketRead()
{
	memset(message, 0, MESSAGE_LENGTH);
	recv(connection, message, sizeof(message), 0);
	return message;
}

void Chat::socketWrite(const char* msg)
{
	memset(message, 0, MESSAGE_LENGTH);
	strcpy_s(message, msg);
	send(connection, message, sizeof(message), 0);
}

void Chat::mysqlopen()
{
	mysql_init(&mysql);
	if (&mysql == nullptr) {
		// Если дескриптор не получен — выводим сообщение об ошибке
		throw "Error: can't create MySQL-descriptor";
	}
 	// Подключаемся к серверу
	if (!mysql_real_connect(&mysql, "localhost", "root", "aL355caN", "chatdb", 0, NULL, 0)) {
		// Если нет возможности установить соединение с БД выводим сообщение об ошибке
		throw "Error: can't connect to database ";
	}
	mysql_query(&mysql, "CREATE TABLE IF NOT EXISTS users(id INT AUTO_INCREMENT PRIMARY KEY, name_ VARCHAR(255), login_ VARCHAR(255), password_ VARCHAR(255))");
	mysql_query(&mysql, "CREATE TABLE IF NOT EXISTS messages(id INT AUTO_INCREMENT PRIMARY KEY, from_ VARCHAR(255), to_ VARCHAR(255), text_ VARCHAR(1000), date_time_ VARCHAR(25))");
}

void Chat::mysqlclose()
{
	mysql_close(&mysql);
}

std::string currDateTime()
{
	char buffer[25];
	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);
	strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", &newtime);
	return buffer;
}
