#include <string>
#include <vector>
#include <cstdlib>
#include <cassert>
#include "menu.hpp"
#include "snake.hpp"
#include "graphic-tools.hpp"

const std::string recordFile("record.txt");

int main()
{
    hide_cursor();
    Menu menu(std::vector<MenuItem> {{
        "���Ű�", []() -> GameOption {
            return { 0 };
        }
    }, {
        "���װ�", []() -> GameOption {
            return { 1 };
        }
    }, {
        "�߼���", []() -> GameOption {
            return { 2 };
        }
    }});
    const GameOption option(menu.get_option());
    try
    {
        SnakySnake game(option, recordFile);
        game.run();
        reveal_cursor();
    }
    catch (const NoBlankSpace&)
    {
        reveal_cursor();
        exit(EXIT_SUCCESS);
    }
    catch (const std::exception& e)
    {
        e.what();
        reveal_cursor();
        exit(EXIT_FAILURE);
    }
    return 0;
}

