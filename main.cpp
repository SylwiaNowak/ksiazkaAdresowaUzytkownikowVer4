#include <iostream>
#include <vector>
#include <windows.h> // funkcja system("cls")
#include <cstdlib> // funkcja exit(0), void exit (int status);
#include <fstream> //biblioteka odpowiedzialna za wspolprace z plikami
#include <sstream>
#include <algorithm> // funkcja transform()
#include <string>

using namespace std;

string plikUzytkownicy ("rejestracjaILogowanie.txt");
string plikAdresaci ("ksiazkaAdresowa.txt");
string plikAdresaciTymczasowy ("ksiazkaAdresowaTymczasowa.txt");

struct Uzytkownik {
    int idUzytkownika = 0;
    string nazwa = "", haslo = "";
};

struct Adresat {
    int idAdresata = 0;
    string imie = "", nazwisko = "", nrTelefonu = "", email = "", adres = "";
    Uzytkownik idZalogowanegoUzytkownika;
};

void wyswietlGlowneMenu(vector<Uzytkownik> &uzytkownicy, vector<Adresat> &adresaci);
char wczytajZnak();
void wczytajUzytkownikowZPlikuTxt(vector<Uzytkownik> &uzytkownicy);
void logowanieUzytkownika(vector<Uzytkownik> &uzytkownicy, vector<Adresat> &adresaci);
void rejestracjaUzytkownika(vector<Uzytkownik> &uzytkownicy);
void zakonczProgram();

int main() {
    vector<Uzytkownik> uzytkownicy;
    vector<Adresat> adresaci;

    wczytajUzytkownikowZPlikuTxt(uzytkownicy);

    wyswietlGlowneMenu(uzytkownicy, adresaci);

    return 0;
}

void wyswietlGlowneMenu(vector<Uzytkownik> &uzytkownicy, vector<Adresat> &adresaci) {
    char wybor;

    while (true) {
        system("cls");
        cout << "1. Logowanie" << endl;
        cout << "2. Rejestracja" << endl;
        cout << "9. Zakoncz program" << endl;
        cout << endl << "Wpisz cyfre wyboru: ";
        wybor = wczytajZnak();

        switch (wybor) {
        case '1':
            logowanieUzytkownika(uzytkownicy, adresaci);
            break;
        case '2':
            rejestracjaUzytkownika(uzytkownicy);
            break;
        case '9':
            zakonczProgram();
            break;
        }
    }
}

char wczytajZnak() {
    string ciagZnakow;
    char znak;

    getline(cin, ciagZnakow);
    for (int i = 0; i != ciagZnakow.length(); i++) {
        if (ciagZnakow.length() == 1) {
            znak = ciagZnakow[0];
        } else cout << "ERROR! Wpisz ponownie cyfre wyboru: ";
    }
    return znak;
}

string wczytajLinie() {
    string ciagZnakow = "";
    getline (cin, ciagZnakow);
    return ciagZnakow;
}

string konwersjaIntNaString(int zmienna) {
    ostringstream ss;
    ss << zmienna;
    string str = ss.str();

    return str;
}

Uzytkownik wczytajPojedynczegoUzytkownika(string liniaZDanymiDoLogowaniaUzytkownika) {
    Uzytkownik klient;
    string pojedynczaDanaDoLogowaniaUzytkownika = "";
    int numerPojedynczejDanejUzytkownika = 1;

    for (int i = 0; i != liniaZDanymiDoLogowaniaUzytkownika.length(); i++) {
        if (liniaZDanymiDoLogowaniaUzytkownika[i] != '|') {
            pojedynczaDanaDoLogowaniaUzytkownika += liniaZDanymiDoLogowaniaUzytkownika[i];
        } else {
            switch (numerPojedynczejDanejUzytkownika) {
            case 1:
                klient.idUzytkownika = atoi(pojedynczaDanaDoLogowaniaUzytkownika.c_str());
                break;
            case 2:
                klient.nazwa = pojedynczaDanaDoLogowaniaUzytkownika;
                break;
            case 3:
                klient.haslo = pojedynczaDanaDoLogowaniaUzytkownika;
                break;
            }
            numerPojedynczejDanejUzytkownika++;
            pojedynczaDanaDoLogowaniaUzytkownika = "";
        }
    }
    return klient;
}

