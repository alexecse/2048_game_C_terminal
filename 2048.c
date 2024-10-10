#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// declararea culorilor aditionale
#define COLOR_CHARCOAL 10
#define COLOR_PGREEN 11
#define COLOR_SAFFRON 12
#define COLOR_SANDY 13
#define COLOR_SIENNA 14
#define COLOR_2 15
#define COLOR_4 16
#define COLOR_8 17
#define COLOR_16 18
#define COLOR_32 19
#define COLOR_64 20
#define COLOR_128 21
#define COLOR_256 22
#define COLOR_512 23
#define COLOR_1024 24
#define COLOR_2048 25
#define COLOR_MIDGREEN 26

int setari_colorare();
int display_menu(int *hasResume);
int new_game(int board[6][6], int *hasResume);
int resume_game(int board[6][6], int *hasResume);
void design_board(WINDOW *game);
void copy_matrix(int where[6][6], int what[6][6]);
int equal_matrix(int A[6][6], int B[6][6]);
void reset(int board[6][6]);
void bordare(int board[6][6], int blocker);
void fill_block(WINDOW *game, int nr, int x, int y);
void randomStart(int *r, int *c, int *content);
void addNew(int board[6][6]);
int full_board(int board[6][6]);
void rotate_right(int board[6][6]);
int swipe_up(int board[6][6]);
int swipe_down(int board[6][6]);
int swipe_left(int board[6][6]);
int swipe_right(int board[6][6]);
void DateTimePoints(WINDOW *game, int points);
void update(WINDOW *game, int board[6][6], int Crow, int CCol);
int autoMove(int board[6][6]);
int isWinner(int board[6][6]);
void theEnd();
void winner();
void pisoias();

int main()
{
    // pentru a primi valori random de cate ori e nevoie
    srand((unsigned int)time(NULL));
    int board[6][6], blocker;

    reset(board);
    blocker = 1;
    bordare(board, blocker);

    int hasResume, optiune;

    // INCEPEM SESIUNEA NCURSES
    initscr();

    // initiem culoarea
    start_color();
    // modul pentru a inregistra sageti
    keypad(stdscr, true);
    curs_set(0);

    // daca nu putem colora, eroare
    if (setari_colorare())
    {
        endwin();
        return 1;
    }

    // nu avem joc inceput
    hasResume = 0;

    // afisare meniu
    optiune = display_menu(&hasResume);
    while (true)
    {
        // NEW GAME
        if (optiune == 1)
        {
            // resetam tabela
            reset(board);
            bordare(board, 1);

            if (new_game(board, &hasResume) == 3)
                optiune = display_menu(&hasResume);
        }
        // RESUME
        else if (optiune == 2 && hasResume)
        {
            resume_game(board, &hasResume);
            optiune = display_menu(&hasResume);
        }
        // QUIT
        else if (optiune == 3)
            break;
    }

    // termina sesiunea ncurses
    endwin();
    return 0;
}

// functie care copiaza o matrice
void copy_matrix(int where[6][6], int what[6][6])
{
    int i, j;
    for (i = 0; i < 6; i++)
        for (j = 0; j < 6; j++)
            where[i][j] = what[i][j];
}

// functie care verifica daca o matrice este egala cu alta
int equal_matrix(int A[6][6], int B[6][6])
{
    int i, j;
    for (i = 0; i < 6; i++)
        for (j = 0; j < 6; j++)
            if (A[i][j] != B[i][j])
                return 0;
    return 1;
}

// functie care reseteaza tabela
void reset(int board[6][6])
{
    int i, j;
    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 6; j++)
        {
            board[i][j] = 0;
        }
    }
}

// functie care bordeaza tabela
void bordare(int board[6][6], int blocker)
{
    int i, N;
    N = 4;
    // bordam cu blocker, numar care nu apare in joc;
    for (i = 0; i <= N + 1; i++)
    {
        board[N + 1][i] = board[i][N + 1] = blocker;
        board[0][i] = board[i][0] = blocker;
    }

    // pt points, in elementul 0,0 salvam numarul de puncte
    board[0][0] = 0;
}

// functie care da cele doua valori intiae
void randomStart(int *r, int *c, int *content)
{
    *r = rand() % 4 + 1;
    *c = rand() % 4 + 1;
    // daca e par atribuie 2, daca e impar atribuie 4
    *content = rand() % 2 == 0 ? 2 : 4;
}

