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

    GameOption options;
    Menu menu(std::vector<MenuItem> {{
        "入门版", [&options]() -> void {
            options.difficulty = 0;
        }
    }, {
        "进阶版", [&options]() -> void {
            options.difficulty = 1;
        }
    }, {
        "高级版", [&options]() -> void {
            options.difficulty = 2;
        }
    }});

    try
    {
        SnakySnake game(options, recordFile);
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

