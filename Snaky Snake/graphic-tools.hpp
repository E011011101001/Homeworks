#ifndef SNAKYSNAKES_GRAPHIC_TOOLS_HPP
#define SNAKYSNAKES_GRAPHIC_TOOLS_HPP

#include <iostream>

enum InputMsg
{
    msg_null,
    up, down, left, right,
    enter, esc
};

using Dir = InputMsg;

Dir opposite(Dir dir);
bool is_direction(InputMsg input);

#ifdef __GNUC__

bool _kbhit();
char _getch();

#elif defined _MSC_VER
#include <Windows.h>
#include <conio.h>
#endif

void clear_screen();
void hide_cursor();
void reveal_cursor();
InputMsg get_input();
void clear_input();

std::ostream& color_null(std::ostream& out);
std::ostream& red(std::ostream& out);
std::ostream& blue(std::ostream& out);
std::ostream& white(std::ostream& out);
using Color = decltype(&color_null);

struct Coordinate;
class Cursor
{
public:
    static void move_to(const Coordinate& r);
    static void abs_move_to(const Coordinate& r);
    Cursor();
    ~Cursor();
};

#endif // SNAKYSNAKES_GRAPHIC_TOOLS_HPP