// functie care returneaza daca tabla este plina de elemente
int full_board(int board[6][6])
{
    int i, j, nr = 0;
    for (i = 1; i <= 4; i++)
        for (j = 1; j <= 4; j++)
            if (board[i][j] != 0)
                nr++;
    return nr;
}

// functie care adauga un nou element
void addNew(int board[6][6])
{
    int i, j, nr;
    // genereaza numar pana gasesti unde sa il pui
    do
    {
        randomStart(&i, &j, &nr);
    } while (board[i][j] != 0);
    board[i][j] = nr;
}

// functie de autoplay
int autoMove(int board[6][6])
{
    int Max = 0, swipe = 0, dif;
    int aux[6][6], changes;
    copy_matrix(aux, board);

    // verifii care miscare e cea mai buna
    swipe_up(board);
    dif = full_board(aux) - full_board(board);
    if (dif > Max)
    {
        Max = dif;
        swipe = 1;
    }
    copy_matrix(board, aux);

    swipe_left(board);
    dif = full_board(aux) - full_board(board);
    if (dif > Max)
    {
        Max = dif;
        swipe = 2;
    }
    copy_matrix(board, aux);

    swipe_down(board);
    dif = full_board(aux) - full_board(board);
    if (dif > Max)
    {
        Max = dif;
        swipe = 3;
    }
    copy_matrix(board, aux);

    swipe_right(board);
    dif = full_board(aux) - full_board(board);
    if (dif > Max)
    {
        Max = dif;
        swipe = 2;
    }
    copy_matrix(board, aux);

    // daca nu a fost nicio mutare buna, fa una random
    if (swipe == 0)
    {
        swipe = rand() % 4 + 1;
    }

    if (swipe == 1)
        changes = swipe_up(board);
    else if (swipe == 2)
        changes = swipe_left(board);
    else if (swipe == 3)
        changes = swipe_down(board);
    else if (swipe == 4)
        changes = swipe_right(board);
    return changes;
}

// functie pentru sageata sus
int swipe_up(int board[6][6])
{
    int i, j, poz, tmp;
    int aux[6][6], *points;

    points = &board[0][0];
    copy_matrix(aux, board);

    // fiecare coloana
    for (j = 1; j <= 4; j++)
    {
        // pt fiecare element, incepand cu primul
        for (i = 1; i <= 4; i++)
        {
            // gasit nr
            if (board[i][j] != 0)
            {
                // cauta pereche
                poz = i + 1;
                while (board[poz][j] == 0 && poz <= 4)
                    poz++;
                // am gasit pereche
                if (poz <= 4 && board[poz][j] == board[i][j])
                {
                    // pune-le impreuna
                    board[i][j] *= 2;
                    *points += board[i][j];
                    board[poz][j] = 0;
                    tmp = board[i][j];
                }
                else
                    tmp = board[i][j];

                // translatam in sus
                board[i][j] = 0;
                poz = 1;
                for (poz = 1; poz <= 4; poz++)
                    if (board[poz][j] == 0)
                    {
                        board[poz][j] = tmp;
                        break;
                    }
            }
        }
    }
    // daca nu s-a schimbat nimic, retunrneaza 0
    if (equal_matrix(aux, board))
        return 0;
    return 1;
}

// functie pentru rotatie in sens ceasornic
void rotate_right(int board[6][6])
{
    int i, j, N, aux;
    N = 4;
    for (i = 1; i <= N / 2; i++)
    {
        for (j = i; j <= N - i; j++)
        {
            aux = board[i][j];
            board[i][j] = board[j][N - i + 1];
            board[j][N - i + 1] = board[N - i + 1][N - j + 1];
            board[N - i + 1][N - j + 1] = board[N - j + 1][i];
            board[N - j + 1][i] = aux;
        }
    }
}

// functie pentru sageata jos
int swipe_down(int board[6][6])
{
    // rotim de doua ori spre dreapta, swipe up si apoi iar 2 rotiri
    int changes;
    rotate_right(board);
    rotate_right(board);
    changes = swipe_up(board);
    rotate_right(board);
    rotate_right(board);
    return changes;
}

// functie pentru sageata dreapta
int swipe_right(int board[6][6])
{
    // rotim o data dreapta, swipe up si apoi 3 rotiri spre dreapta
    int changes;
    rotate_right(board);
    changes = swipe_up(board);
    rotate_right(board);
    rotate_right(board);
    rotate_right(board);
    return changes;
}

