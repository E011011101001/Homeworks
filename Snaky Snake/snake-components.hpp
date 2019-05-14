#ifndef SNAKYSNAKES_SNAKE_COMPONENTS_HPP
#define SNAKYSNAKES_SNAKE_COMPONENTS_HPP

#include <cstddef>
#include <deque>
#include <memory>
#include "canvas.hpp"
#include "menu.hpp"

class SnakeNode
{
    Canvas& _canvas;
public:
    const Coordinate r;
    SnakeNode();
    SnakeNode(const Coordinate& r);
    ~SnakeNode();
};

class Food
{
    Canvas& _canvas;
public:
    const unsigned int value;
    const Coordinate r;
    Food();
    Food(const Coordinate& r); // Called only to turn dead Snake into Food
    ~Food();
};

class Snake
{
    Canvas& _canvas;
    std::deque<std::shared_ptr<SnakeNode>> _nodes;
    Dir _direction, _preDir;

public:
    Snake();
    ~Snake() = default;
    void move();
    void evolve();
    Coordinate next_coord() const;
    void change_dir(InputMsg dir);
    const SnakeNode& head() const;
    void push(const Coordinate& r);
    void pop();
    void turn_wall();
    void turn_food(std::vector<std::shared_ptr<Food>>& foods);
    void init();
    void reinit();
};

class Score
{
    static std::map<unsigned int, std::string> levelStrMap;
    Canvas& _canvas;
    unsigned int _score;
    size_t _length;
    unsigned int _highestScore;
    const std::string levelStr;
    const unsigned int difficulty;
public:
    Score(unsigned int difficulty);
    ~Score();
    void print_score() const;
    void set_highest(unsigned int hScore);
    unsigned int get_highest_score() { return _highestScore; }
    void reset_length();
    void operator +=(unsigned int);
};

#endif // SNAKYSNAKES_SNAKE_COMPONENTS_HPP

