#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include <conio.h>
#include <cctype>
#include <windows.h>
#include <limits>
using namespace std;
struct Gracz
{
    string nazwa;
    int wygrane;
    int remisy;
    int przegrane;
};
string toLowercase(const string &str)
{
    string result = str;
    for (char &c : result)
    {
        c = tolower(c);
    }
    return result;
}
void wyswietl_plansze(const vector<vector<char>> &plansza, int aktualny_wiersz, int aktualna_kolumna)
{
    system("cls");
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (i == aktualny_wiersz && j == aktualna_kolumna)
            {
                cout << "[" << plansza[i][j] << "] ";
            }
            else
            {
                cout << " " << plansza[i][j] << "  ";
            }
        }
        cout << endl
             << "-----------" << endl;
    }
}
bool czy_wygrana(const vector<vector<char>> &plansza, char znak)
{
    for (int i = 0; i < 3; ++i)
    {
        if ((plansza[i][0] == znak && plansza[i][1] == znak && plansza[i][2] == znak) ||
            (plansza[0][i] == znak && plansza[1][i] == znak && plansza[2][i] == znak))
        {
            return true;
        }
    }
    if ((plansza[0][0] == znak && plansza[1][1] == znak && plansza[2][2] == znak) ||
        (plansza[0][2] == znak && plansza[1][1] == znak && plansza[2][0] == znak))
    {
        return true;
    }
    return false;
}
bool czy_pelna(const vector<vector<char>> &plansza)
{
    for (const auto &wiersz : plansza)
    {
        for (char pole : wiersz)
        {
            if (pole == ' ')
            {
                return false;
            }
        }
    }
    return true;
}
pair<int, int> pytaj_o_ruch(const vector<vector<char>> &plansza, char znak_gracza)
{
    int wiersz = 0, kolumna = 0;
    char klawisz;
    while (true)
    {
        wyswietl_plansze(plansza, wiersz, kolumna);
        klawisz = getch();
        switch (klawisz)
        {
        case 72:
            wiersz = (wiersz - 1 + 3) % 3;
            break;
        case 80:
            wiersz = (wiersz + 1) % 3;
            break;
        case 75:
            kolumna = (kolumna - 1 + 3) % 3;
            break;
        case 77:
            kolumna = (kolumna + 1) % 3;
            break;
        case 13:
            if (plansza[wiersz][kolumna] == ' ')
            {
                return make_pair(wiersz, kolumna);
            }
            else
            {
                cout << "To pole jest juz zajete. Sprobuj ponownie." << endl;
            }
            break;
        }
    }
}
void zapisz_statystyki(const Gracz &gracz)
{
    ifstream plik_statystyki_in("statystyki.txt");
    vector<Gracz> lista_graczy;
    if (plik_statystyki_in.is_open())
    {
        Gracz temp_gracz;
        while (plik_statystyki_in >> temp_gracz.nazwa >> temp_gracz.wygrane >> temp_gracz.remisy >> temp_gracz.przegrane)
        {
            lista_graczy.push_back(temp_gracz);
        }
        plik_statystyki_in.close();
    }
    bool znaleziono = false;
    for (Gracz &istniejacy_gracz : lista_graczy)
    {
        if (toLowercase(istniejacy_gracz.nazwa) == toLowercase(gracz.nazwa))
        {
            istniejacy_gracz.wygrane += gracz.wygrane;
            istniejacy_gracz.remisy += gracz.remisy;
            istniejacy_gracz.przegrane += gracz.przegrane;
            znaleziono = true;
            break;
        }
    }
    if (!znaleziono)
    {
        lista_graczy.push_back(gracz);
    }
    ofstream plik_statystyki_out("statystyki.txt");
    if (plik_statystyki_out.is_open())
    {
        for (const Gracz &aktualny_gracz : lista_graczy)
        {
            plik_statystyki_out << aktualny_gracz.nazwa << " " << aktualny_gracz.wygrane << " " << aktualny_gracz.remisy << " " << aktualny_gracz.przegrane << endl;
        }
        plik_statystyki_out.close();
    }
    else
    {
        cout << "Nie udalo sie otworzyc pliku statystyki.txt do zapisu." << endl;
    }
}
void gra_z_graczem()
{
    vector<vector<char>> plansza(3, vector<char>(3, ' '));
    Gracz gracz1 = {"", 0, 0, 0};
    Gracz gracz2 = {"", 0, 0, 0};
    cout << "Podaj nazwe gracza 1: ";
    cin >> gracz1.nazwa;
    cout << "Podaj nazwe gracza 2: ";
    cin >> gracz2.nazwa;
    char znak_gracza1 = 'X';
    char znak_gracza2 = 'O';
    char aktualny_gracz = znak_gracza1;
    while (true)
    {
        wyswietl_plansze(plansza, 0, 0);
        pair<int, int> ruch = pytaj_o_ruch(plansza, aktualny_gracz);
        plansza[ruch.first][ruch.second] = aktualny_gracz;
        if (czy_wygrana(plansza, aktualny_gracz))
        {
            if (aktualny_gracz == znak_gracza1)
            {
                cout << "Gracz " << gracz1.nazwa << " wygral!" << endl;
                gracz1.wygrane++;
                gracz2.przegrane++;
            }
            else
            {
                cout << "Gracz " << gracz2.nazwa << " wygral!" << endl;
                gracz2.wygrane++;
                gracz1.przegrane++;
            }
            cout << "Nacisnij Enter, aby wrocic do menu glownego..." << endl;
            getch();
            break;
        }
        if (czy_pelna(plansza))
        {
            cout << "Remis! Plansza jest pelna." << endl;
            gracz1.remisy++;
            gracz2.remisy++;
            cout << "Nacisnij Enter, aby wrocic do menu glownego..." << endl;
            getch();
            break;
        }
        aktualny_gracz = (aktualny_gracz == znak_gracza1) ? znak_gracza2 : znak_gracza1;
    }
    zapisz_statystyki(gracz1);
    zapisz_statystyki(gracz2);
}
int ocen_ruch(const vector<vector<char>> &plansza)
{
    if (czy_wygrana(plansza, 'O'))
    {
        return 1;
    }
    else if (czy_wygrana(plansza, 'X'))
    {
        return -1;
    }
    else if (czy_pelna(plansza))
    {
        return 0;
    }
    return -2;
}
pair<int, pair<int, int>> minimax(const vector<vector<char>> &plansza, char gracz)
{
    int ocena = ocen_ruch(plansza);
    if (ocena != -2)
    {
        return make_pair(ocena, make_pair(-1, -1));
    }
    int najlepsza_ocena = (gracz == 'O') ? numeric_limits<int>::min() : numeric_limits<int>::max();
    pair<int, int> najlepszy_ruch = make_pair(-1, -1);
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (plansza[i][j] == ' ')
            {
                vector<vector<char>> nowa_plansza = plansza;
                nowa_plansza[i][j] = gracz;
                int ocena_ruchu = minimax(nowa_plansza, (gracz == 'O') ? 'X' : 'O').first;
                if ((gracz == 'O' && ocena_ruchu > najlepsza_ocena) || (gracz == 'X' && ocena_ruchu < najlepsza_ocena))
                {
                    najlepsza_ocena = ocena_ruchu;
                    najlepszy_ruch = make_pair(i, j);
                }
            }
        }
    }
    return make_pair(najlepsza_ocena, najlepszy_ruch);
}
void gra_z_komputerem()
{
    vector<vector<char>> plansza(3, vector<char>(3, ' '));
    Gracz gracz1 = {"", 0, 0, 0};
    Gracz komputer = {"Komputer", 0, 0, 0};
    cout << "Podaj nazwe gracza 1: ";
    cin >> gracz1.nazwa;
    char znak_gracza1 = 'X';
    char znak_komputera = 'O';
    char aktualny_gracz = znak_gracza1;
    while (true)
    {
        wyswietl_plansze(plansza, 0, 0);
        pair<int, int> ruch;
        if (aktualny_gracz == znak_gracza1)
        {
            ruch = pytaj_o_ruch(plansza, aktualny_gracz);
        }
        else
        {
            pair<int, pair<int, int>> najlepszy_ruch = minimax(plansza, znak_komputera);
            ruch = najlepszy_ruch.second;
        }
        plansza[ruch.first][ruch.second] = aktualny_gracz;
        if (czy_wygrana(plansza, aktualny_gracz))
        {
            if (aktualny_gracz == znak_gracza1)
            {
                cout << "Gracz " << gracz1.nazwa << " wygral!" << endl;
                gracz1.wygrane++;
                komputer.przegrane++;
            }
            else
            {
                cout << "Komputer wygral!" << endl;
                komputer.wygrane++;
                gracz1.przegrane++;
            }
            cout << "Nacisnij Enter, aby wrocic do menu glownego..." << endl;
            getch();
            break;
        }
        if (czy_pelna(plansza))
        {
            cout << "Remis! Plansza jest pelna." << endl;
            gracz1.remisy++;
            komputer.remisy++;
            cout << "Nacisnij Enter, aby wrocic do menu glownego..." << endl;
            getch();
            break;
        }
        aktualny_gracz = (aktualny_gracz == znak_gracza1) ? znak_komputera : znak_gracza1;
    }
    zapisz_statystyki(gracz1);
    zapisz_statystyki(komputer);
}
int wyswietl_menu_glowne()
{
    int pozycja_menu = 1;
    char klawisz;
    while (true)
    {
        system("cls");
        cout << "\033[1;31mKolko i krzyzyk\033[0m" << endl;
        cout << "\033[1;32mMENU GLOWNE\033[0m" << endl;
        for (int i = 1; i <= 3; ++i)
        {
            if (i == pozycja_menu)
            {
                cout << "> ";
            }
            cout << i << ". ";
            switch (i)
            {
            case 1:
                cout << "\033[1;33mGra z innym graczem\033[0m" << endl;
                break;
            case 2:
                cout << "\033[1;33mGra z komputerem\033[0m" << endl;
                break;
            case 3:
                cout << "\033[1;33mWyjdz z gry\033[0m" << endl;
                break;
            }
        }
        klawisz = getch();
        switch (klawisz)
        {
        case 72: //
            pozycja_menu = (pozycja_menu - 1 + 3) % 3 + 1;
            break;
        case 80: //
            pozycja_menu = (pozycja_menu + 1) % 3 + 1;
            break;
        case 13:
            return pozycja_menu;
        }
    }
}
void SetConsoleColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
int main()
{
    while (true)
    {
        int wybor = wyswietl_menu_glowne();
        switch (wybor)
        {
        case 1:
            SetConsoleColor(12);
            gra_z_graczem();
            break;
        case 2:
            SetConsoleColor(10);
            gra_z_komputerem();
            break;
        case 3:
            SetConsoleColor(15);
            cout << "\033[1;31mDziekujemy za gre! Do widzenia\033[0m" << endl;
            return 0;
        default:
            cout << "Nieprawidlowy wybor. Sprobuj ponownie." << endl;
        }
        SetConsoleColor(15);
    }
    return 0;
}
