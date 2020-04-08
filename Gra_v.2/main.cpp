#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <time.h>
#include <string>
#include <fstream>

using namespace std;

fstream zapisgry("zapisygier.txt", ios:: out | ios:: app);
fstream dane("zapisygier.txt", ios::in);
fstream zapiswynik("wyniki.txt", ios:: out | ios:: app);

struct danegracza
{
    int n;
    int x;
    int y;
    int zycia;
    int punkty;
    int trudnosc;
    int przeciwnicy;
    int playercolor;

};

struct monster
{
    bool czyzyje;
    int x;
    int y;
    int zycia;
    int moc;

};

struct przeciwnicy
{
    monster skeleton1;
    monster skeleton2;
    monster skeleton3;
    monster skeleton4;
    monster skeleton5;
    monster skeleton6;
    monster skeleton7;
};

void hidecursor();
void kolory(int k);
void czekaj(int x);
void pauza();
void menu(danegracza &player, przeciwnicy &oponenci);
void instrukcja(danegracza &player, przeciwnicy &oponenci);
void gra(danegracza &player, char plansza[][40], przeciwnicy &oponenci);
void nowagra(danegracza &player, przeciwnicy &oponenci);
void wyswietl(danegracza &player, char plansza[][40], przeciwnicy &oponenci);
void opcje(danegracza &player, przeciwnicy &oponenci);
void menupodczasgry(danegracza &player, char plansza[][40], przeciwnicy &oponenci);
void wyswietlmenupodczasgry(danegracza player, char plansza[][40], int z);
void losujiwstawwmiejsce(danegracza player, char plansza[][40], char znak, int ile /*1-4*/);
void wstaw(danegracza player, char plansza[][40], char znak, int gdziey, int gdziex);
void ruchprzeciwnikow(danegracza &player, char plansza[][40], przeciwnicy &oponenci);
void ruch(przeciwnicy &oponenci, char plansza[0][40], int z, int n);
bool czywpolu(int x, int y, int n);
void ruchnagracza(danegracza &player, przeciwnicy &oponenci, int z, char plansza[][40]);
void walka(danegracza &player, przeciwnicy &oponenci,  char plansza[][40], int z);
void wyswietlwalke(danegracza player, char plansza[][40], int z);
void zmianakoloruzawodnika(danegracza &player, przeciwnicy &oponenci);
void koniecgry(danegracza &player,przeciwnicy &oponenci);
void zapispostepu(danegracza player, char plansza[0][40], przeciwnicy oponenci);
void wczytaj(danegracza &player, przeciwnicy &oponenci);

void menu(danegracza &player, przeciwnicy &oponenci)
{
    system("cls");
    int x = 0;
    int znak;
    string tab[] = {"Nowa gra","Wczytaj poprzedni zapis","Opcje", "Zmiana koloru pionka", "Instrukcja", "Zakoncz" };

    for(int i = 0; i<6; i++)
    {
        if(i == x)
            kolory(2);
        else
            kolory(15);
        cout << tab[i] << endl;
    }

    while(true)
    {
        znak = getch();
        if(znak == 224)
        {
            system("cls");
            znak = getch();
            if(znak == 80 && x<5)
                x++;
            else if(znak==72 && x>0)
                x--;
            for(int i = 0; i<6; i++)
            {
                if(i == x)
                    kolory(2);
                else
                    kolory(15);
                cout << tab[i] << endl;
                kolory(15);
            }
        }
        else if(znak == 13)
        {
            if(x == 0)
                nowagra(player, oponenci);
            else if(x == 1)
            {
                wczytaj(player, oponenci);
            }
            else if(x == 2)
                opcje(player, oponenci);
            else if(x == 3)
                zmianakoloruzawodnika(player, oponenci);
            else if(x == 4)
                instrukcja(player, oponenci);
            else if(x == 5)
            {
                czekaj(1);
                exit(0);
            }
        }
    }
}

void hidecursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = false;
   SetConsoleCursorInfo(consoleHandle, &info);
}

void kolory(int k)
{
    /// 0- czarny
    /// 15 - bialy

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, k);
}

void czekaj(int x)
{
    clock_t a;
    a = clock () + 0.00000000000001 * CLOCKS_PER_SEC ;
    while (clock() < a) {}
}

void pauza(danegracza player, char plansza[][40], przeciwnicy &oponenci)
{
    int znak = getch();
    while(true)
    {
        if(znak == 13 )
        {
            gra(player, plansza, oponenci);
            break;
        }

        else
            znak = getch();
    }
}

void instrukcja(danegracza &player, przeciwnicy &oponenci)
{
    system("cls");
    cout << "Witamy w grze V.2. Jej glownym celem jest zebrac jak najwiecej monet($). Poczatkowo znajduja sie one na mapie startowej," << endl;
    cout << "ale pojawiaja sie takze po otworzeniu niekorych mystercachy(?), ktore, gdy na nie najdziesz otwierasz klawiszem E." << endl;
    cout << "Z mystercachy mozesz sie takze spodziewac kilku nowych mystercachy lub skeletonow.";
    cout << endl << "Ze skeletonami mozesz walczyc. Gdy sie spotkacie pojawi sie pasek obrazen, czym dokladniejszy bedzie cios, tym skeleton otrzyma ";
    cout << "wiecej obrazen, a ty zachowasz wiecej zycia. Sterujesz za pomoca strzalek. Gre mozesz zapauzowac klawiszem ESC." << endl;
    cout << "Gre mozesz zapisac i pozniej ja wczytac i kontynuowac rozgrywke." << endl;
    cout << "Udanej zabawy!" ;
    cout << endl << "Aby zapisac swoj wynik nacisjnij klawisz S. Wynik zapisze sie takze, gdy stracisz wszystkie zycia." << endl << endl << "Nacisnij ENTER, aby wrocic do MENU";
    int znak = getch();
    while(true)
    {
        if(znak == 13)
            menu(player, oponenci);
        else
            znak = getch();
    }
}

