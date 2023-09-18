/* main.cpp Chat */

#define WIN32_LEAN_AND_MEAN
#include "Chat.h"
#include "Logger.h"
#include "SConnect.h"

namespace NS
{
    Chat chat;
    Logger log;
    SConnect sconn;
}

void threadChat()
{
    std::string str;
    try
    {
        NS::chat.connection = NS::sconn.sconnection();
        NS::chat.mysqlopen();
    }
    catch (const char* exception)
    {
        str = "Error: ";
        str += exception;
        NS::log.writelog(str);
        str += '\n';
        //std::cerr << str;
        return;
    }
    str.clear();
    while (true)
    {
        str += "\nEnter 1 to enter the chat \nEnter 2 to register in the chat \nEnter 3 to exit the program \n";
        NS::chat.socketWrite(str.c_str());
        str.clear();
        char mode = NS::chat.socketRead()[0];
        try
        {
            switch (mode)
            {
            case '1':   // Entering the chat
                NS::chat.logIn();
                break;
            case '2':   // New User Registration
                NS::chat.checkIn();
                break;
            case '3':   // Exiting the program
                // Закрываем соединение с базой данных
                NS::chat.mysqlclose();
                NS::chat.socketWrite("Goodbye!");
                NS::log.writelog("Exit");
                // Закрываем сокет, завершаем соединение
                closesocket(NS::sconn.ListenSocket);
                return;
            default:
                throw "Wrong enter, please re-enter!";
            }
        }
        catch (const char* exception)
        {
            str = "Error: ";
            str += exception;
            NS::log.writelog(str);
            str += '\n';
            //std::cerr << str;
        }
    }
}

void threadServ()
{
    while (true)
    {
        std::cout << '\n' << "Enter 1 to show the logs \n";
        char mode;
        std::cin >> mode;
        switch (mode)
        {
        case '1':   // Show logs
            NS::log.readlog();
            break;
        default:
            break;
        }
    }
}

int main()
{
    std::thread thrCht(threadChat);
    std::thread thrSrv(threadServ);
    thrSrv.detach();
    if (thrCht.joinable()) {
        thrCht.join();
    }
    NS::log.user_file.close();  // Закрываем файл

    return 0;
}