void wczytajUzytkownikowZPlikuTxt(vector<Uzytkownik> &uzytkownicy) {
    Uzytkownik klient;
    string liniaZDanymiDoLogowaniaUzytkownika = "";

    fstream plikTekstowy;
    plikTekstowy.open(plikUzytkownicy.c_str(), ios::in);

    if (plikTekstowy.good() == true) {
        while (getline(plikTekstowy, liniaZDanymiDoLogowaniaUzytkownika)) {

            klient = wczytajPojedynczegoUzytkownika(liniaZDanymiDoLogowaniaUzytkownika);
            uzytkownicy.push_back(klient);
        }
        plikTekstowy.close();
        plikTekstowy.clear();
    }
}

bool sprawdzCzyNazwaUzytkownikaIstnieje(vector<Uzytkownik> &uzytkownicy, Uzytkownik klient) {
    for (vector<Uzytkownik> ::iterator itr = uzytkownicy.begin(); itr != uzytkownicy.end(); itr++) {
        if (itr -> nazwa == klient.nazwa) {
            return true;
        }
    }
    return false;
}

bool sprawdzCzyPoprawneHaslo(vector<Uzytkownik> &uzytkownicy, Uzytkownik klient) {
    for (vector<Uzytkownik> ::iterator itr = uzytkownicy.begin(); itr != uzytkownicy.end(); itr++) {
        if (itr -> haslo == klient.haslo) {
            return true;
        }
    }
    return false;
}

int pobierzIdZalogowanegoKlienta(vector<Uzytkownik> &uzytkownicy, Uzytkownik klient) {
    for (vector<Uzytkownik>::iterator itr = uzytkownicy.begin(); itr != uzytkownicy.end(); itr++) {
        if (itr -> nazwa == klient.nazwa) {
            klient.idUzytkownika = itr -> idUzytkownika;
        }
    }
    return klient.idUzytkownika;
}

void dodanieUzytkownikaDoPlikuTxt(Uzytkownik klient) {
    fstream plikTekstowy;
    plikTekstowy.open(plikUzytkownicy.c_str(), ios::out | ios::app);

    if (plikTekstowy.good() == true) {
        if (klient.idUzytkownika == 1) {
            plikTekstowy << konwersjaIntNaString(klient.idUzytkownika) << '|';
        } else {
            plikTekstowy << endl << konwersjaIntNaString(klient.idUzytkownika) << '|';
        }
        plikTekstowy << klient.nazwa << '|';
        plikTekstowy << klient.haslo << '|';
        plikTekstowy.close();
        plikTekstowy.clear();

        cout << "---Konto zalozone---" << endl;
        system("pause");
    } else {
        cout << "Nie udalo sie otworzyc pliku i zapisac w nim danych" << endl;
        system("pause");
    }
}

void nadpiszWszystkichUzytkownikowDoPlikuTekstowego(vector<Uzytkownik> &uzytkownicy) {
    string liniaZDanymiUzytkownika = "";

    fstream plikTekstowy;
    plikTekstowy.open(plikUzytkownicy.c_str(), ios::out);

    if (plikTekstowy.good()) {
        for (vector<Uzytkownik> ::iterator itr = uzytkownicy.begin(); itr != uzytkownicy.end(); itr++) {
            liniaZDanymiUzytkownika += (konwersjaIntNaString(itr -> idUzytkownika)) + '|';
            liniaZDanymiUzytkownika += itr -> nazwa + '|';
            liniaZDanymiUzytkownika += itr -> haslo + '|';

            plikTekstowy << liniaZDanymiUzytkownika << endl;
            liniaZDanymiUzytkownika = "";
        }
        plikTekstowy.close();
    } else {
        cout << "Nie można otworzyć pliku." << endl;
    }
}

void zmianaHasla(vector<Uzytkownik> &uzytkownicy, Uzytkownik klient, int idZalogowanegoKlienta) {
    cout << "Podaj nowe haslo: ";
    for (vector<Uzytkownik>::iterator itr = uzytkownicy.begin(); itr != uzytkownicy.end(); itr++) {
        if (itr -> idUzytkownika == idZalogowanegoKlienta) {
            itr -> haslo = wczytajLinie();
            cout << "Haslo zostalo pomyslnie zmienione." << endl;
            //cout << "nowe haslo: " << itr -> haslo << endl;
            //cout << "ID: " << idZalogowanegoKlienta << endl;
            system("pause");
            break;
        }
    }
    nadpiszWszystkichUzytkownikowDoPlikuTekstowego(uzytkownicy);
}