void gra(danegracza &player, char plansza[][40], przeciwnicy &oponenci)
{
    kolory(0);
    int n = player.n;
    srand(time(NULL));
    wyswietl(player, plansza, oponenci);
    while(true)
    {
        ruchprzeciwnikow(player, plansza, oponenci);
        if(player.zycia <= 0)
            koniecgry(player, oponenci);


        if(player.x == oponenci.skeleton1.x && player.y == oponenci.skeleton1.y && oponenci.skeleton1.czyzyje)
            walka(player, oponenci, plansza, 1);
        if(player.x == oponenci.skeleton2.x && player.y == oponenci.skeleton2.y && oponenci.skeleton2.czyzyje)
            walka(player, oponenci, plansza, 2);
        if(player.x == oponenci.skeleton3.x && player.y == oponenci.skeleton3.y && oponenci.skeleton3.czyzyje)
            walka(player, oponenci, plansza, 3);
        if(player.x == oponenci.skeleton4.x && player.y == oponenci.skeleton4.y && oponenci.skeleton4.czyzyje)
            walka(player, oponenci, plansza, 4);

        if(player.zycia <= 0)
            koniecgry(player, oponenci);

        wyswietl(player, plansza, oponenci);
        int znak = getch();
        if(znak == 224)
        {
            znak = getch();
            int x1 = player.x;
            int y1 = player.y;
            switch(znak){
                case 72: player.y--;
                        break;
                case 80: player.y++;
                        break;
                case 75: player.x--;
                        break;
                case 77: player.x++;
                        break;
            }
            if(plansza[player.y][player.x] == 's' || player.y >= n || player.x>= 40 || player.x<0 || player.y<0 )
            {

                player.x = x1;
                player.y = y1;
            }
            wyswietl(player, plansza, oponenci);
        }
        else if(znak == 115)
        {
            koniecgry(player, oponenci);
            menu(player, oponenci);
        }
        else if(znak == 27)
        {
           menupodczasgry(player, plansza, oponenci);
        }
        else if(znak == 101)
        {
            if(plansza[player.y][player.x] ==  'k')
            {

                plansza[player.y][player.x] = '0';
                int w = rand()%5;
                if(w%2 == 0)
                {
                    losujiwstawwmiejsce(player, plansza, 'm', 3); /// dodajemy 2 monety
                }
                if(w == 1)
                {
                    losujiwstawwmiejsce(player, plansza, 'k', 2); /// dodajemy skrzynke
                }
                if(w == 3)
                {
                    player.zycia++;
                    if(oponenci.skeleton4.czyzyje == false)
                        oponenci.skeleton4.czyzyje = true;
                    else if(oponenci.skeleton3.czyzyje == false)
                        oponenci.skeleton3.czyzyje = true;
                    else if(oponenci.skeleton2.czyzyje == false)
                        oponenci.skeleton2.czyzyje = true;
                    else if(oponenci.skeleton1.czyzyje == false)
                        oponenci.skeleton1.czyzyje = true;
                }
            }
        }
        if(plansza[player.y][player.x] == 'm')
        {
            plansza[player.y][player.x] = '0';
            player.punkty++;
        }
        if(player.x == oponenci.skeleton1.x && player.y == oponenci.skeleton1.y && oponenci.skeleton1.czyzyje)
            walka(player, oponenci, plansza, 1);
        if(player.x == oponenci.skeleton2.x && player.y == oponenci.skeleton2.y && oponenci.skeleton2.czyzyje)
            walka(player, oponenci, plansza, 2);
        if(player.x == oponenci.skeleton3.x && player.y == oponenci.skeleton3.y && oponenci.skeleton3.czyzyje)
            walka(player, oponenci, plansza, 3);
        if(player.x == oponenci.skeleton4.x && player.y == oponenci.skeleton4.y && oponenci.skeleton4.czyzyje)
            walka(player, oponenci, plansza, 4);

        if(player.zycia <= 0)
            koniecgry(player, oponenci);

    }

}

void nowagra(danegracza &player, przeciwnicy &oponenty)
{
    //player.playercolor = 3;
    player.x = 0;
    player.y = 0;
    player.zycia = 7;
    player.punkty = 0;
    int n = player.n;
    char plansza[n][40];

    if(player.przeciwnicy == 1)
    {
        oponenty.skeleton1.czyzyje = true;
        oponenty.skeleton1.x = 19;
        oponenty.skeleton1.y = n/2;
        oponenty.skeleton1.zycia = 7;
        oponenty.skeleton2.czyzyje = false;
        oponenty.skeleton3.czyzyje = false;
        oponenty.skeleton4.czyzyje = false;
        oponenty.skeleton5.czyzyje = false;
        oponenty.skeleton6.czyzyje = false;
        oponenty.skeleton7.czyzyje = false;
    }
    else if(player.przeciwnicy == 2)
    {
        oponenty.skeleton1.czyzyje = true;
        oponenty.skeleton1.x = 19;
        oponenty.skeleton1.y = 2;
        oponenty.skeleton1.zycia = 7;
        oponenty.skeleton2.czyzyje = true;
        oponenty.skeleton2.x = 19;
        oponenty.skeleton2.y = n-3;
        oponenty.skeleton2.zycia = 7;
        oponenty.skeleton3.czyzyje = false;
        oponenty.skeleton4.czyzyje = false;
        oponenty.skeleton5.czyzyje = false;
        oponenty.skeleton6.czyzyje = false;
        oponenty.skeleton7.czyzyje = false;

    }
    else if(player.przeciwnicy == 3)
    {
        oponenty.skeleton1.czyzyje = true;
        oponenty.skeleton1.x = 37;
        oponenty.skeleton1.y = 1;
        oponenty.skeleton1.zycia = 7;
        oponenty.skeleton2.czyzyje = true;
        oponenty.skeleton2.x = 5;
        oponenty.skeleton2.y = n/2;
        oponenty.skeleton2.zycia = 7;
        oponenty.skeleton3.czyzyje = true;
        oponenty.skeleton3.x = 22;
        oponenty.skeleton3.y = n-2;
        oponenty.skeleton3.zycia = 7;
        oponenty.skeleton4.czyzyje = false;
        oponenty.skeleton5.czyzyje = false;
        oponenty.skeleton6.czyzyje = false;
        oponenty.skeleton7.czyzyje = false;

    }
    else if(player.przeciwnicy == 4)
    {
        oponenty.skeleton1.czyzyje = true;
        oponenty.skeleton1.x = 2;
        oponenty.skeleton1.y = 2;
        oponenty.skeleton1.zycia = 7;
        oponenty.skeleton2.czyzyje = true;
        oponenty.skeleton2.x = 37;
        oponenty.skeleton2.y = n-2;
        oponenty.skeleton2.zycia = 7;
        oponenty.skeleton3.czyzyje = true;
        oponenty.skeleton3.x = 2;
        oponenty.skeleton3.y = n-2;
        oponenty.skeleton3.zycia = 7;
        oponenty.skeleton4.czyzyje = true;
        oponenty.skeleton4.x = 37;
        oponenty.skeleton4.y = 2;
        oponenty.skeleton4.zycia = 7;
        oponenty.skeleton5.czyzyje = false;
        oponenty.skeleton6.czyzyje = false;
        oponenty.skeleton7.czyzyje = false;

    }

    for(int i = 0; i<n; i++)
    {
        for(int j = 0; j<40; j++)
        {
            if(rand()% 33 == 1)
                plansza[i][j] = 's'; /// sciany
            else
                plansza[i][j] = '0';
        }
    }
    for(int i = 0; i<n; i++)
    {
        for(int j = 0; j<40; j++)
        {
            if(rand()% 44 == 2)
                plansza[i][j] = 'm'; /// kesze
        }
    }
    for(int i = 0; i<n; i++)
    {
        for(int j = 0; j<55; j++)
        {
            if(rand()% 66 == 2)
                plansza[i][j] = 'k'; /// monety
        }
    }
    gra(player, plansza, oponenty);

}

