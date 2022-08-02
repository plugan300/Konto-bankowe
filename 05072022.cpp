#include <iostream>
#include "KontoBankowe.h"
#include <windows.h>
#include <vector>
#include "W.h"
#include <algorithm>

#define blue 9
#define green 10
#define cyan 11
#define red 12
#define magneta 13
#define yellow 14
#define white 15

using namespace std;

//Metody glownego programu
void wybor_menu_poczatkowe(int &operacja,HANDLE &hConsole, W* bledne, vector<KontoBankowe>& konta);
void logowanie(HANDLE &hConsole, W* bledne, vector<KontoBankowe>& konta);
void nowe_konto(HANDLE &hConsole, vector<KontoBankowe>& konta);
void jestes_zalogowany(HANDLE& hConsole, vector<KontoBankowe>& konta, vector<string>& words);

//Metody pomocnicze
string to_lower_case(string& text);
void display_array_words(W tab[], int size, HANDLE& hConsole);
vector<string> line_to_array(string& line);
void usun_konto(vector<KontoBankowe>& konta, KontoBankowe* konto_do_usuniecia);


int main() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    vector<KontoBankowe> konta;

 
    string line;

    //Menu głowne

    
    //Bledne dane
    W line_end[] = { W(red,"Podales bledne dane!") };

    for (;;) {
        W line_header[] = {
            W(green,"W systemie jest : "), W(red, to_string(konta.size())), W(green," zarejstrowanych kont\n"),
            W(yellow,"Wybierz jedna liczbe odpowiadajaca operacji :\n"),
            W(yellow,"["),W(green,"0"), W(yellow,"] "), W(magneta,"Zaloguj sie\n"),
            W(yellow,"["),W(green,"1"), W(yellow,"] "), W(magneta,"Stworz nowe konto")
        };
        display_array_words(line_header, sizeof(line_header) / sizeof(W), hConsole);

        getline(cin, line);
        system("cls");
        vector<string> vec;
        if (line.size() > 0) {
            vec = line_to_array(line);
        }
        int operacja;

        try {
            if (line.size() > 0) {
                operacja = stoi(vec[0]);
                if (operacja > 1) {                   
                    display_array_words(line_end, sizeof(line_end) / sizeof(W), hConsole);
                    continue;
                }
                wybor_menu_poczatkowe(operacja, hConsole, line_end, konta);
            }
            else {
                display_array_words(line_end, sizeof(line_end) / sizeof(W), hConsole);
                continue;
            }
           
        }
        catch (...) {
            display_array_words(line_end, sizeof(line_end) / sizeof(W), hConsole);         
        }
    }

    return 0;
}

vector<string> line_to_array(string &line) {
    vector<string> tab;
    string word;

    line.push_back(' ');
    bool clear = false;

    for (int i = 0; i < line.size(); i++) {
        if (line.at(i) != ' ') {
            word.push_back(line.at(i));
            clear = true;
        }
        else {
            if (clear) {
                tab.push_back(word);
                word.clear();
                clear = false;
            }
          
        }
    }
    return tab;
}

void display_array_words(W tab[], int size, HANDLE &hConsole) {
    for (int i = 0; i < size; i++) {
        SetConsoleTextAttribute(hConsole, *tab[i].get_id_color());    
        cout << *tab[i].get_text();
    }
    cout << endl;
}