/////////////////////KSIZKA ADRESOWA FUNKCJE///////////////////////////////////////////

void wczytajAdresatowDanegoUzytkownikaZPlikuTxt(vector<Adresat> &adresaci, int idZalogowanegoKlienta);
void wprowadzDaneNowegoAdresata(vector<Adresat> &adresaci, int idZalogowanegoKlienta);
void wyszukajAdresataWedlugImie(vector<Adresat> &adresaci);
void wyszukajAdresataWedlugNazwisko(vector<Adresat> &adresaci);
void wyswietlKsiazkeAdresowaDanegoUzytkownika(vector<Uzytkownik> &uzytkownicy, vector<Adresat> &adresaci, int idZalogowanegoKlienta);
void wylogujSie(int idZalogowanegoKlienta);
void usunAdresata(vector<Uzytkownik> &uzytkownicy, vector<Adresat> &adresaci);
void edytujAdresata(vector<Uzytkownik> &uzytkownicy, vector<Adresat> &adresaci);
void wylogujSie(vector<Uzytkownik> &uzytkownicy, vector<Adresat> &adresaci, int idZalogowanegoKlienta);

////////////////////KONIEC KSIAZKA ADRESOWA FUNKCJE///////////////////////////////////


void wyswietlPanelZarzadzaniaAdresatami(vector<Uzytkownik> &uzytkownicy, Uzytkownik klient, int idZalogowanegoKlienta, vector<Adresat> &adresaci) {
    char wybor;

    wczytajAdresatowDanegoUzytkownikaZPlikuTxt(adresaci, idZalogowanegoKlienta);

    while(true) {
        system("cls");
        cout << "KSIAZKA ADRESOWA" << endl << endl;
        cout << "1. Dodaj adresata" << endl;
        cout << "2. Wyszukaj po imieniu" << endl;
        cout << "3. Wyszukaj po nazwisku" << endl;
        cout << "4. Wyswietl wszystkich adresatow" << endl;
        cout << "5. Usun adresata" << endl;
        cout << "6. Edytuj adresata" << endl;
        cout << "7. Zmien haslo" << endl;
        cout << "8. wyloguj sie" << endl;
        cout << endl << "Twoj wybor: ";
        wybor = wczytajZnak();

        switch (wybor) {
        case '1':
            wprowadzDaneNowegoAdresata(adresaci, idZalogowanegoKlienta);
            break;
        case '2':
            wyszukajAdresataWedlugImie(adresaci);
            break;
        case '3':
            wyszukajAdresataWedlugNazwisko(adresaci);
            break;
        case '4':
            wyswietlKsiazkeAdresowaDanegoUzytkownika(uzytkownicy, adresaci, idZalogowanegoKlienta);
            break;
        case '5':
            usunAdresata(uzytkownicy, adresaci);
            break;
        case '6':
            edytujAdresata(uzytkownicy, adresaci);
            break;
        case '7':
            zmianaHasla(uzytkownicy, klient, idZalogowanegoKlienta);
            break;
        case '8':
            wylogujSie(uzytkownicy, adresaci, idZalogowanegoKlienta);
            break;
        }
    }
}