void wyswietl(danegracza &player, char plansza[][40], przeciwnicy &oponenci)
{
    system("cls");
    int n = player.n;
    kolory(2);
    cout << (char) 201;
    for(int j = 0; j<40; j++)
    {
        cout << (char) 205;
    }

    cout << (char) 187 << endl;
    for(int i = 0; i<n;i++)
    {
        cout << (char)186;
        for(int j = 0; j<40;j++)
        {
                if(player.x==j && player.y==i)
                {
                    kolory(player.playercolor);
                    cout <<"X";
                    kolory(2);
                }
                else if(plansza[i][j] == 's')
                {
                    kolory(2);
                    cout << (char) 178;
                    kolory(15);
                }
                else if(plansza[i][j] == 'm')
                {
                    kolory(6);
                    cout << "$";
                    kolory(15);
                }
                else if(plansza[i][j] == 'k')
                {
                    kolory(13);
                    cout << char(127);
                    kolory(15);
                }
                else if(plansza[i][j] == 'f')
                {
                    kolory(15);
                    cout << "#";
                    kolory(15);
                }
                else
                {
                    kolory(6);
                    cout << " ";
                    kolory(2);
                }

        }
        kolory(2);
        cout << (char)186;

        cout << endl;

    }
    cout << (char)200;
    for(int j = 0; j<40; j++)
        cout << (char) 205;
    cout << (char) 188 << endl;

    kolory(15);
    cout << "Punkty: " << player.punkty  << endl << "Zycia: " << player.zycia << endl;

}

void opcje(danegracza &player, przeciwnicy &oponenci)
{
    system("cls");
    string opcje[] = {"Rozmiar planszy", "Poziom trudnosci", "Liczba przeciwnikow"};
    int r = player.n/5 , t = player.trudnosc, p = player.przeciwnicy;
    int wartosci[] = {r,t,p};
    int x = 0;
    int y = 0;
    int znak;

    for(int i = 0; i<3; i++)
    {
        if(i == x)
            kolory(6);
        else
            kolory(15);
        cout << opcje[i] << "  ";
        if(i == 0)
        {
            cout << wartosci[i]*5 << endl;
        }
        else
        {
            cout << wartosci[i]-1 << endl;
        }
    }

     while(true)
     {
        znak = getch();
        if(znak == 224)
        {
            system("cls");
            znak = getch();
            if(znak == 80 && x<2)
                x++;
            else if(znak==72 && x>0)
                x--;
            else if(znak == 75 && wartosci[x]>2 )
                wartosci[x]--;
            else if(znak == 77 && wartosci[x]<5)
                wartosci[x]++;
            for(int i = 0; i<3; i++)
            {
                if(i == x)
                    kolory(6);
                else
                    kolory(15);
                cout << opcje[i] << "  ";
                if(i == 0)
                    cout << wartosci[i]*5 << endl;
                else
                    cout << wartosci[i]-1 << endl;
                kolory(15);
            }

        }
        else if(znak == 13)
        {
            player.n = wartosci[0]*5;
            player.trudnosc = wartosci[1]-1;
            player.przeciwnicy = wartosci[2]-1;

            system("cls");
            menu(player, oponenci);
        }
    }

}

void menupodczasgry(danegracza &player, char plansza[][40], przeciwnicy &oponenci)
{
    /// string mozliwosci[] = {"Zapisz i wyjdz", "Wyjdz bez zapisywania", "Wczytaj stara gre", "Kontynuuj"};
    int z = 0;
    wyswietlmenupodczasgry(player, plansza, z);
    int znak;
    while(true)
     {
        znak = getch();
        if(znak == 224)
        {
            system("cls");
            znak = getch();
            if(znak == 80 && z<3)
                z++;
            else if(znak==72 && z>0)
                z--;
            for(int i = 0; i<4; i++)
            {
                if(i == z)
                    kolory(6);
                else
                    kolory(15);
                wyswietlmenupodczasgry(player, plansza, z);
                kolory(15);
            }
        }

    else if(znak == 13)
    {
        if(z == 0)
        {
            zapispostepu(player, plansza, oponenci);
            menu(player, oponenci);
        }
        else if(z == 1)
        {
            menu(player, oponenci);
        }
        else if(z == 2)
        {
            wczytaj(player, oponenci);
        }
        else if(z == 3)
        {
            pauza(player, plansza, oponenci);
        }
    }
    }
}

