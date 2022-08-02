#include "KontoBankowe.h"
#include <windows.h>

KontoBankowe::KontoBankowe(std::string login, std::string haslo, double suma) {
	this->suma = suma;
	this->login = login;
	this->haslo = haslo;
}

void KontoBankowe::doladuj(double suma) {
	if (suma >= 0) {
		this->suma += suma;	
	}
}

void KontoBankowe::wyplac(double suma, HANDLE &hConsole) {
	if (suma <= this->suma && suma >= 0) {
		this->suma -= suma;
		SetConsoleTextAttribute(hConsole, 10);
		std::cout << "<" << "Pomyslnie wyplacono :" << suma << ">" << std::endl;
	}
	else {
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "<" << "Nie pomylnie wyplacono poniewaz na twoim koncie jest:" << this->suma << ">" << std::endl;
	}
		
}


std::string KontoBankowe::get_login() {
	return login;
}
std::string KontoBankowe::get_haslo() {
	return haslo;
}
double KontoBankowe::get_suma() {
	return suma;
}

void KontoBankowe::set_login(std::string login) {
	this->login = login;
}

void KontoBankowe::set_haslo(std::string haslo) {
	this->haslo = haslo;
}