// functie pentru sageata stanga
int swipe_left(int board[6][6])
{
    // rotim de 3 ori, swipe up si apoi mai rotim o data
    int changes;
    rotate_right(board);
    rotate_right(board);
    rotate_right(board);
    changes = swipe_up(board);
    rotate_right(board);
    return changes;
}

// functie care produce interfata de joc 2048
void design_board(WINDOW *game)
{
    int row, col, startR, startC;
    getmaxyx(game, row, col);
    wbkgd(game, COLOR_PAIR(1));

    // chenar in jurul ferestrei
    box(game, 0, 0);

    // celule repetitive
    char border[] = "+--------------+--------------+--------------+--------------+";
    char filler[] = "|              |              |              |              |";

    startR = 6, startC = 8;
    wattron(game, COLOR_PAIR(2) | A_BOLD);
    mvwprintw(game, startR, startC, "+-----------------------------------+");
    startR++;
    mvwprintw(game, startR, startC, "|                                   |");
    startR++;
    mvwprintw(game, startR, startC, "| * Date:                           |");
    startR++;
    mvwprintw(game, startR, startC, "|   Time:                           |");
    startR++;
    mvwprintw(game, startR, startC, "|                                   |");
    startR++;
    mvwprintw(game, startR, startC, "| * Points:                         |");
    startR++;
    mvwprintw(game, startR, startC, "|                                   |");
    startR++;
    mvwprintw(game, startR, startC, "| * Use arrows to play              |");
    startR++;
    mvwprintw(game, startR, startC, "|   Press [q] to return to menu     |");
    startR++;
    mvwprintw(game, startR, startC, "|                                   |");
    startR++;
    mvwprintw(game, startR, startC, "+-----------------------------------+");
    wrefresh(game);

    startC = col / 2 - strlen(border) / 2;

    // Desenam tabelul pt joc
    int cnt;
    for (cnt = 0; cnt <= 28; cnt++)
    {
        startR = row / 2 + cnt - 14;
        if (cnt % 7 == 0)
            mvwprintw(game, startR, startC, "%s", border);
        else
            mvwprintw(game, startR, startC, "%s", filler);
    }

    wattroff(game, COLOR_PAIR(2) | A_BOLD);
    wrefresh(game);
}

// functie care actualizeaza data, timpul si punctele
void DateTimePoints(WINDOW *game, int points)
{
    time_t timpul;
    struct tm *tInfo;
    time(&timpul);
    tInfo = localtime(&timpul);

    wattron(game, COLOR_PAIR(2) | A_ITALIC);
    mvwprintw(game, 8, 18, "%d-%d-%d", tInfo->tm_mday, tInfo->tm_mon + 1, tInfo->tm_year + 1900);
    mvwprintw(game, 9, 18, "%02d:%02d:%02d", tInfo->tm_hour, tInfo->tm_min, tInfo->tm_sec);
    mvwprintw(game, 11, 20, "%d", points);
    wattroff(game, COLOR_PAIR(2) | A_ITALIC);
}

// functie care updateaza datele din tabela
void update(WINDOW *game, int board[6][6], int row_center, int col_center)
{
    int i, j, points;
    // vectorii drow si dcol contin variatiile coordonatelor fata de centru
    // ale coltului stanga sus al patratului respectiv
    int drow[5] = {0, -13, -6, +1, +8};
    int dcol[5] = {0, -29, -14, +1, +16};
    // punctajul tabelei curente
    points = board[0][0];

    wclear(game);
    wrefresh(game);

    design_board(game);

    DateTimePoints(game, points);

    for (i = 1; i <= 4; i++)
    {
        for (j = 1; j <= 4; j++)
        {
            // unde gasim element, dam cu fill cu text art-ul asociat
            fill_block(game, board[i][j], row_center + drow[i], col_center + dcol[j]);
            wrefresh(game);
        }
    }
    wrefresh(game);
}