void wyswietlmenupodczasgry(danegracza player, char plansza[][40], int z)
{
    string mozliwosci[] = {"Zapisz i wyjdz", "Wyjdz bez zapisywania", "Wczytaj stara gre", "Kontynuuj"};

    system("cls");
    int n = player.n;
    kolory(2);
    cout << (char) 201;
    for(int j = 0; j<40; j++)
    {
        cout << (char) 205;
    }

    cout << (char) 187 << endl;
    for(int i = 0; i<n/2-3;i++)
    {
        cout << (char)186;
        for(int j = 0; j<40;j++)
        {
                if(player.x==j && player.y==i)
                {
                    kolory(player.playercolor);
                    cout <<"X";
                    kolory(2);
                }
                else if(plansza[i][j] == 's')
                {
                    kolory(2);
                    cout << (char) 178;
                    kolory(15);
                }
                else if(plansza[i][j] == 'm')
                {
                    kolory(6);
                    cout << "$";
                    kolory(15);
                }
                else if(plansza[i][j] == 'k')
                {
                    kolory(13);
                    cout << (char) 127;
                    kolory(15);
                }
                else if(plansza[i][j] == 'f')
                {
                    kolory(15);
                    cout << "#";
                    kolory(15);
                }
                else
                {
                    kolory(6);
                    cout << " ";
                    kolory(2);
                }
        }
        kolory(2);
        cout << (char)186 << endl;
    }
    kolory(2);
    cout << (char) 186;
/// //////////////////////////// tutaj
    for(int k = 0; k<8; k++)
    {
        if(player.x==k && player.y==(n/2-3))
                {
                    kolory(player.playercolor);
                    cout <<"X";
                    kolory(2);
                }
                else if(plansza[n/2-3][k] == 's')
                {
                    kolory(2);
                    cout << (char) 178;
                    kolory(15);
                }
                else if(plansza[n/2-3][k] == 'm')
                {
                    kolory(6);
                    cout << "$";
                    kolory(15);
                }
                else if(plansza[n/2-3][k] == 'k')
                {
                    kolory(13);
                    cout << (char) 127;
                    kolory(15);
                }
                else if(plansza[n/2-3][k] == 'f')
                {
                    kolory(15);
                    cout << "#";
                    kolory(15);
                }
                else
                {
                    kolory(6);
                    cout << " ";
                    kolory(2);
                }
    }

    kolory(4);
    cout << (char) 201;
    for(int k = 0; k<23; k++)
        cout << (char) 205;
    cout << (char) 187;
/// /////////////////////////////// tutaj
    for(int k = 0; k<7; k++)
    {
        if(player.x==k+33 && player.y==(n/2-3))
                {
                    kolory(player.playercolor);
                    cout <<"X";
                    kolory(2);
                }
                else if(plansza[n/2-3][k+33] == 's')
                {
                    kolory(2);
                    cout << (char) 178;
                    kolory(15);
                }
                else if(plansza[n/2-3][k+33] == 'm')
                {
                    kolory(6);
                    cout << "$";
                    kolory(15);
                }
                else if(plansza[n/2-3][k+33] == 'k')
                {
                    kolory(13);
                    cout << (char) 127;
                    kolory(15);
                }
                else if(plansza[n/2-3][k+33] == 'f')
                {
                    kolory(15);
                    cout << "#";
                    kolory(15);
                }
                else
                {
                    kolory(6);
                    cout << " ";
                    kolory(2);
                }
    }

    kolory(2);
    cout << (char) 186;
    cout <<  endl;
    kolory(2);

    int d,s;

    for(int i = n/2-1;i<n/2+3;i++)
    {
        if(i == n/2-1)
            d = 14;
        else if(i == n/2)
            d = 21;
        else if(i == n/2+1)
            d = 17;
        else if(i == n/2+2)
            d = 9;
        s = (2*n+9-d)/2;
        kolory(2);
        cout << (char) 186;
        kolory(4);
/// ////////////////////////////////////////////////////////////////////////////

        for(int j = 0; j<8; j++)
        {
            if(player.x==j && player.y==i-1)
                {
                    kolory(player.playercolor);
                    cout <<"X";
                    kolory(2);
                }
                else if(plansza[i-1][j] == 's')
                {
                    kolory(2);
                    cout << (char) 178;
                    kolory(15);
                }
                else if(plansza[i-1][j] == 'm')
                {
                    kolory(6);
                    cout << "$";
                    kolory(15);
                }
                else if(plansza[i-1][j] == 'k')
                {
                    kolory(13);
                    cout << (char) 127;
                    kolory(15);
                }
                else if(plansza[i][j] == 'f')
                {
                    kolory(15);
                    cout << "#";
                    kolory(15);
                }
                else
                {
                    kolory(6);
                    cout << " ";
                    kolory(2);
                }
        }

/// ////////////////////////////////////////////////////////////////////////////
        kolory(4);
        cout << (char) 186;
        for(int i = 0; i<s-n+7; i++)
            cout << " ";
        if(z+n/2-1 == i)
            kolory(11);
        else
            kolory(12);
        cout << mozliwosci[i-n/2+1];
        kolory(4);
        if(i == n/2-1)
        {
           for(int j = 0; j<s-n+8; j++)
            cout << " ";
        }
        else
        {
            for(int j = 0; j<s-n+7; j++)
            cout << " ";
        }
        cout << (char) 186;

/// ////////////////////////////////////////////////////////////////////////////
        for(int j = 33; j<40; j++)
        {
            if(player.x==j && player.y==i-1)
                {
                    kolory(player.playercolor);
                    cout <<"X";
                    kolory(2);
                }
                else if(plansza[i-1][j] == 's')
                {
                    kolory(2);
                    cout << (char) 178;
                    kolory(15);
                }
                else if(plansza[i-1][j] == 'm')
                {
                    kolory(6);
                    cout << "$";
                    kolory(15);
                }
                else if(plansza[i-1][j] == 'k')
                {
                    kolory(13);
                    cout << (char) 127;
                    kolory(15);
                }
                else if(plansza[i][j] == 'f')
                {
                    kolory(15);
                    cout << "#";
                    kolory(15);
                }
                else
                {
                    kolory(6);
                    cout << " ";
                    kolory(2);
                }
        }

        kolory(2);
        cout << (char) 186 << endl;
    }

    kolory(2);
    cout << (char) 186;
/// ////////////////////////// tutaj
    for(int k = 0; k<8; k++)
    {
        if(player.x==k && player.y==(n/2+2))
                {
                    kolory(player.playercolor);
                    cout <<"X";
                    kolory(2);
                }
                else if(plansza[n/2+2][k] == 's')
                {
                    kolory(2);
                    cout << (char) 178;
                    kolory(15);
                }
                else if(plansza[n/2+2][k] == 'm')
                {
                    kolory(6);
                    cout << "$";
                    kolory(15);
                }
                else if(plansza[n/2+2][k] == 'k')
                {
                    kolory(13);
                    cout << (char) 127;
                    kolory(15);
                }
                else if(plansza[n/2+2][k] == 'f')
                {
                    kolory(15);
                    cout << "#";
                    kolory(15);
                }
                else
                {
                    kolory(6);
                    cout << " ";
                    kolory(2);
                }
    }

    kolory(4);
    cout << (char) 200;
    for(int k = 0; k<23; k++)
        cout << (char) 205;
    cout << (char) 188;
/// //////////////////////////// tutaj
    for(int k = 0; k<7; k++)
    {
        if(player.x==k+34 && player.y==(n/2+2))
                {
                    kolory(player.playercolor);
                    cout <<"X";
                    kolory(2);
                }
                else if(plansza[n/2+2][k+33] == 's')
                {
                    kolory(2);
                    cout << (char) 178;
                    kolory(15);
                }
                else if(plansza[n/2+2][k+33] == 'm')
                {
                    kolory(6);
                    cout << "$";
                    kolory(15);
                }
                else if(plansza[n/2+2][k+33] == 'k')
                {
                    kolory(13);
                    cout << (char) 127;
                    kolory(15);
                }
                else if(plansza[n/2+2][k+33] == 'f')
                {
                    kolory(15);
                    cout << "#";
                    kolory(15);
                }
                else
                {
                    kolory(6);
                    cout << " ";
                    kolory(2);
                }
    }
    kolory(2);
    cout << (char) 186;
    cout << endl;
    kolory(2);

    kolory(2);
    for(int i = n/2+3; i<n;i++)
    {
        cout << (char)186;
        for(int j = 0; j<40;j++)
        {
                if(player.x==j && player.y==i)
                {
                    kolory(player.playercolor);
                    cout <<"X";
                    kolory(2);
                }
                else if(plansza[i][j] == 's')
                {
                    kolory(2);
                    cout << (char) 178;
                    kolory(15);
                }
                else if(plansza[i][j] == 'm')
                {
                    kolory(6);
                    cout << "$";
                    kolory(15);
                }
                else if(plansza[i][j] == 'k')
                {
                    kolory(13);
                    cout << (char) 127;
                    kolory(15);
                }
                else if(plansza[i][j] == 'f')
                {
                    kolory(15);
                    cout << "#";
                    kolory(15);
                }
                else
                {
                    kolory(6);
                    cout << " ";
                    kolory(2);
                }

        }
        kolory(2);
        cout << (char)186;

        cout << endl;
    }
    cout << (char)200;
    for(int j = 0; j<40; j++)
        cout << (char) 205;
    cout << (char) 188 << endl;

    kolory(15);
    cout << "Punkty: " << player.punkty  << endl << "Zycia: " << player.zycia << endl;
}