void logowanieUzytkownika(vector<Uzytkownik> &uzytkownicy, vector<Adresat> &adresaci) {
    Uzytkownik klient;

    int idZalogowanegoKlienta = 0;
    bool czyIstniejeNazwa = false;
    bool czyPoprawneHaslo = false;

    cout <<"-----LOGOWANIE DO SYSTEMU-----" << endl;
    cout << "Podaj nazwe: ";
    klient.nazwa = wczytajLinie();
    czyIstniejeNazwa = sprawdzCzyNazwaUzytkownikaIstnieje(uzytkownicy, klient);

    if (czyIstniejeNazwa == false) {
        cout << "Nie ma takiej nazwy uzytkownika. Powrot do panelu glownego." << endl;
        Sleep(2000);
    } else {
        cout << "Podaj haslo (masz 3 proby): ";

        for (int i = 2; i >= 0; i--) {
            klient.haslo = wczytajLinie();
            czyPoprawneHaslo = sprawdzCzyPoprawneHaslo(uzytkownicy, klient);
            if (czyPoprawneHaslo == false) {
                if (i == 0) {
                    cout << "Niepoprawne haslo, powrot do panelu glownego." << endl;
                    Sleep(2000);
                    break;
                }
                cout << "Logowanie do systemu nie powiodlo sie. Sprobuj ponownie (ilosc prob: " << i << "): " << endl;
                Sleep(2000);
            } else {
                idZalogowanegoKlienta = pobierzIdZalogowanegoKlienta(uzytkownicy, klient);
                //cout << "idZalogowanegoKlienta: " << idZalogowanegoKlienta << endl;
                //system("pause");
                wyswietlPanelZarzadzaniaAdresatami(uzytkownicy, klient, idZalogowanegoKlienta, adresaci);
                break;
            }
        }
    }
}

void rejestracjaUzytkownika(vector<Uzytkownik> &uzytkownicy) {
    Uzytkownik klient;
    bool poprawnaNazwa = false;

    system("cls");
    cout << "---Dodanie nowego uzytkownika---" << endl;

    if (uzytkownicy.empty() == true) {
        klient.idUzytkownika = 1;
    } else {
        klient.idUzytkownika = uzytkownicy.back().idUzytkownika + 1;
    }

    cout << "Podaj nazwe uzytkownika: ";
    while (true) {
        klient.nazwa = wczytajLinie();
        poprawnaNazwa = sprawdzCzyNazwaUzytkownikaIstnieje(uzytkownicy, klient);
        if (poprawnaNazwa == true) {
            cout << "Podana nazwa uzytkownika juz istnieje. Podaj nowa nazwe uzytkownika: ";
        } else {
            cout << "Poprawna nazwa uzytkownika." << endl;
            break;
        }
    }

    cout << "Podaj haslo uzytkownika: ";
    klient.haslo = wczytajLinie();

    uzytkownicy.push_back(klient);

    dodanieUzytkownikaDoPlikuTxt(klient);
}

void zakonczProgram() {
    system("cls");
    cout << endl << "*****Koniec programu*****" << endl;
    exit(0);
}

//////////////////////KSIAZKA ADRESOWA/////////////////////////////////////////////////

int wczytajLiczbeCalkowita() {
    string wejscie = "";
    int liczba = 0;

    while (true) {
        getline(cin, wejscie);

        stringstream myStream(wejscie);
        if (myStream >> liczba)
            break;
        cout << "To nie jest liczba, wpisz ponownie: ";
    }
    return liczba;
}

string zamienPierwszaLitereNaDuzaAPozostaleNaMale(string wyraz) {
    if (wyraz.empty() == false) {
        transform (wyraz.begin(), wyraz.end(), wyraz.begin(), ::tolower);
        wyraz[0] = toupper(wyraz[0]);
    }
    return wyraz;
}

Adresat pobierzDaneAdresata(string daneAdresataZapisaneWLinii) {
    Adresat osoba;
    string pojedynczaDanaAdresata = "";
    int numerPojedynczejDanejAdresata = 1;

    for (int i = 0; i < daneAdresataZapisaneWLinii.length(); i++) {
        if (daneAdresataZapisaneWLinii[i] != '|') {
            pojedynczaDanaAdresata += daneAdresataZapisaneWLinii[i];
        } else {
            switch (numerPojedynczejDanejAdresata) {
            case 1:
                osoba.idAdresata = atoi(pojedynczaDanaAdresata.c_str());
                break;
            case 2:
                osoba.idZalogowanegoUzytkownika.idUzytkownika = atoi(pojedynczaDanaAdresata.c_str());
                break;
            case 3:
                osoba.imie = pojedynczaDanaAdresata;
                break;
            case 4:
                osoba.nazwisko = pojedynczaDanaAdresata;
                break;
            case 5:
                osoba.nrTelefonu = pojedynczaDanaAdresata;
                break;
            case 6:
                osoba.email = pojedynczaDanaAdresata;
                break;
            case 7:
                osoba.adres = pojedynczaDanaAdresata;
                break;
            }
            pojedynczaDanaAdresata = "";
            numerPojedynczejDanejAdresata++;
        }
    }
    return osoba;
}