void wybor_menu_poczatkowe(int& operacja, HANDLE& hConsole, W* bledne, vector<KontoBankowe>& konta) {
    if (operacja == 0) {
        logowanie(hConsole, bledne, konta);
    
    }
    else if (operacja == 1) {
        nowe_konto(hConsole, konta);
    }
}
void logowanie(HANDLE& hConsole, W* bledne, vector<KontoBankowe>& konta) {
    string line;
    vector<string> words;

    while (true) {
        W w[] = {
            W(green, "Podaj : "), W(red, "login"), W(green, " i "), W(red, "haslo"), W(green, ", oddzielone spacja lub "), W(red, "undo"),
            W(green, " aby wyjsc do menu glownego\n")};
        display_array_words(w, sizeof(w)/ sizeof(W), hConsole);

        getline(cin, line);
        words = line_to_array(line);
        
        if (words.size() == 1) {
            if (to_lower_case(words[0]) == "undo") {
                system("cls");
                break;
            }
            else {
                system("cls");
                display_array_words(bledne, sizeof(*bledne) / sizeof(W), hConsole);
            }
        }
        else if (words.size() > 2) {
            system("cls");
            display_array_words(bledne, sizeof(*bledne) / sizeof(W), hConsole);
        }
        else if (words.size() == 2) {
            //kod wyszukiwania konta;
            jestes_zalogowany(hConsole, konta, words);
        
        }
        else {
            system("cls");
        }
    }
}
void jestes_zalogowany(HANDLE& hConsole, vector<KontoBankowe>& konta, vector<string>& words) {
    bool find = false;
    W konto_nieistnieje[] = {W(red, "Konto o loginie : "), W(green, to_lower_case(words[0])), W(red, " nie istnieje.")};
    W niepoprawne_haslo[] = {W(red, "Podales niepoprawne haslo.")};
    W menu_zalogowanie[] = {
        W(blue, "["),W(green, "0"),W(blue,"] "),W(white, "Doladuj konto.\n"),
        W(blue, "["),W(green, "1"),W(blue,"] "),W(white, "Wyplac z konta.\n"),
        W(blue, "["),W(green, "2"),W(blue,"] "),W(white, "Przelej na inne konto.\n"),
        W(blue, "["),W(green, "3"),W(blue,"] "),W(white, "Usun konto.\n"),
        W(blue, "["),W(green, "4"),W(blue,"] "),W(white, "Wyloguj.\n"),

    };

    KontoBankowe* konto = nullptr;
    double suma;

    for (KontoBankowe& b : konta) {
        string login = b.get_login();
        if (to_lower_case(words[0]) == to_lower_case(login)) {
            find = true;
            konto = &b;
            break;
        }
    }


    if (find) {
        string haslo = konto->get_haslo();
        if (to_lower_case(haslo) == words[1]) {
            system("cls");
            while (true) {                
                W naglowek_login[] = { W(blue, "=====>> "),W(white, "Konto : ") ,W(yellow, konto->get_login()) ,W(blue, " <<=====") };
                W naglowek_suma[] = { W(yellow, "=====>> "),W(white, "Suma : ") ,W(yellow, to_string(konto->get_suma())) ,W(yellow, "zl <<=====") };
                W blad[] = { W(red,  "Niepoprawne dane!")};
                W operacja_0[] = { W(green,"Wpisz ilosc pieniedzy ktore chcesz wplacic.")};

                W operacja_0_suma[] = { W(red,"Mozesz doladowac konto suma od : "),W(green, "0zl"),W(red, " do : "),W(green, "500zl")};             
                W operacja_1[] = { W(green,"Mozesz wyplacic do : "),W(red, to_string(konto->get_suma()))};
                W operacja_1_malo_pieniedzy[] = { W(red, "Masz za malo pieniedzy.")};

                W operacja_2[] = { W(green, "Wpisz login innego "),W(red, "konta "),W(green,"oraz obok "),W(red, "ilosc pieniedzy "),W(green, "ktore chcesz przelac.")};
                W operacja_2_niemakonta[] = { W(red, "Konto nie istnieje.")};
                W operacja_2_przelano[] = { W(green, "Przekazano pieniadze")};
                W operacja_2_niemaszpieniedzy[] = { W(red, "Nie masz wystarczajacej ilosc pieniedzy")};
                
                W usunieto_konto[] = { W(green, "Usunieto konto") };
                
                display_array_words(naglowek_login, sizeof(naglowek_login) / sizeof(W), hConsole);
                display_array_words(naglowek_suma, sizeof(naglowek_suma) / sizeof(W), hConsole);
                display_array_words(menu_zalogowanie, sizeof(menu_zalogowanie) / sizeof(W), hConsole);

                double suma_do_wyplaty = 0;
                double suma_do_przekazania = 0;
                string line;
                getline(cin, line);
                vector<string> words_login = line_to_array(line);
                KontoBankowe* ptr_do_przelewu = nullptr;

                if (words_login.size() > 1 || words_login.size() == 0) {
                    system("cls");
                    display_array_words(blad, sizeof(blad) / sizeof(W), hConsole);
                }
                try {
                    int liczba_operacji = stoi(words_login[0]);
                    if (liczba_operacji >= 0 && liczba_operacji <= 4) {
                        //Kod
                        switch (liczba_operacji) {
                            case 0:
                                system("cls");
                                display_array_words(operacja_0, sizeof(operacja_0) / sizeof(W), hConsole);
                                getline(cin, line);
                                words_login = line_to_array(line);
                                suma = stod(words_login[0]);
                                system("cls");
                                if (suma >= 0 && suma <= 500) {
                                    konto->doladuj(suma);
                                }
                                else {
                                    display_array_words(operacja_0_suma, sizeof(operacja_0_suma) / sizeof(W), hConsole);
                                }                                                                  
                                break;
                            case 1:
                                system("cls");
                                display_array_words(operacja_1, sizeof(operacja_1) / sizeof(W), hConsole);
                                getline(cin, line);
                                words_login = line_to_array(line);
                                suma_do_wyplaty = stod(words_login[0]);
                                system("cls");
                                if (suma_do_wyplaty <= konto->get_suma()) {
                                    konto->wyplac(suma_do_wyplaty, hConsole);
                                }
                                else {                                   
                                    display_array_words(operacja_1_malo_pieniedzy, sizeof(operacja_1_malo_pieniedzy) / sizeof(W), hConsole);
                                }
                                
                                break;
                            case 2:
                                system("cls");
                                display_array_words(operacja_2, sizeof(operacja_2) / sizeof(W), hConsole);
                                getline(cin, line);                    
                                words_login = line_to_array(line);
                                if (words_login.size() != 2) {
                                    system("cls");
                                    break;
                                }
                                suma_do_wyplaty = stod(words_login[1]);
                                line = to_lower_case(words_login[0]);
                                system("cls");
                                if (suma_do_wyplaty >= 0 && suma_do_wyplaty <= konto->get_suma()) {
                                    find = false;                                
                                    for (KontoBankowe& b : konta) {
                                        if (b.get_login() == line) {
                                            find = true;
                                            ptr_do_przelewu = &b;
                                            break;
                                        }
                                    }
                                    if (find) {
                                        ptr_do_przelewu->doladuj(suma_do_wyplaty);
                                        konto->wyplac(suma_do_wyplaty, hConsole);
                                        display_array_words(operacja_2_przelano, sizeof(operacja_2_przelano) / sizeof(W), hConsole);
                                    }
                                    else {
                                        display_array_words(operacja_2_niemakonta, sizeof(operacja_2_niemakonta) / sizeof(W), hConsole);
                                    }
                                }
                                else {
                                    display_array_words(operacja_2_niemaszpieniedzy, sizeof(operacja_2_niemaszpieniedzy) / sizeof(W), hConsole);
                                }
                                break;
                            case 3:
                                usun_konto(konta, konto);
                                system("cls");
                                display_array_words(usunieto_konto, sizeof(usunieto_konto)/sizeof(W), hConsole);
                                return;
                                break;
                            case 4:
                                system("cls");
                                return;
                                break;
                        }
                    }
                    else {
                        system("cls");
                        display_array_words(blad, sizeof(blad) / sizeof(W), hConsole);
                    }
                    
                }
                catch (...) {
                    system("cls");
                    display_array_words(blad, sizeof(blad) / sizeof(W), hConsole);
                }
            }
        }
        else {
            system("cls");
            display_array_words(niepoprawne_haslo, sizeof(niepoprawne_haslo) / sizeof(W), hConsole);
        }       
    }
    else {
        system("cls");
        display_array_words(konto_nieistnieje, sizeof(konto_nieistnieje) / sizeof(W), hConsole);
    }
}

