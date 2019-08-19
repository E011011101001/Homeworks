#ifndef SNAKYSNAKES_MENU_HPP
#define SNAKYSNAKES_MENU_HPP

#include <string>
#include <vector>
#include <cstddef>
#include <functional>
#include <initializer_list>

struct GameOption
{
    unsigned int difficulty;
    GameOption() : difficulty(0) {}
};

class MenuItem
{
public:
    const std::string title;
    std::function<void ()> fp;

    MenuItem(const std::string&& title, std::function<void ()>);
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
    void exec();
    explicit Menu(std::vector<MenuItem>&& items, size_t layer = 0);
};

#endif // SNAKYSNAKES_MENU_HPP