void losujiwstawwmiejsce(danegracza player, char plansza[][40], char znak, int ile/*1-4*/)
{
    for(int i = 0; i<ile; i++)
    {
        int p = rand()%4, o = (rand()%4)+1;
        if(p == 0) /// na polnoc
        {
            wstaw(player, plansza, znak, o, 0);
        }
        else if(p == 1) /// na poludnie
        {
            wstaw(player, plansza, znak, o*(-1), 0);
        }
        else if(p == 2) /// na wschod
        {
            wstaw(player, plansza, znak, 0, o);
        }
        else
        {
            wstaw(player, plansza, znak, 0, o*(-1));
        }
    }

}

void wstaw(danegracza player, char plansza[][40], char znak, int gdziey, int gdziex)
{
    plansza[player.y+gdziey][player.x+gdziex] = znak;
}

void ruchprzeciwnikow(danegracza &player, char plansza[][40], przeciwnicy &oponenci)
{
    if(abs(player.x-oponenci.skeleton1.x)<=4 && abs(player.y-oponenci.skeleton1.y)<=4)
    {
        if(oponenci.skeleton1.czyzyje)
            ruchnagracza(player, oponenci, 1, plansza);
    }
    else
    {
        if(oponenci.skeleton1.czyzyje)
            ruch(oponenci, plansza, 1, player.n);
    }
///////////////////////////////////////
    if(abs(player.x-oponenci.skeleton2.x)<=4 && abs(player.y-oponenci.skeleton2.y)<=4)
    {
        if(oponenci.skeleton2.czyzyje)
            ruchnagracza(player, oponenci, 2, plansza);
    }
    else
    {
        if(oponenci.skeleton2.czyzyje)
            ruch(oponenci, plansza, 2, player.n);
    }
///////////////////////////////////////
    if(abs(player.x-oponenci.skeleton3.x)<=4 && abs(player.y-oponenci.skeleton3.y)<=4)
    {
        if(oponenci.skeleton3.czyzyje)
            ruchnagracza(player, oponenci, 3, plansza);
    }
    else
    {
        if(oponenci.skeleton3.czyzyje)
            ruch(oponenci, plansza, 3, player.n);
    }
///////////////////////////////////////
    if(abs(player.x-oponenci.skeleton4.x)<=4 && abs(player.y-oponenci.skeleton4.y)<=4)
    {
        if(oponenci.skeleton4.czyzyje)
            ruchnagracza(player, oponenci, 4, plansza);
    }
    else
    {
        if(oponenci.skeleton4.czyzyje)
            ruch(oponenci, plansza, 4, player.n);
    }
///////////////////////////////////////


    if(oponenci.skeleton1.czyzyje)
        plansza[oponenci.skeleton1.y][oponenci.skeleton1.x] = 'f';
    if(oponenci.skeleton2.czyzyje)
        plansza[oponenci.skeleton2.y][oponenci.skeleton2.x] = 'f';
    if(oponenci.skeleton3.czyzyje)
        plansza[oponenci.skeleton3.y][oponenci.skeleton3.x] = 'f';
    if(oponenci.skeleton4.czyzyje)
        plansza[oponenci.skeleton4.y][oponenci.skeleton4.x] = 'f';

}