void nowe_konto(HANDLE& hConsole, vector<KontoBankowe>& konta) {
    W header_menu[] = { W(green, "Jestes w trybie tworzenia konta") };
    W podpowiedz[] = { W(green, "Podaj : "), W(red, "login "), W(red, "haslo "), W(yellow, "ilosc pieniedzy"), W(green, ", odzielone spacja.") };
    W niepoprawne_dane[] = { W(red, "Niepoprawne dane") };
    
    
    string line;
    vector<string> words;
    double poczatkowa_wartosc;
    system("cls");
    bool find = false;

    while (true) {
        display_array_words(header_menu, sizeof(header_menu) / sizeof(W), hConsole);
        display_array_words(podpowiedz, sizeof(podpowiedz) / sizeof(W), hConsole);
        getline(cin, line);
        words = line_to_array(line);
        find = false;
        if (words.size() == 3) {
            for (KontoBankowe &konto : konta) {
                if (konto.get_login() == words[0]) {
                    find = true;
                    break;
                }
            }
            try {
                poczatkowa_wartosc = stod(words[2]);

            }
            catch (...) {
                system("cls");
                display_array_words(niepoprawne_dane, sizeof(niepoprawne_dane) / sizeof(W), hConsole);
                break;

            }

            if (find) {
                system("cls");
                W juz_istnieje[] = { W(green, "Konto o loginie : "), W(red, words[0]), W(green, " juz istnieje.")};
                display_array_words(juz_istnieje, sizeof(juz_istnieje) / sizeof(W), hConsole);
            }
            else {
                system("cls");
                W konto_zostalo_stworzone[] = { W(green, "Stworzyles konto o loginie : "), W(red, words[0])};
                display_array_words(konto_zostalo_stworzone, sizeof(konto_zostalo_stworzone) / sizeof(W), hConsole);
                konta.push_back(KontoBankowe(words[0], words[1], poczatkowa_wartosc));
                return;
            }


        }
        else {
            system("cls");
            display_array_words(niepoprawne_dane, sizeof(niepoprawne_dane) / sizeof(W), hConsole);
        }

    
    }

}
string to_lower_case(string& text) {
    string new_text;
    for (char c : text) {
        new_text.push_back(tolower(c));
    }
    return new_text;   
}
void usun_konto(vector<KontoBankowe> &konta, KontoBankowe* konto_do_usuniecia) {

    int index = 0;
    for (KontoBankowe &konto : konta) {
        if (konto.get_login() == konto_do_usuniecia->get_login()) {
            konta.erase(konta.begin() + index);
            break;
        }
        index++;
    }
}