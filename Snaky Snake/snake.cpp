#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <ctime>
#include <chrono>
#include <thread>
#include <algorithm>
#include <cassert>
#include "menu.hpp"
#include "snake.hpp"

#define milli_sleep(msec) std::this_thread::sleep_for(std::chrono::milliseconds(msec))

const size_t SnakySnake::MAX_DEAD_TIME(5);

SnakySnake::SnakySnake(const GameOption& option, const std::string& file)
    : _sleepTime(200),
    _deadTime(0),
    _option(option),
    _recordFile(file),
    _highestScores(4, 0), /* 4 levels allowed at most */
    _score(option.difficulty),
    _snake(),
    _foods(0),
    _canvas(Canvas::get_instance())
{
    std::ifstream fin(file);
    if (fin.is_open()) // if not, file doesn't exist, just assume that the game is being played for the first time
    {
        while (fin)
        {
            unsigned int difficulty;
            fin >> difficulty;
            fin >> _highestScores.at(difficulty);
        }
        fin.close();
    }
    _score.set_highest(_highestScores.at(_option.difficulty));
    _score.print_score();
    supplement_food();
}

void SnakySnake::run()
{
    bool dead(false);
    do
    {
        milli_sleep(_sleepTime);
        handle_input();
        Coordinate snakeNextCoord(_snake.next_coord());
        if (_canvas.is_food(snakeNextCoord))
            eat_food(snakeNextCoord);
        else if (_canvas.is_wall(snakeNextCoord) || _canvas.is_snake(snakeNextCoord))
        {
            switch (_option.difficulty)
            {
                case 0:
                case 3:
                    dead = true;
                    break;
                case 1:
                    _snake.turn_wall();
                    _snake.reinit();
                    _score.reset_length();
                    break;
                case 2:
                    ++_deadTime;
                    if (_deadTime >= MAX_DEAD_TIME)
                        dead = true;
                    else
                    {
                        _snake.turn_food(_foods);
                        _snake.reinit();
                        _score.reset_length();
                    }
                    break;
                default:
                    break;
            }
        }
        else
            _snake.move();
    } while (!dead);
}

void SnakySnake::handle_input()
{
    while (_kbhit())
    {
        InputMsg input = get_input();
        if (is_direction(input))
            _snake.change_dir(input);
        clear_input();
    }
}

SnakySnake::~SnakySnake()
{
    _highestScores.at(_option.difficulty) = _score.get_highest_score();
    std::ofstream fout(_recordFile);
    if (fout.is_open())
    {
        for (size_t i = 0; i < _highestScores.size(); ++i)
            fout << i << ' ' << _highestScores.at(i) << std::endl;
        fout.close();
    }
    // highlight final score when _score destructs
}

void SnakySnake::supplement_food()
{
    std::random_device rdDevice;
    // static std::default_random_engine rdEngine (rdDevice);
    static std::uniform_int_distribution<size_t> interval(1, 5);
    size_t foodNum(interval(rdDevice));
    for (size_t i = 0; i < foodNum; ++i)
        _foods.emplace_back(new Food());
}

std::vector<std::shared_ptr<Food>>::iterator SnakySnake::find_food_at(const Coordinate& r)
{
    return std::find_if(_foods.begin(), _foods.end(), [&r](const std::shared_ptr<Food>& food) -> bool
    {
        return food->r == r;
    });
}

void SnakySnake::eat_food(const Coordinate& r)
{
    auto iterPFood(find_food_at(r)), foodEnd(_foods.end());
    assert(iterPFood != foodEnd);
    _score += (*iterPFood)->value;
    _foods.erase(iterPFood);
    _snake.evolve();
    if (!_foods.size())
        supplement_food();
}