bool czywpolu(int x, int y, int n)
{
    if(x>=0 && x<n && y>=0 && y<40)
        return true;

    return false;
}

void ruch(przeciwnicy &oponenci, char plansza[0][40], int z, int n)
{
    int x, y, k = rand()%4;

    switch(k)
    {
    case 0:
        x = 0;
        y = 1;
        break;
    case 1:
        x = 0;
        y = -1;
        break;
    case 2:
        x = 1;
        y = 0;
        break;
    case 3:
        x = -1;
        y = 0;
        break;
    }
    if(z == 1)
    {
        if(plansza[oponenci.skeleton1.y+x][oponenci.skeleton1.x+y] != 's'  &&  czywpolu(oponenci.skeleton1.y+x, oponenci.skeleton1.x+y, n ))
        {
            plansza[oponenci.skeleton1.y][oponenci.skeleton1.x] = '0';
            oponenci.skeleton1.x += y;
            oponenci.skeleton1.y += x;
        }
        else
            ruch(oponenci, plansza, 1, n);
    }
    else if(z == 2)
    {
        if(plansza[oponenci.skeleton2.y+x][oponenci.skeleton2.x+y] != 's'  &&  czywpolu(oponenci.skeleton2.y+x, oponenci.skeleton2.x+y, n ))
        {
            plansza[oponenci.skeleton2.y][oponenci.skeleton2.x] = '0';
            oponenci.skeleton2.x += y;
            oponenci.skeleton2.y += x;
        }
        else
            ruch(oponenci, plansza, 2, n);
    }
    if(z == 3)
    {
        if(plansza[oponenci.skeleton3.y+x][oponenci.skeleton3.x+y] != 's'  &&  czywpolu(oponenci.skeleton3.y+x, oponenci.skeleton3.x+y, n ))
        {
            plansza[oponenci.skeleton3.y][oponenci.skeleton3.x] = '0';
            oponenci.skeleton3.x += y;
            oponenci.skeleton3.y += x;
        }
        else
            ruch(oponenci, plansza, 3, n);
    }
    else if(z == 4)
    {
        if(plansza[oponenci.skeleton4.y+x][oponenci.skeleton4.x+y] != 's'  &&  czywpolu(oponenci.skeleton4.y+x, oponenci.skeleton4.x+y, n ))
        {
            plansza[oponenci.skeleton4.y][oponenci.skeleton4.x] = '0';
            oponenci.skeleton4.x += y;
            oponenci.skeleton4.y += x;
        }
        else
            ruch(oponenci, plansza, 4, n);
    }



}

void ruchnagracza(danegracza &player, przeciwnicy &oponenci, int z, char plansza[][40])
{
    if(z == 1)
    {
        plansza[oponenci.skeleton1.y][oponenci.skeleton1.x] = '0';
        int k = rand()%2;

        if(player.y-oponenci.skeleton1.y > 0) /// do gory
        {
            if(plansza[oponenci.skeleton1.y+1][oponenci.skeleton1.x] != 's' && k == 1)
            {
                oponenci.skeleton1.y++;
                k = 3;
            }
        }
        else if(player.y - oponenci.skeleton1.y < 0) /// w dol
        {
            if(plansza[oponenci.skeleton1.y-1][oponenci.skeleton1.x] != 's' && k == 1)
            {
                oponenci.skeleton1.y--;
                k = 3;
            }

        }

        if(player.x-oponenci.skeleton1.x > 0) /// w prawo
        {
            if(plansza[oponenci.skeleton1.y][oponenci.skeleton1.x+1] != 's' && k == 0)
                oponenci.skeleton1.x++;
        }
        else if(player.x - oponenci.skeleton1.x < 0) /// w lewo
        {
            if(plansza[oponenci.skeleton1.y][oponenci.skeleton1.x-1] != 's' && k == 0)
                oponenci.skeleton1.x--;
        }
    }
    else if(z == 2)
    {
        plansza[oponenci.skeleton2.y][oponenci.skeleton2.x] = '0';
        int k = rand()%2;

        if(player.y-oponenci.skeleton2.y > 0) /// do gory
        {
            if(plansza[oponenci.skeleton2.y+1][oponenci.skeleton2.x] != 's' && k == 1)
            {
                oponenci.skeleton2.y++;
                k = 3;
            }
        }
        else if(player.y - oponenci.skeleton2.y < 0) /// w dol
        {
            if(plansza[oponenci.skeleton2.y-1][oponenci.skeleton2.x] != 's' && k == 1)
            {
                oponenci.skeleton2.y--;
                k = 3;
            }

        }

        if(player.x-oponenci.skeleton2.x > 0) /// w prawo
        {
            if(plansza[oponenci.skeleton2.y][oponenci.skeleton2.x+1] != 's' && k == 0)
                oponenci.skeleton2.x++;
        }
        else if(player.x - oponenci.skeleton2.x < 0) /// w lewo
        {
            if(plansza[oponenci.skeleton2.y][oponenci.skeleton2.x-1] != 's' && k == 0)
                oponenci.skeleton2.x--;
        }
    }
    else if(z == 3)
    {
        plansza[oponenci.skeleton3.y][oponenci.skeleton3.x] = '0';
        int k = rand()%2;

        if(player.y-oponenci.skeleton3.y > 0) /// do gory
        {
            if(plansza[oponenci.skeleton3.y+1][oponenci.skeleton3.x] != 's' && k == 1)
            {
                oponenci.skeleton3.y++;
                k = 3;
            }
        }
        else if(player.y - oponenci.skeleton3.y < 0) /// w dol
        {
            if(plansza[oponenci.skeleton3.y-1][oponenci.skeleton3.x] != 's' && k == 1)
            {
                oponenci.skeleton3.y--;
                k = 3;
            }

        }

        if(player.x-oponenci.skeleton3.x > 0) /// w prawo
        {
            if(plansza[oponenci.skeleton3.y][oponenci.skeleton3.x+1] != 's' && k == 0)
                oponenci.skeleton3.x++;
        }
        else if(player.x - oponenci.skeleton3.x < 0) /// w lewo
        {
            if(plansza[oponenci.skeleton3.y][oponenci.skeleton3.x-1] != 's' && k == 0)
                oponenci.skeleton3.x--;
        }
    }
    else if(z == 4)
    {
        plansza[oponenci.skeleton4.y][oponenci.skeleton4.x] = '0';
        int k = rand()%2;

        if(player.y-oponenci.skeleton4.y > 0) /// do gory
        {
            if(plansza[oponenci.skeleton4.y+1][oponenci.skeleton4.x] != 's' && k == 1)
            {
                oponenci.skeleton4.y++;
                k = 3;
            }
        }
        else if(player.y - oponenci.skeleton4.y < 0) /// w dol
        {
            if(plansza[oponenci.skeleton4.y-1][oponenci.skeleton4.x] != 's' && k == 1)
            {
                oponenci.skeleton4.y--;
                k = 3;
            }

        }

        if(player.x-oponenci.skeleton4.x > 0) /// w prawo
        {
            if(plansza[oponenci.skeleton4.y][oponenci.skeleton4.x+1] != 's' && k == 0)
                oponenci.skeleton4.x++;
        }
        else if(player.x - oponenci.skeleton4.x < 0) /// w lewo
        {
            if(plansza[oponenci.skeleton4.y][oponenci.skeleton4.x-1] != 's' && k == 0)
                oponenci.skeleton4.x--;
        }
    }
}

