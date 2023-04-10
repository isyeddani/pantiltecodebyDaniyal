#include <iostream>
#include <fstream>
#include <string>
using namespace std;


void CreateFile()
{
    fstream Myfile;
    Myfile.open("Newfile.txt", ios::out);
    Myfile.close();
}

void FileTest(int A)
{
    
    std::cout << A << std::endl;
    fstream Myfile;
    Myfile.open("Newfile.txt", ios::in);
    if (Myfile.is_open())
    {
        int azzdeg;
        int elvdeg;
        int dustbin;
        string Aux;
        Myfile >> Aux;
        // std::cout << Aux << std::endl;
        if (Aux == "Azimuth:")
        {
            Myfile >> azzdeg;
            azzdeg *= 10;
            std::cout << azzdeg << std::endl;
        }
        else
        {
            std::cout << "Invalid" << std::endl;
            Myfile >> dustbin;
        }
        Myfile >> Aux;
        if (Aux == "Elevation:")
        {
            Myfile >> elvdeg;
            elvdeg *= 10;
            std::cout << elvdeg << std::endl;
        }
        else
        {
            std::cout << "Invalid" << std::endl;
            Myfile >> dustbin;
        }
    }
    else
    {
        std::cout << "FileNotFound" << std::endl;
        int a;
        std::cin >> a;
        CreateFile();
    }
}



int main()
{
    FileTest(1);
    int a;
    std::cin >> a;
    fstream Myfile;
    // int a;
    std::cin >> a;
    Myfile.open("Newfile.txt", ios::out); // Write
    // int a;
    std::cin >> a;
    if (Myfile.is_open())
    {
        Myfile << "Azimuth:\n";
        Myfile << -40;
        Myfile << "\n";
        Myfile << "Elevation:\n";
        Myfile << 60;
    }

    Myfile.close();
    FileTest(2);
    FileTest(3);
    FileTest(4);
    return 0;
}