bool sprawdzCzyAdresatNalezyDoKsiazkiAdresowejUzytkownika(Adresat osoba, int idZalogowanegoKlienta) {
    if (osoba.idZalogowanegoUzytkownika.idUzytkownika == idZalogowanegoKlienta)
        return true;
    return false;
}

void wczytajAdresatowDanegoUzytkownikaZPlikuTxt(vector<Adresat> &adresaci, int idZalogowanegoKlienta) {
    Adresat osoba;
    string daneAdresataZapisaneWLinii = "";
    bool adresatNalezyDoKsiazkiAdresowejUzytkownika = false;

    fstream plikTekstowy;
    plikTekstowy.open(plikAdresaci.c_str(), ios::in);

    if (plikTekstowy.good() == true) {
        while (getline (plikTekstowy, daneAdresataZapisaneWLinii)) {
            osoba = pobierzDaneAdresata(daneAdresataZapisaneWLinii);
            adresatNalezyDoKsiazkiAdresowejUzytkownika = sprawdzCzyAdresatNalezyDoKsiazkiAdresowejUzytkownika(osoba, idZalogowanegoKlienta);

            if (adresatNalezyDoKsiazkiAdresowejUzytkownika == true) {
                adresaci.push_back(osoba);
                //cout << "ID: " << idZalogowanegoKlienta << " - " << osoba.idAdresata << endl;
                //system("pause");
            }
        }
    }
    plikTekstowy.close();
    plikTekstowy.clear();
}

void dopiszAdresataDoPlikuTxt(Adresat osoba, Uzytkownik klient) {
//DODANIE OSOBY DO PLIKU TEKSTOWEGO
    fstream plikTekstowy;
    plikTekstowy.open(plikAdresaci.c_str(), ios::out | ios::app); //input output stream out - wyprowadz
    //czyli pokieruj strumien danych z programu do pliku
    // metoda c_str() konwertuje ciag znaków zapisany w zmiennej typu string na ciag który moze byæ zapisany w tablicy znaków
    // oznacza to, ze bez tego nie da się operowac na zmiennych typu string zapisanych w pliku *txt
    // bo nie bedzie mozna ich zapisac do tablicy znakow

    if (plikTekstowy.good() == true) {
        //wyrzucenie danej strumieniem do pliku na dysk:
        if (osoba.idAdresata == 1) {
            plikTekstowy << osoba.idAdresata << '|';
        } else {
            plikTekstowy << endl << osoba.idAdresata << '|';
        }
        plikTekstowy << osoba.idZalogowanegoUzytkownika.idUzytkownika << '|';
        plikTekstowy << osoba.imie << '|';
        plikTekstowy << osoba.nazwisko << '|';
        plikTekstowy << osoba.nrTelefonu << '|';
        plikTekstowy << osoba.email << '|';
        plikTekstowy << osoba.adres << '|';

        plikTekstowy.close();
        plikTekstowy.clear();

        cout << "Adresat dodany";
        Sleep(2000);
    } else {
        cout << "Nie mozna otworzyc pliku \"ksiazka_adresowa.txt\"" << endl;
    }
}

void wprowadzDaneNowegoAdresata(vector<Adresat> &adresaci, int idZalogowanegoKlienta) {
    Adresat osoba;
    Uzytkownik klient;

    //WCZYTANIE DANYCH ADRESATA DO STRUKTURY:
    system("cls");

    if (adresaci.empty() == true) {
        osoba.idAdresata = 1;
    } else {
        osoba.idAdresata = adresaci.back().idAdresata + 1;
    }

    osoba.idZalogowanegoUzytkownika.idUzytkownika = idZalogowanegoKlienta; //zapamietajAktualneIdUzytkownika(uzytkownicy);
    cout << "Podaj imie nowego adresata: ";
    osoba.imie = wczytajLinie();
    osoba.imie = zamienPierwszaLitereNaDuzaAPozostaleNaMale(osoba.imie);
    cout << "Podaj nazwisko nowego adresata: ";
    osoba.nazwisko = wczytajLinie();
    osoba.nazwisko = zamienPierwszaLitereNaDuzaAPozostaleNaMale(osoba.nazwisko);
    cout << "Podaj numer telefonu nowego adresata: ";
    osoba.nrTelefonu = wczytajLinie();
    cout << "Podaj adres email nowego adresata: ";
    osoba.email = wczytajLinie();
    cout << "Podaj adres do korespondencji nowego adresata: ";
    osoba.adres = wczytajLinie();

    //WCZYTANIE DANYCH ADRESATA DO VECTORA:
    adresaci.push_back(osoba);

    dopiszAdresataDoPlikuTxt(osoba, klient);
}

