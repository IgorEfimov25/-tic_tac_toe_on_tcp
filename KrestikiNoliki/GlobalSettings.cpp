#include "GlobalSettings.h";
#include <fstream>
#include <string>
#include <iostream>

bool LoadSettings(int type, std::string& ip, int& port)
{

    std::string filename;
    if (type == 1)
        filename = "ServerSettings.txt";
    else if (type == 2)
        filename = "ClientSettings.txt";
    else
        return false;

    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Не удалось открыть файл: " << filename << std::endl;
        return false;
    }


    if (!std::getline(file, ip))
    {
        std::cerr << "Ошибка чтения IP из файла " << filename << std::endl;
        return false;
    }


    std::string portStr;
    if (!std::getline(file, portStr))
    {
        std::cerr << "Ошибка чтения порта из файла " << filename << std::endl;
        return false;
    }


    try
    {
        port = std::stoi(portStr);
    }
    catch (...)
    {
        std::cerr << "Порт должен быть целым числом в файле " << filename << std::endl;
        return false;
    }

    return true;
}