void walka(danegracza &player, przeciwnicy &oponenci, char plansza[][40], int z)
{
    if(z == 1)
    {
    int kolor[] = {4, 4, 4, 6, 6, 2, 10, 2, 6, 6, 4, 4, 4};
    int k = 0;
    int obieg = 0;
    int znak = 88;
    while(true)
    {
       /* system("cls");
        for(int i = 0; i<13; i++)
        { //219-177
            kolory(kolor[i]);
            if(i==k)
                cout << (char) 219;
            else
                cout << (char) 177;
        }*/
        wyswietlwalke(player, plansza, k);
        czekaj(player.trudnosc);
        if(k == 12)
            obieg = 1;
        else if(k == 0)
            obieg = 0;
        if(obieg == 0)
            k++;
        else
            k--;
        if(kbhit())
        {
            znak = getch();
            if(znak == 13)
            {
                plansza[oponenci.skeleton1.y][oponenci.skeleton1.x] = '0';
                oponenci.skeleton1.y +=4;
                if(k == 5 || k == 8)
                {
                    oponenci.skeleton1.czyzyje = false;
                    player.punkty += 5;
                    player.zycia -= 1;
                    break;
                }
                else
                {
                    player.zycia -= 3;
                    break;
                }
                if(player.zycia<0)
                    player.zycia = 0;
            }

        }


    }
    }
    else if(z == 2)
    {
    int kolor[] = {4, 4, 4, 6, 6, 2, 10, 2, 6, 6, 4, 4, 4};
    int k = 0;
    int obieg = 0;
    int znak = 88;
    while(true)
    {
        system("cls");
        for(int i = 0; i<13; i++)
        { //219-177
            kolory(kolor[i]);
            if(i==k)
                cout << (char) 219;
            else
                cout << (char) 177;
        }
        czekaj(player.trudnosc);
        if(k == 12)
            obieg = 1;
        else if(k == 0)
            obieg = 0;
        if(obieg == 0)
            k++;
        else
            k--;
        if(kbhit())
        {
            znak = getch();
            if(znak == 13)
            {
                plansza[oponenci.skeleton2.y][oponenci.skeleton2.x] = '0';
                oponenci.skeleton2.y +=4;
                if(k == 5 || k == 8)
                {
                    oponenci.skeleton2.czyzyje = false;
                    player.punkty += 5;
                    player.zycia -= 1;
                    break;
                }
                else
                {
                    player.zycia -= 3;
                    break;
                }
                if(player.zycia<0)
                    player.zycia = 0;
            }

        }


    }
    }
    else if(z == 3)
    {
    int kolor[] = {4, 4, 4, 6, 6, 2, 10, 2, 6, 6, 4, 4, 4};
    int k = 0;
    int obieg = 0;
    int znak = 88;
    while(true)
    {
        system("cls");
        for(int i = 0; i<13; i++)
        { //219-177
            kolory(kolor[i]);
            if(i==k)
                cout << (char) 219;
            else
                cout << (char) 177;
        }
        czekaj(player.trudnosc);
        if(k == 12)
            obieg = 1;
        else if(k == 0)
            obieg = 0;
        if(obieg == 0)
            k++;
        else
            k--;
        if(kbhit())
        {
            znak = getch();
            if(znak == 13)
            {
                plansza[oponenci.skeleton3.y][oponenci.skeleton3.x] = '0';
                oponenci.skeleton3.y +=4;
                if(k == 5 || k == 8)
                {
                    oponenci.skeleton3.czyzyje = false;
                    player.punkty += 5;
                    player.zycia -= 1;
                    break;
                }
                else
                {
                    player.zycia -= 3;
                    break;
                }
                if(player.zycia<0)
                    player.zycia = 0;
            }

        }


    }
    }
    else if(z == 4)
    {
    int kolor[] = {4, 4, 4, 6, 6, 2, 10, 2, 6, 6, 4, 4, 4};
    int k = 0;
    int obieg = 0;
    int znak = 88;
    while(true)
    {
        system("cls");
        for(int i = 0; i<13; i++)
        { //219-177
            kolory(kolor[i]);
            if(i==k)
                cout << (char) 219;
            else
                cout << (char) 177;
        }
        czekaj(player.trudnosc);
        if(k == 12)
            obieg = 1;
        else if(k == 0)
            obieg = 0;
        if(obieg == 0)
            k++;
        else
            k--;
        if(kbhit())
        {
            znak = getch();
            if(znak == 13)
            {
                plansza[oponenci.skeleton4.y][oponenci.skeleton4.x] = '0';
                oponenci.skeleton4.y +=4;
                if(k == 5 || k == 8)
                {
                    oponenci.skeleton4.czyzyje = false;
                    player.punkty += 5;
                    player.zycia -= 1;
                    break;
                }
                else
                {
                    player.zycia -= 3;
                    break;
                }
                if(player.zycia<0)
                    player.zycia = 0;
            }

        }


    }
    }
}

