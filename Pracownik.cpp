#include "Pracownik.h"

Pracownik::Pracownik(std::string _imie, int _wiek, Date _dataurodzenia, Date _datazatrudnienia) {
	imie = _imie;
	wiek = _wiek;
	data_urodzenia = _dataurodzenia;
	data_zatrudnienia = _datazatrudnienia;
}