// functie pentru setarile de culoare
int setari_colorare()
{
    // daca terminalul nu accepta culori, eroare
    if (!has_colors())
    {
        endwin();
        return 1;
    }

    // daca nu se poate activa modul de culoare, eroare
    if (start_color() != OK)
    {
        endwin();
        return 1;
    }

    start_color();
    // culorile special adaugate
    init_color(COLOR_CHARCOAL, 149, 275, 325);
    init_color(COLOR_PGREEN, 165, 616, 561);
    init_color(COLOR_SAFFRON, 916, 769, 416);
    init_color(COLOR_SANDY, 957, 635, 380);
    init_color(COLOR_SIENNA, 906, 435, 318);
    init_color(COLOR_MIDGREEN, 0, 200, 239);
    init_color(COLOR_2, 102, 380, 345);
    init_color(COLOR_4, 200, 757, 682);
    init_color(COLOR_8, 302, 557, 659);
    init_color(COLOR_16, 925, 808, 514);
    init_color(COLOR_32, 882, 698, 239);
    init_color(COLOR_64, 953, 608, 325);
    init_color(COLOR_128, 906, 435, 318);
    init_color(COLOR_256, 824, 400, 590);
    init_color(COLOR_512, 886, 302, 157);
    init_color(COLOR_1024, 773, 239, 106);
    init_color(COLOR_2048, 361, 98, 39);

    // fundal
    init_pair(1, COLOR_WHITE, COLOR_CHARCOAL);
    init_pair(2, COLOR_BLACK, COLOR_PGREEN);
    init_pair(15, COLOR_CHARCOAL, COLOR_WHITE);
    // eroare
    init_pair(14, COLOR_WHITE, COLOR_MIDGREEN);

    // pt numarul 2
    init_pair(3, COLOR_WHITE, COLOR_2);
    // pt numarul 4
    init_pair(4, COLOR_WHITE, COLOR_4);
    // pt numarul 8
    init_pair(5, COLOR_WHITE, COLOR_8);
    // pt numarul 16
    init_pair(6, COLOR_WHITE, COLOR_16);
    // pt numarul 32
    init_pair(7, COLOR_WHITE, COLOR_32);
    // pt numarul 64
    init_pair(8, COLOR_WHITE, COLOR_64);
    // pt numarul 128
    init_pair(9, COLOR_WHITE, COLOR_128);
    // pt numarul 256
    init_pair(10, COLOR_WHITE, COLOR_256);
    // pt numarul 512
    init_pair(11, COLOR_WHITE, COLOR_512);
    // pt numarul 1024
    init_pair(12, COLOR_WHITE, COLOR_1024);
    // pt numarul 2048
    init_pair(13, COLOR_WHITE, COLOR_2048);

    return 0;
}

