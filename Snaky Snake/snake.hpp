#ifndef SNAKYSNAKES_SNAKE_HPP
#define SNAKYSNAKES_SNAKE_HPP

#include <vector>
#include "canvas.hpp"
#include "snake-components.hpp"

// The whole game
class SnakySnake
{
    const unsigned int _sleepTime;
    size_t _deadTime;
    static const size_t MAX_DEAD_TIME;
    GameOption _option;
    const std::string _recordFile;
    std::vector<unsigned int> _highestScores;
    Score _score;
    Snake _snake; // declare after _score to destruct after it
    std::vector<std::shared_ptr<Food>> _foods;
    Canvas& _canvas;

    void supplement_food();
    void eat_food(const Coordinate& r);
    void handle_input(); // Player interaction
    std::vector<std::shared_ptr<Food>>::iterator find_food_at(const Coordinate& r);
public:
    SnakySnake(const GameOption& option, const std::string& file);
    ~SnakySnake();
    void run();
};

#endif // SNAKYSNAKES_SNAKE_HPP