void wyszukajAdresataWedlugImie(vector<Adresat> &adresaci) {
    string szukaneImie = "";
    int iloscZnalezionychWynikow = 0;

    system("cls");
    if (!adresaci.empty()) {
        cout << "Podaj imie, ktore wyszukac: ";
        szukaneImie = wczytajLinie();
        szukaneImie = zamienPierwszaLitereNaDuzaAPozostaleNaMale(szukaneImie);

        for (vector<Adresat> ::iterator itr = adresaci.begin(); itr != adresaci.end(); itr++) {
            if (itr -> imie == szukaneImie) {
                cout << itr -> idAdresata << endl;
                cout << itr -> imie << endl;
                cout << itr -> nazwisko << endl;
                cout << itr -> nrTelefonu << endl;
                cout << itr -> email << endl;
                cout << itr -> adres << endl;

                iloscZnalezionychWynikow++;
            }
        }

        if (iloscZnalezionychWynikow == 0) {
            cout << endl << "Nie znaleziono adresatow o podanym imieniu" << endl;
        } else {
            cout << endl << "Ilosc znalezionych adresatow o podanym imieniu wynosi: " << iloscZnalezionychWynikow << endl;
        }
    } else {
        cout << "Ksiazka adresowa jest pusta." << endl;
    }
    system("pause");
}

void wyszukajAdresataWedlugNazwisko(vector<Adresat> &adresaci) {
    string szukaneNazwisko = "";
    int iloscZnalezionychWynikow = 0;

    system("cls");
    if (!adresaci.empty()) {
        cout << "Podaj nazwisko, ktore wyszukac: ";
        szukaneNazwisko = wczytajLinie();
        szukaneNazwisko = zamienPierwszaLitereNaDuzaAPozostaleNaMale(szukaneNazwisko);

        for (vector<Adresat> ::iterator itr = adresaci.begin(); itr != adresaci.end(); itr++) {
            if (itr -> nazwisko == szukaneNazwisko) {
                cout << itr -> idAdresata << endl;
                cout << itr -> imie << endl;
                cout << itr -> nazwisko << endl;
                cout << itr -> nrTelefonu << endl;
                cout << itr -> email << endl;
                cout << itr -> adres << endl;

                iloscZnalezionychWynikow++;
            }
        }

        if (iloscZnalezionychWynikow == 0) {
            cout << endl << "Nie znaleziono adresatow o podanym nazwisku" << endl;
        } else {
            cout << endl << "Ilosc znalezionych adresatow o podanym nazwisku wynosi: " << iloscZnalezionychWynikow << endl;
        }
    }

    else {
        cout << "Ksiazka adresowa jest pusta." << endl;
    }
    system("pause");
}

void wyswietlKsiazkeAdresowaDanegoUzytkownika(vector<Uzytkownik> &uzytkownicy, vector<Adresat> &adresaci, int idZalogowanegoKlienta) {
    Adresat osoba;
    Uzytkownik klient;

    system("cls");
    if (!adresaci.empty()) {
        for (vector<Adresat>::iterator itr = adresaci.begin(); itr != adresaci.end(); itr++) {
            if (itr -> idZalogowanegoUzytkownika.idUzytkownika == idZalogowanegoKlienta) {
                cout << "ID adresata: " << itr -> idAdresata << endl;
                cout << "ID Uzytkownika: " << itr -> idZalogowanegoUzytkownika.idUzytkownika << endl;
                cout << "Imie: " << itr -> imie << endl;
                cout << "Nazwisko: " << itr -> nazwisko << endl;
                cout << "numer telefonu: " << itr -> nrTelefonu << endl;
                cout << "email: " << itr -> email << endl;
                cout << "adres: " << itr -> adres << endl;
            }
        }
        cout << endl;
    } else {
        cout << "Ksiazka adresowa jest pusta" << endl;
    }
    system("pause");
}

