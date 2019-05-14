#ifndef SNAKYSNAKES_MENU_HPP
#define SNAKYSNAKES_MENU_HPP

#include <string>
#include <vector>
#include <cstddef>
#include <initializer_list>

struct GameOption
{
    const unsigned int difficulty;
};

class MenuItem
{
public:
    const std::string title;
    GameOption (* const fp)();
    MenuItem(const std::string&& title, GameOption (* const f)());
    MenuItem(const MenuItem& item);
    ~MenuItem();
};

class MenuArrow
{
    size_t _place;
    size_t _itemSize;
public:
    MenuArrow(size_t size);
    ~MenuArrow();

    size_t place();
    size_t operator ++();
    size_t operator --();
};

class Menu
{
    size_t _layer;
    std::vector<MenuItem> _items;
    MenuArrow arrow; // Must be declared after _items
    const size_t marginLeft;
public:
    void print_menu();
    GameOption get_option();
    Menu(std::vector<MenuItem>&& items, size_t layer = 0);
};

#endif // SNAKYSNAKES_MENU_HPP

