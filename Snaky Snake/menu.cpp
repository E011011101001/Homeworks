#include "menu.hpp"
#include "graphic-tools.hpp"
#include <iostream>

void Menu::print_menu()
{
    using std::cout;
    for(size_t i = 0; i < _items.size(); ++i)
    {
        for (size_t n = 0; n < marginLeft; ++n)
            cout.put(' ');
        cout << ((i == arrow.place()) ? "->" : "  ")
            << i + 1 << ". " << _items.at(i).title << std::endl;
    }
}

void Menu::exec()
{
    do
    {
        clear_screen();
        print_menu();
        bool validInput(true);
        do
        {
            switch (get_input())
            {
                case up:
                    --arrow;
                    validInput = true;
                    break;
                case down:
                    ++arrow;
                    validInput = true;
                    break;
                case enter:
                    clear_screen();
                    _items.at(arrow.place()).fp();
                    return;
                case esc:
                    if (_layer >= 1)
                        return; // TODO
                    else
                        validInput = false;
                    break;
                default:
                    validInput = false;
                    break;
            }
        } while (!validInput);
    } while (true);
}

MenuItem::MenuItem(const std::string&& title, std::function<void ()> fp)
    : title(title), fp(fp)
{
}

MenuItem::~MenuItem()
{}

MenuItem::MenuItem(const MenuItem &item)
    : title(item.title), fp(item.fp)
{
}

Menu::Menu(std::vector<MenuItem>&& items, size_t layer /* 0 by default */)
// TODO: test marginLeft
    : _layer(layer), _items(items) ,arrow(items.size()), marginLeft(5)
{
    exec();
}

MenuArrow::MenuArrow(size_t size)
    : _place(0), _itemSize(size)
{

}

size_t MenuArrow::place()
{
    return _place;
}

size_t MenuArrow::operator++()
{
    return (_place == _itemSize - 1) ? (_place = 0) : (++_place);
}

size_t MenuArrow::operator--()
{
    return (_place == 0) ? (_place = _itemSize - 1) : (--_place);
}

MenuArrow::~MenuArrow()
{
}