bool usunPlikAdresaciOryginalny () {
    if( remove( "ksiazkaAdresowa.txt" ) == 0 ) {
        return true;
    } else
        return false;
}

bool podmienPlikAdresaciTymczasowyNaOryginalny () {
    if( rename( "ksiazkaAdresowaTymczasowa.txt", "ksiazkaAdresowa.txt" ) == 0 ) {
        return true;
    } else
        return false;
}

void nadpiszAdresatowDoPlikuTekstowego (vector<Adresat> &adresaci, int idAdresataDoEdycjiLubUsuniecia) {
    Adresat osobaZOryginalnegoPliku;
    string daneAdresataZOryginalnegoPlikuZapisaneWLinii = "";
    string liniaZDanymiAdresata = "";

    fstream plikTekstowyOrg;
    plikTekstowyOrg.open(plikAdresaci.c_str(), ios::in);
    fstream plikTekstowyTymczasowy;
    plikTekstowyTymczasowy.open(plikAdresaciTymczasowy.c_str(), ios::out);

    if (plikTekstowyOrg.good() == true) {
        while(getline(plikTekstowyOrg,daneAdresataZOryginalnegoPlikuZapisaneWLinii)) {
            osobaZOryginalnegoPliku = pobierzDaneAdresata(daneAdresataZOryginalnegoPlikuZapisaneWLinii);

            if (osobaZOryginalnegoPliku.idAdresata != idAdresataDoEdycjiLubUsuniecia) {
                plikTekstowyTymczasowy << osobaZOryginalnegoPliku.idAdresata << '|';
                plikTekstowyTymczasowy << osobaZOryginalnegoPliku.idZalogowanegoUzytkownika.idUzytkownika << '|';
                plikTekstowyTymczasowy << osobaZOryginalnegoPliku.imie << '|';
                plikTekstowyTymczasowy << osobaZOryginalnegoPliku.nazwisko << '|';
                plikTekstowyTymczasowy << osobaZOryginalnegoPliku.nrTelefonu << '|';
                plikTekstowyTymczasowy << osobaZOryginalnegoPliku.email << '|';
                plikTekstowyTymczasowy << osobaZOryginalnegoPliku.adres << '|' << endl;

            } else {
                for (vector<Adresat> ::iterator itr = adresaci.begin(); itr != adresaci.end(); itr++) {
                    if (itr -> idAdresata == idAdresataDoEdycjiLubUsuniecia) {
                        liniaZDanymiAdresata += konwersjaIntNaString(itr -> idAdresata) + '|';
                        liniaZDanymiAdresata += konwersjaIntNaString(itr -> idZalogowanegoUzytkownika.idUzytkownika) + '|';
                        liniaZDanymiAdresata += itr->imie + '|';
                        liniaZDanymiAdresata += itr->nazwisko + '|';
                        liniaZDanymiAdresata += itr->nrTelefonu + '|';
                        liniaZDanymiAdresata += itr->email + '|';
                        liniaZDanymiAdresata += itr->adres + '|';

                        plikTekstowyTymczasowy << liniaZDanymiAdresata << endl;
                        liniaZDanymiAdresata = "";
                    }
                }
            }
        }
        plikTekstowyTymczasowy.close();
        plikTekstowyOrg.close();
        usunPlikAdresaciOryginalny();
        podmienPlikAdresaciTymczasowyNaOryginalny();
    } else {
        cout << "Nie mozna otworzyc pliku ksiazkaAdresowa.txt" << endl;
    }
}