// functie care afiseaza Text Art un bloc anume
void fill_block(WINDOW *game, int nr, int startR, int startC)
{
    if (nr == 2)
    {
        wattron(game, COLOR_PAIR(3) | A_BOLD);
        mvwprintw(game, startR, startC, "              "), startR++;
        mvwprintw(game, startR, startC, "     __       "), startR++;
        mvwprintw(game, startR, startC, "      _)      "), startR++;
        mvwprintw(game, startR, startC, "     /__      "), startR++;
        mvwprintw(game, startR, startC, "              "), startR++;
        mvwprintw(game, startR, startC, "              "), startR++;
        wrefresh(game);
        wattroff(game, COLOR_PAIR(3) | A_BOLD);
    }
    else if (nr == 4)
    {
        wattron(game, COLOR_PAIR(4) | A_BOLD);
        mvwprintw(game, startR, startC, "              "), startR++;
        mvwprintw(game, startR, startC, "              "), startR++;
        mvwprintw(game, startR, startC, "     |_|      "), startR++;
        mvwprintw(game, startR, startC, "       |      "), startR++;
        mvwprintw(game, startR, startC, "              "), startR++;
        mvwprintw(game, startR, startC, "              "), startR++;
        wrefresh(game);
        wattroff(game, COLOR_PAIR(4) | A_BOLD);
    }
    else if (nr == 8)
    {
        wattron(game, COLOR_PAIR(5) | A_BOLD);
        mvwprintw(game, startR, startC, "              "), startR++;
        mvwprintw(game, startR, startC, "      _       "), startR++;
        mvwprintw(game, startR, startC, "     (_)      "), startR++;
        mvwprintw(game, startR, startC, "     (_)      "), startR++;
        mvwprintw(game, startR, startC, "              "), startR++;
        mvwprintw(game, startR, startC, "              "), startR++;
        wrefresh(game);
        wattroff(game, COLOR_PAIR(5) | A_BOLD);
    }
    else if (nr == 16)
    {
        wattron(game, COLOR_PAIR(6) | A_BOLD);
        mvwprintw(game, startR, startC, "              "), startR++;
        mvwprintw(game, startR, startC, "        _     "), startR++;
        mvwprintw(game, startR, startC, "    /| |_'    "), startR++;
        mvwprintw(game, startR, startC, "     | |_)    "), startR++;
        mvwprintw(game, startR, startC, "              "), startR++;
        mvwprintw(game, startR, startC, "              "), startR++;
        wrefresh(game);
        wattroff(game, COLOR_PAIR(6) | A_BOLD);
    }
    else if (nr == 32)
    {
        wattron(game, COLOR_PAIR(7) | A_BOLD);
        mvwprintw(game, startR, startC, "              "), startR++;
        mvwprintw(game, startR, startC, "    __ __     "), startR++;
        mvwprintw(game, startR, startC, "    __) _)    "), startR++;
        mvwprintw(game, startR, startC, "    __)/__    "), startR++;
        mvwprintw(game, startR, startC, "              "), startR++;
        mvwprintw(game, startR, startC, "              "), startR++;
        wrefresh(game);
        wattroff(game, COLOR_PAIR(7) | A_BOLD);
    }
    else if (nr == 64)
    {
        wattron(game, COLOR_PAIR(8) | A_BOLD);
        mvwprintw(game, startR, startC, "              "), startR++;
        mvwprintw(game, startR, startC, "     _        "), startR++;
        mvwprintw(game, startR, startC, "    |_'|_|    "), startR++;
        mvwprintw(game, startR, startC, "    |_)  |    "), startR++;
        mvwprintw(game, startR, startC, "              "), startR++;
        mvwprintw(game, startR, startC, "              "), startR++;
        wrefresh(game);
        wattroff(game, COLOR_PAIR(8) | A_BOLD);
    }
    else if (nr == 128)
    {

        wattron(game, COLOR_PAIR(9) | A_BOLD);
        mvwprintw(game, startR, startC, "              "), startR++;
        mvwprintw(game, startR, startC, "     __  _    "), startR++;
        mvwprintw(game, startR, startC, "   /| _)(_)   "), startR++;
        mvwprintw(game, startR, startC, "    |/__(_)   "), startR++;
        mvwprintw(game, startR, startC, "              "), startR++;
        mvwprintw(game, startR, startC, "              "), startR++;
        wrefresh(game);
        wattroff(game, COLOR_PAIR(9) | A_BOLD);
    }
    else if (nr == 256)
    {
        wattron(game, COLOR_PAIR(10) | A_BOLD);
        mvwprintw(game, startR, startC, "              "), startR++;
        mvwprintw(game, startR, startC, "  __  __ _    "), startR++;
        mvwprintw(game, startR, startC, "   _)|_ |_'   "), startR++;
        mvwprintw(game, startR, startC, "  /____)|_)   "), startR++;
        mvwprintw(game, startR, startC, "              "), startR++;
        mvwprintw(game, startR, startC, "              "), startR++;
        wrefresh(game);
        wattroff(game, COLOR_PAIR(10) | A_BOLD);
    }
    else if (nr == 512)
    {
        wattron(game, COLOR_PAIR(11) | A_BOLD);
        mvwprintw(game, startR, startC, "              "), startR++;
        mvwprintw(game, startR, startC, "   __   __    "), startR++;
        mvwprintw(game, startR, startC, "  |_ /|  _)   "), startR++;
        mvwprintw(game, startR, startC, "  __) | /__   "), startR++;
        mvwprintw(game, startR, startC, "              "), startR++;
        mvwprintw(game, startR, startC, "              "), startR++;
        wrefresh(game);
        wattroff(game, COLOR_PAIR(11) | A_BOLD);
    }
    else if (nr == 1024)
    {
        wattron(game, COLOR_PAIR(12) | A_BOLD);
        mvwprintw(game, startR, startC, "              "), startR++;
        mvwprintw(game, startR, startC, "    _ __      "), startR++;
        mvwprintw(game, startR, startC, " /|/ \\ _)|_|  "), startR++;
        mvwprintw(game, startR, startC, "  |\\_//__  |  "), startR++;
        mvwprintw(game, startR, startC, "              "), startR++;
        mvwprintw(game, startR, startC, "              "), startR++;
        wrefresh(game);
        wattroff(game, COLOR_PAIR(12) | A_BOLD);
    }
    else if (nr == 2048)
    {
        wattron(game, COLOR_PAIR(13) | A_BOLD);
        mvwprintw(game, startR, startC, "              "), startR++;
        mvwprintw(game, startR, startC, " __  _     _  "), startR++;
        mvwprintw(game, startR, startC, "  _)/ \\|_|(_) "), startR++;
        mvwprintw(game, startR, startC, " /__\\_/  |(_) "), startR++;
        mvwprintw(game, startR, startC, "              "), startR++;
        mvwprintw(game, startR, startC, "              "), startR++;
        wrefresh(game);
        wattroff(game, COLOR_PAIR(13) | A_BOLD);
    }
}