void wyswietlwalke(danegracza player, char plansza[][40], int k)
{

    system("cls");
    cout << endl << endl << endl << "                 Aby zaatakowac nacisnij ENTER" << endl << endl << endl << endl;
    int kolor[] = {4, 4, 4, 6, 6, 2, 10, 2, 6, 6, 4, 4, 4};
    cout << "                       ";
    for(int i = 0; i<13; i++)
    { //219-177
        kolory(kolor[i]);
        if(i==k)
            cout << (char) 219;
        else
            cout << (char) 177;
    }

}

void zmianakoloruzawodnika(danegracza &player, przeciwnicy &oponenci)
{
    int znak;
    int x = 1;
    system("cls");
    kolory(15);
    cout << "Zmieniaj kolor wciskajac strzalke w lewo lub prawo! Zakceptuj ENTER-em" << endl << endl;
    for(int i = 0; i<37; i++)
        cout << " ";
    kolory(x);
    cout << "X";

    while(true)
    {
        znak = getch();
        if(znak == 224)
        {
            system("cls");
            znak = getch();
            if(znak == 77 && x<15)
                x++;
            else if(znak == 75 && x>1)
                x--;
        }
        else if(znak == 13)
        {
            player.playercolor = x;
            menu(player, oponenci);
            cout << player.playercolor;
            break;
        }
        system("cls");
        kolory(15);
        cout << "Zmieniaj kolor wciskajac strzalke w lewo lub prawo! Zakceptuj ENTER-em" << endl << endl;
        for(int i = 0; i<37; i++)
            cout << " ";
        kolory(x);
        cout << "X";
    }
}

void koniecgry(danegracza &player,przeciwnicy &oponenci)
{
    string nick;
    system("cls");
    kolory(15);
    cout << "Straciles wszystkie zycia. Skelotony pokonaly Cie. Zdobyles " << player.punkty << " punktow.";
    cout << endl << "Podaj nick pod jakim zapisac Cie w tablicy wynikow: ";
    cin >> nick;
    zapiswynik << nick;
    zapiswynik << " " << player.punkty << endl;
    menu(player, oponenci);
}

void zapispostepu(danegracza player, char plansza[0][40], przeciwnicy oponenci)
{
    system("cls");
    string nick;
    cout << "Podaj nick: ";
    cin >> nick;
    zapisgry << nick << " " << player.n << " " << player.playercolor << " " << player.przeciwnicy << " " << player.punkty << " " << player.trudnosc << " " << player.x << " " << player.y << " " << player.zycia << " ";
    zapisgry << oponenci.skeleton1.czyzyje << " " << oponenci.skeleton1.x << " " << oponenci.skeleton1.y << " " << oponenci.skeleton2.czyzyje << " " << oponenci.skeleton2.x << " " << oponenci.skeleton2.y << " " << oponenci.skeleton3.czyzyje << " " << oponenci.skeleton3.x << " " << oponenci.skeleton3.y << " " << oponenci.skeleton4.czyzyje << " " << oponenci.skeleton4.x << " " << oponenci.skeleton4.y << " ";
    for(int i = 0; i<player.n; i++)
    {
        for(int j = 0; j<40; j++)
        {
            zapisgry << plansza[i][j] << " ";
        }
    }
    zapisgry << endl << endl;
}

void wczytaj(danegracza &player, przeciwnicy &oponenci)
{
    system("cls");
    string nick;
    cout << "Podaj nick z jakiego chcesz wczytac gre: ";
    cin >> nick;

    int k = 0;
    string dana = "jjj";
    dane.seekg( 0 );
    dane >> dana;
    while(dana != nick)
    {
        dane >> dana;
    }
        dane >> player.n;
        dane >> player.playercolor;
        dane >> player.przeciwnicy;
        dane >> player.punkty;
        dane >> player.trudnosc;
        dane >> player.x;
        dane >> player.y;
        dane >> player.zycia;

        dane >> oponenci.skeleton1.czyzyje;
        dane >> oponenci.skeleton1.x;
        dane >> oponenci.skeleton1.y;

        dane >> oponenci.skeleton2.czyzyje;
        dane >> oponenci.skeleton2.x;
        dane >> oponenci.skeleton2.y;

        dane >> oponenci.skeleton3.czyzyje;
        dane >> oponenci.skeleton3.x;
        dane >> oponenci.skeleton3.y;

        dane >> oponenci.skeleton4.czyzyje;
        dane >> oponenci.skeleton4.x;
        dane >> oponenci.skeleton4.y;

        int n = player.n;
        char plansza[n][40];

        for(int i = 0; i<n; i++)
        {
            for(int j = 0; j<40; j++)
                dane >> plansza[i][j];
        }

        gra(player, plansza, oponenci);

    }



int main()
{
    hidecursor();
    danegracza player1;
    przeciwnicy oponenci;
    player1.n = 15;
    player1.playercolor = 3;
    player1.trudnosc = 2;
    player1.przeciwnicy = 2;
    menu(player1, oponenci);

    return 0;
}