void usunAdresata(vector<Uzytkownik> &uzytkownicy, vector<Adresat> &adresaci) {
    int idUsuwanegoAdresata = 0;
    char wybor;
    bool czyIstniejeAdresat = false;

    system("cls");
    if (!adresaci.empty()) {
        cout << "Wybierz ID adresata do usuniecia: ";
        idUsuwanegoAdresata = wczytajLiczbeCalkowita();

        for (vector<Adresat> ::iterator itr = adresaci.begin(); itr != adresaci.end(); itr++) {
            if (itr -> idAdresata == idUsuwanegoAdresata) {
                czyIstniejeAdresat = true;
                cout << "Potwierdz naciskajac klawisz 't': ";
                wybor = wczytajZnak();
                if (wybor == 't') {
                    adresaci.erase(itr);
                    cout << "Wskazany adresat zostal usuniety" << endl;
                    nadpiszAdresatowDoPlikuTekstowego(adresaci, idUsuwanegoAdresata);
                    break;
                } else {
                    cout << "Wybrany adresat nie zostal usuniety" << endl;
                    break;
                }
            }
        }
        if (czyIstniejeAdresat == false) {
            cout << "Szukany adresat nie istnieje" << endl;
        }
    } else {
        cout << "Ksiazka adresowa jest pusta" << endl;
    }
    system("pause");
}

void edytujAdresata(vector<Uzytkownik> &uzytkownicy, vector<Adresat> &adresaci) {
    int idAdresataDoEdycji = 0;
    bool czyIstniejeAdresat = false;
    char wybor;

    system("cls");
    if (!adresaci.empty()) {
        cout << "W celu edycji, podaj ID wybranego adresata: ";
        idAdresataDoEdycji = wczytajLiczbeCalkowita();

        for (vector<Adresat> ::iterator itr = adresaci.begin(); itr != adresaci.end(); itr++) {
            if (itr -> idAdresata == idAdresataDoEdycji) {
                czyIstniejeAdresat = true;

                cout << "Wskaz jaka dana edytowa: " << endl;
                cout << "1. - imie" << endl;
                cout << "2. - nazwisko" << endl;
                cout << "3. - numer telefonu" << endl;
                cout << "4. - e-mail" << endl;
                cout << "5. - adres" << endl;
                cout << "6. - powrot do menu" << endl;
                wybor = wczytajZnak();

                switch (wybor) {
                case '1':
                    cout << "Podaj nowe imie: " << endl;
                    itr -> imie = wczytajLinie();
                    itr -> imie = zamienPierwszaLitereNaDuzaAPozostaleNaMale(itr -> imie);
                    cout << "Imie zostalo edytowane" << endl;
                    nadpiszAdresatowDoPlikuTekstowego(adresaci, idAdresataDoEdycji);
                    break;
                case '2':
                    cout << "Podaj nowe nazwisko: " << endl;
                    itr -> nazwisko = wczytajLinie();
                    itr -> nazwisko = zamienPierwszaLitereNaDuzaAPozostaleNaMale(itr -> nazwisko);
                    cout << "Nazwisko zostalo edytowane" << endl;
                    nadpiszAdresatowDoPlikuTekstowego(adresaci, idAdresataDoEdycji);
                    break;
                case '3':
                    cout << "Podaj nowy numer telefonu: " << endl;
                    itr -> nrTelefonu = wczytajLinie();
                    cout << "Numer telefonu zostal edytowany" << endl;
                    nadpiszAdresatowDoPlikuTekstowego(adresaci, idAdresataDoEdycji);
                    break;
                case '4':
                    cout << "Podaj nowy emial: " << endl;
                    itr -> email = wczytajLinie();
                    cout << "E-mail zostal edytowany" << endl;
                    nadpiszAdresatowDoPlikuTekstowego(adresaci, idAdresataDoEdycji);
                    break;
                case '5':
                    cout << "Podaj nowy adres: " << endl;
                    itr -> adres = wczytajLinie();
                    cout << "Adres zostal edytowany" << endl;
                    nadpiszAdresatowDoPlikuTekstowego(adresaci, idAdresataDoEdycji);
                    break;
                case '6':
                    cout << "Powrot do menu" << endl;
                    break;
                default:
                    cout << "Niewlasciwa cyfra wyboru, sprobuj ponownie: " << endl;
                    break;
                }
            }
        }

        if (czyIstniejeAdresat == false) {
            cout << "Nie ma takiego adresata" << endl;
        }
    } else {
        cout << "Ksiazka adresowa jest pusta" << endl;
    }
    system("pause");
}

void wylogujSie(vector<Uzytkownik> &uzytkownicy, vector<Adresat> &adresaci, int idZalogowanegoKlienta) {
    idZalogowanegoKlienta = 0;
    wyswietlGlowneMenu(uzytkownicy, adresaci);
}