// functie care produce interfata meniului
int display_menu(int *hasResume)
{

    int row, col, i, len, nr_options;
    int length, y;

    clear();
    refresh();

    // marimea ecranului
    getmaxyx(stdscr, row, col);
    // lungime linie de ASCII Text Art
    len = col / 2 - 96 / 2;
    box(stdscr, 0, 0);
    bkgd(COLOR_PAIR(1));

    mvprintw(10, len, " ::::::::   :::::::      :::      ::::::::        ::::::::      :::     ::::    ::::  ::::::::::");
    mvprintw(11, len, ":+:    :+: :+:   :+:    :+:      :+:    :+:      :+:    :+:   :+: :+:   +:+:+: :+:+:+ :+:       ");
    mvprintw(12, len, "      +:+  +:+  :+:+   +:+ +:+   +:+    +:+      +:+         +:+   +:+  +:+ +:+:+ +:+ +:+       ");
    mvprintw(13, len, "    +#+    +#+ + +:+  +#+  +:+    +#++:++#       :#:        +#++:++#++: +#+  +:+  +#+ +#++:++#  ");
    mvprintw(14, len, "  +#+      +#+#  +#+ +#+#+#+#+#+ +#+    +#+      +#+   +#+# +#+     +#+ +#+       +#+ +#+       ");
    mvprintw(15, len, " #+#       #+#   #+#       #+#   #+#    #+#      #+#    #+# #+#     #+# #+#       #+# #+#       ");
    mvprintw(16, len, "##########  #######        ###    ########        ########  ###     ### ###       ### ##########");

    int mRow = 10, mCol = 30;
    WINDOW *menu = newwin(mRow, mCol, 5 + row / 2 - mRow / 2, col / 2 - mCol / 2);
    // deseneaza un chenar in jurul ferestrei
    box(menu, ACS_VLINE, ACS_HLINE);
    // coloram fundalul ferestrei
    wbkgd(menu, COLOR_PAIR(15));
    refresh();
    wrefresh(menu);

    // foloseste sageti
    keypad(menu, true);

    // alocare dinamica pentru optiuni
    char **optiuni = (char **)malloc(3 * sizeof(char));
    for (i = 0; i < 3; i++)
        optiuni[i] = (char *)malloc(strlen("New Game") * sizeof(char));

    strcpy(optiuni[0], "New Game");
    strcpy(optiuni[1], "Resume");
    strcpy(optiuni[2], "Quit");

    int optiune, ales = 0;
    nr_options = 3;

    y = getmaxx(menu);

    while (true)
    {
        for (i = 0; i < nr_options; i++)
        {
            if (i == ales)
            {
                // daca nu avem joc deschis, fa resume opac
                if (*hasResume == 0 && i == 1)
                    wattron(menu, A_DIM);
                else
                    wattron(menu, A_REVERSE);
            }
            length = strlen(optiuni[i]);
            mvwprintw(menu, i + 3, y / 2 - length / 2, "%s", optiuni[i]);
            wattroff(menu, A_REVERSE);
            wattroff(menu, A_DIM);
        }
        optiune = wgetch(menu);

        // navigam cu sageti
        switch (optiune)
        {
        case KEY_UP:
            ales--;
            if (ales == -1)
                ales = nr_options - 1;
            break;
        case KEY_DOWN:
            ales++;
            if (ales == nr_options)
                ales = 0;
            break;
        default:
            break;
        }
        // daca apasa enter
        if (optiune == 10)
        {
            // a ales New Game
            if (ales == 0)
            {
                *hasResume = 1;
                return 1; // new game
            }
            // a ales Quit
            else if (ales == nr_options - 1)
            {
                endwin();
                // quit
                return 3;
            }
            // resume
            else if (*hasResume && ales == 1)
                return 2;
        }
        refresh();
    }

    // Eliberare memorie
    for (i = 0; i < 3; i++)
        free(optiuni[i]);
    free(optiuni);
}

