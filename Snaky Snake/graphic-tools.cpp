#include "graphic-tools.hpp"
#include <cstdlib>
#include <exception>
#include <cassert>
#include "canvas.hpp"

#ifdef __GNUC__
#include <termios.h>
#include <stdio.h>
#include <zconf.h>
#include <fcntl.h>

namespace
{
    void initTermios(const termios& oldTerm, bool echo)
    {
        termios newTerm = oldTerm;
        newTerm.c_lflag &= ~ICANON; // Disable Buffer
        if (echo)
        {
            newTerm.c_lflag |= ECHO;
        }
        else
        {
            newTerm.c_lflag &= ~ECHO;
        }
        tcsetattr(0, TCSANOW, &newTerm);
    }

    void resetTermios(const termios* oldTerm)
    {
        tcsetattr(0, TCSANOW, oldTerm);
    }

    char getch_(bool echo)
    {
        char ch;
        termios oldTerm;
        tcgetattr(0, &oldTerm);
        initTermios(oldTerm, echo);
        ch = getchar();
        resetTermios(&oldTerm);
        return ch;
    }
}

bool _kbhit()
{
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    // tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return true;
    }
    return false;
}

char _getch()
{
    return getch_(false);
}

char _getche()
{
    return getch_(true);
}

#define ANSICodeStr(num) ("\e["#num"m")

std::ostream& color_null(std::ostream& out)
{
    out << ANSICodeStr(0);
    return out;
}

std::ostream& red(std::ostream& out)
{
    out << ANSICodeStr(41);
    return out;
}

std::ostream& blue(std::ostream& out)
{
    out << ANSICodeStr(44);
    return out;
}

std::ostream& white(std::ostream& out)
{
    out << ANSICodeStr(47);
    return out;
}

void hide_cursor()
{
    std::cout << "\e[?25l";
}

void reveal_cursor()
{
    std::cout << "\e[?25l";
}
#undef ANSICodeStr

#elif defined _MSC_VER // END __GNUC__
#include <Windows.h>

void hide_cursor()
{
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(hout, &CursorInfo);
    CursorInfo.bVisible = false;
    SetConsoleCursorInfo(hout, &CursorInfo);
}

void reveal_cursor()
{
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(hout, &CursorInfo);
    CursorInfo.bVisible = true;
    SetConsoleCursorInfo(hout, &CursorInfo);
}

std::ostream& color_null(std::ostream& out)
{
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hout, 15);
    return out;
}

std::ostream& blue(std::ostream& out)
{
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hout, 9 * 16 + 15);
    return out;
}

std::ostream& red(std::ostream& out)
{
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hout, 12 * 16 + 15);
    return out;
}

std::ostream& white(std::ostream& out)
{
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hout, 23 * 16 + 15);
    return out;
}

#endif // _MSC_VER

void clear_screen()
{
#ifdef __GNUC__
    system("clear");
#elif defined _MSC_VER
    system("cls");
#endif // _MSC_VER
    Cursor::abs_move_to({ 0, 0 });
}

InputMsg get_input()
{
    char ch = _getch();
#ifdef __GNUC__
    if (ch == 27)
    {
        if (!_kbhit())
            return esc;
        ch = _getch();
        if (ch == 91)
        {
            ch = _getch();
            switch (ch)
            {
                case 65:
                    return up;
                case 66:
                    return down;
                case 67:
                    return right;
                case 68:
                    return left;
                default:
                    return msg_null;
            }
        }
    }
    else if (ch == 10)
        return enter;
    else
        return msg_null;
#elif defined(_MSC_VER) // END __GNUC__

    if (ch == -32)
    {
        if (_kbhit())
        {
            ch = _getch();
            switch (ch)
            {
                case 72:
                    return up;
                case 75:
                    return left;
                case 77:
                    return right;
                case 80:
                    return down;
            }
        }
    }
    else if (ch == 13)
        return enter;
    else if (ch == 27)
        return esc;

#endif // _MSC_VER
    return msg_null;
}

void clear_input()
{
    bool noInput(false);
    do
    {
        if (_kbhit())
            _getch();
        else
            noInput = true;
    } while (!noInput);
}

Dir opposite(Dir dir)
{
    bool isDirection(is_direction(dir));
    assert(isDirection);
    switch (dir)
    {
        case up:
            return down;
        case down:
            return up;
        case left:
            return right;
        case right:
            return left;
        default:
            throw std::invalid_argument("not a direction");
    }
}

bool is_direction(InputMsg input)
{
    return (
        input == up ||
        input == down ||
        input == left ||
        input == right
    );
}

// coordinates not considering border of game
void Cursor::move_to(const Coordinate& r)
{
    // begining from the third line
#ifdef __GNUC__
    abs_move_to({ 2 * r.x + 3, r.y + 3 });
#elif defined _MSC_VER
    abs_move_to({ 2 * r.x + 2, r.y + 2 });
#endif
}

Cursor::Cursor() {}
Cursor::~Cursor() {}

void Cursor::abs_move_to(const Coordinate& r)
{
#ifdef __GNUC__
    std::cout << "\e[" << r.y << ';' << r.x << 'H';
#elif defined _MSC_VER
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = static_cast<SHORT>(r.x);
    pos.Y = static_cast<SHORT>(r.y);
    SetConsoleCursorPosition(hout,pos);
#endif
}

