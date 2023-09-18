/* Logger.cpp Chat */

#include "Logger.h"

Logger::Logger(const std::string& date_time, const std::string& text) : _date_time(date_time), _text(text)
{

}

Logger::Logger()
{
	if (!user_file) {
		// Для создания файла используем параметр ios::trunc
		user_file = std::fstream("log.txt", std::ios::in | std::ios::out | std::ios::trunc);
	}
	if (!user_file) {
		throw  "Could not open file log.txt !";
	}
}

Logger::~Logger()
{
	user_file.close();
}

std::fstream& operator >> (std::fstream& is, Logger& log)
{
	is >> log._date_time;
	is >> log._text;
	return is;
}

std::ostream& operator << (std::ostream& os, const Logger& log)
{
	os << log._date_time;
	os << ' ';
	os << log._text;
	return os;
}

void Logger::writelog(const std::string& text)
{
	user_file = std::fstream("log.txt", std::ios::app);
	user_file.seekp(0, std::ios_base::end);
	mutex.lock();
	user_file << Logger(currDate_Time(), text) << '\n';
	mutex.unlock();
}

void Logger::readlog()
{
    // Считываем данные из файла
	user_file = std::fstream("log.txt", std::ios::in);
	user_file.seekg(0, std::ios_base::beg);
	std::string str;
	while (getline(user_file, str))
	{
		mutex.lock_shared();
		std::cout << str << '\n';
		mutex.unlock_shared();
	}
}

std::string currDate_Time()
{
	char buffer[25];
	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);
	strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", &newtime);
	return buffer;
}