// functie pentru final de joc - pierdut
void theEnd()
{
    int col, len, lentext;
    char text[] = "Press any key to return to menu";
    // marimea ecranului
    col = getmaxx(stdscr);
    // lungime linie de ASCII Text Art
    lentext = col / 2 - strlen(text) / 2;
    len = col / 2 - 96 / 2;

    clear();
    refresh();
    bkgd(COLOR_PAIR(1));
    mvprintw(10, len, "::::::::::: :::::::::  :::   :::          :::      ::::::::      :::     ::::::::::: ::::    :::");
    mvprintw(11, len, "    :+:     :+:    :+: :+:   :+:        :+: :+:   :+:    :+:   :+: :+:       :+:     :+:+:   :+:");
    mvprintw(12, len, "    +:+     +:+    +:+  +:+ +:+        +:+   +:+  +:+         +:+   +:+      +:+     :+:+:+  +:+");
    mvprintw(13, len, "    +#+     +#++:++#:    +#++:        +#++:++#++: :#:        +#++:++#++:     +#+     +#+ +:+ +#+");
    mvprintw(14, len, "    +#+     +#+    +#+    +#+         +#+     +#+ +#+   +#+# +#+     +#+     +#+     +#+  +#+#+#");
    mvprintw(15, len, "    #+#     #+#    #+#    #+#         #+#     #+# #+#    #+# #+#     #+#     #+#     #+#   #+#+#");
    mvprintw(16, len, "    ###     ###    ###    ###         ###     ###  ########  ###     ### ########### ###    ####");
    mvprintw(25, lentext, "%s", text);
    getch();
    endwin();
}

// functie care verifica daca tabela contine un 2048
int isWinner(int board[6][6])
{
    int i, j;
    for (i = 1; i <= 4; i++)
        for (j = 1; j <= 4; j++)
            if (board[i][j] == 2024)
                return 1;
    return 0;
}

// functie pentru final de joc - castigat
void winner()
{
    int col, len, lentext;
    char text[] = "Press any key to return to menu";

    // marimea ecranului
    col = getmaxx(stdscr);
    // lungime linie de ASCII Text Art
    lentext = col / 2 - strlen(text) / 2;
    len = col / 2 - 71 / 2;

    clear();
    refresh();
    bkgd(COLOR_PAIR(1));
    mvprintw(10, len, ":::       ::: ::::::::::: ::::    ::: ::::    ::: :::::::::: ::::::::: ");
    mvprintw(11, len, ":+:       :+:     :+:     :+:+:   :+: :+:+:   :+: :+:        :+:    :+:");
    mvprintw(12, len, "+:+       +:+     +:+     :+:+:+  +:+ :+:+:+  +:+ +:+        +:+    +:+");
    mvprintw(13, len, "+#+  +:+  +#+     +#+     +#+ +:+ +#+ +#+ +:+ +#+ +#++:++#   +#++:++#: ");
    mvprintw(14, len, "+#+ +#+#+ +#+     +#+     +#+  +#+#+# +#+  +#+#+# +#+        +#+    +#+");
    mvprintw(15, len, " #+#+# #+#+#      #+#     #+#   #+#+# #+#   #+#+# #+#        #+#    #+#");
    mvprintw(16, len, "  ###   ###   ########### ###    #### ###    #### ########## ###    ###");
    mvprintw(25, lentext, "%s", text);
    getch();
}

