#include <string>
#include <iostream>
#include <windows.h>
#pragma once

class KontoBankowe {
public:
	KontoBankowe(std::string login, std::string haslo,double suma);

	void doladuj(double suma);
	void wyplac(double suma, HANDLE &hConsole);

	std::string get_login();
	std::string get_haslo();
	double get_suma();

	void set_login(std::string login);
	void set_haslo(std::string haslo);


private:
	double suma;
	std::string login;
	std::string haslo;
};