// functie pentru joc nou
int new_game(int board[6][6], int *hasResume)
{
    int i, j, nr, row_center, col_center;
    int secDelay, sec;
    char optiune, changes;

    // cate secunde de delay pt autoplay
    secDelay = 3;

    // la intoarcere vom avea resume
    *hasResume = 1;

    // sa nu vedem cursorul
    curs_set(0);

    // coordonatele maxime
    getmaxyx(stdscr, row_center, col_center);

    // fereastra noua pentru joc
    WINDOW *game = newwin(row_center, col_center, 0, 0);
    wbkgd(game, COLOR_PAIR(1));

    keypad(game, TRUE);

    row_center /= 2, col_center /= 2;
    // Umplem doua casute aleator
    randomStart(&i, &j, &nr);
    board[i][j] = nr;
    randomStart(&i, &j, &nr);
    board[i][j] = nr;

    // setam sa inregistreze la secunda lipsa unei taste apasate
    wtimeout(game, 1000);
    // incepem sa numaram de la 0
    sec = 0;

    while (1)
    {
        // arata tabela curenta
        update(game, board, row_center, col_center);
        // EASTER EGG: daca jucatorul are punctajul fix 1000
        if (board[0][0] == 1000)
        {
            // afiseaza un pisoi
            pisoias();
            resume_game(board, hasResume);
            return 1000;
        }
        // salvam o copie
        int aux[6][6];
        copy_matrix(aux, board);

        // optiune salveaza ce tasta apasam
        optiune = wgetch(game);
        if (optiune == 3)
        {
            // Actiune pentru tasta UP
            changes = swipe_up(board);
        }
        else if (optiune == 2)
        {
            // Actiune pentru tasta DOWN
            changes = swipe_down(board);
        }
        else if (optiune == 4)
        {
            // Actiune pentru tasta LEFT
            changes = swipe_left(board);
        }
        else if (optiune == 5)
        {
            // Actiune pentru tasta RIGHT
            changes = swipe_right(board);
        }
        else if (optiune == 'q')
            return 3;

        if (isWinner(board))
        {
            update(game, board, row_center, col_center);
            // Asteapta sa apase o tasta
            wgetch(game);
            winner();
        }

        // pentru fiecare secunda trecuta
        if (optiune == ERR)
        {
            sec++;
            // auto-play daca avem destule secunde
            if (sec == secDelay)
            {
                changes = autoMove(board);
                sec = 0;
            }
            // actualizeaza timpul
            DateTimePoints(game, board[0][0]);
        }

        // adauga element nou daca avem o miscare anterioara valida
        if ((changes || full_board(board) < 16) && !equal_matrix(board, aux))
            addNew(board);

        // daca nu au mai fost facute modificari si tabela e full, ai pierdut
        if (changes == 0 && full_board(board) == 16)
        {
            theEnd();
            hasResume = 0;
            return 3;
        }
    }
}

// functie pentru easter egg
void pisoias()
{
    int col, len, lentext;
    char text[] = "You found the Easter Egg! Press any key to continue :)";

    // marimea ecranului
    col = getmaxx(stdscr);
    // lungime linie de ASCII Text Art
    lentext = col / 2 - strlen(text) / 2;
    len = col / 2 - 68 / 2;

    clear();
    refresh();
    bkgd(COLOR_PAIR(1));
    mvprintw(4, len, "                                               .--.                 ");
    mvprintw(5, len, "                                               `.  \\                ");
    mvprintw(6, len, "                                                 \\  \\                ");
    mvprintw(7, len, "                                                  .  \\               ");
    mvprintw(8, len, "                                                  :   .              ");
    mvprintw(9, len, "                                                  |    .             ");
    mvprintw(10, len, "                                                  |    :             ");
    mvprintw(11, len, "                                                  |    |             ");
    mvprintw(12, len, "  ..._  ___                                       |    |             ");
    mvprintw(13, len, " `."
                      ".`''''"
                      "--..___                              |    |             ");
    mvprintw(14, len, " ,-\\  \\             "
                      "-...__         _____________/    |              ");
    mvprintw(15, len, " / ` \" \'                    `"
                      ""
                      ""
                      ""
                      "                  .              ");
    mvprintw(16, len, " \\                                                      L             ");
    mvprintw(17, len, " (>                                                      \\            ");
    mvprintw(18, len, "/                                                         \\           ");
    mvprintw(19, len, "\\_    ___..---.                                            L          ");
    mvprintw(20, len, "  `--'         '.                                           \\         ");
    mvprintw(21, len, "                 .                                           \\_       ");
    mvprintw(22, len, "                _/`.                                           `.._   ");
    mvprintw(23, len, "             .'     -.                                             `. ");
    mvprintw(24, len, "            /     __.-Y     /''''''-...___,...--------.._            |");
    mvprintw(25, len, "           /   _.\"    |    /                ' .      \\   '---..._    |");
    mvprintw(26, len, "          /   /      /    /                _,. '    ,/           |   |");
    mvprintw(27, len, "          \\_,'     _.'   /              /''     _,-'            _|   |");
    mvprintw(28, len, "                  '     /               `-----''               /     |");
    mvprintw(29, len, "                  `...-'                                       `...-' ");

    mvprintw(25, lentext, "%s", text);
    getch();
}

// functie pentru resume game
int resume_game(int board[6][6], int *hasResume)
{
    new_game(board, hasResume);
    return 0;
}
