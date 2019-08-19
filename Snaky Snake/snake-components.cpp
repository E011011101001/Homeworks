// SnakeBody and Food

#include "snake-components.hpp"
#include <random>
#include <exception>
#include <cassert>

Snake::Snake()
    : _canvas(Canvas::get_instance()), _nodes (0)
{
    init();
}

void Snake::move()
{
    bool gonnaHitWall(_canvas.is_wall((head().r.next(_direction))));
    assert(!gonnaHitWall);
    push((head().r.next(_direction)));
    pop();
    _preDir = _direction;
}

void Snake::change_dir(Dir dir)
{
    if (dir != _preDir && dir != opposite(_preDir))
        _direction = dir;
}

const SnakeNode& Snake::head() const
{
    return *_nodes.front();
}

void Snake::push(const Coordinate& r)
{
    _nodes.emplace_front(new SnakeNode(r));
}

void Snake::pop()
{
    _nodes.pop_back();
}

Coordinate Snake::next_coord() const
{
    return (head().r.next(_direction));
}

void Snake::evolve()
{
    push((head().r.next(_direction)));
}

void Snake::turn_wall()
{
    std::vector<Coordinate> coords;
    for (const auto& node : _nodes)
        coords.emplace_back(node->r);
    _nodes.clear();
    for (const auto& r : coords)
        _canvas.set(r, px_wall);
}

void Snake::turn_food(std::vector<std::shared_ptr<Food>>& foods)
{
    std::vector<Coordinate> coords;
    for (const auto& node : _nodes)
        coords.emplace_back(node->r);
    _nodes.clear();
    for (const auto& r : coords)
        foods.emplace_back(new Food(r));
}

void Snake::reinit()
{
    _nodes.clear();
    init();
}

void Snake::init()
{
    _nodes.emplace_front(new SnakeNode());
    _preDir = _direction = (_nodes.front()->r.x < _canvas.width / 2) ? right : left;
}

Food::Food()
    : _canvas(Canvas::get_instance()),
    value(2),
    r(_canvas.get_blank_coord())
{
    _canvas.set(r, px_food);
}

Food::~Food()
{
    _canvas.set(r, px_blank);
}

Food::Food(const Coordinate& r)
    : _canvas(Canvas::get_instance()),
    value(1),
    r(r)
{
    _canvas.set(r, px_food);
}


SnakeNode::SnakeNode()
    : _canvas(Canvas::get_instance()), r(_canvas.get_blank_coord())
{
    _canvas.set(r, px_snake);
}

SnakeNode::~SnakeNode()
{
    _canvas.set(r, px_blank);
}

SnakeNode::SnakeNode(const Coordinate& r)
    : _canvas(Canvas::get_instance()), r(r)
{
    _canvas.set(r, px_snake);
}

std::map<unsigned int, std::string> Score::levelStrMap {
        { 0, "入门版" },
        { 1, "进阶版" },
        { 2, "高级版" },
        { 3, "放飞版" }
};

Score::Score(unsigned int difficulty)
    : _canvas(Canvas::get_instance()),
    _score(0),
    _length(1),
    _highestScore(0), // Temporarily 0
    levelStr(levelStrMap.at(difficulty)),
    difficulty(difficulty) {}

void Score::print_score() const
{
    _canvas.cursor.abs_move_to({ 0, 0 });
    std::cout << "Score: " << _score << ' '
        << levelStr << " Highest: " << _highestScore << ' '
        << "Length: " << _length;
}

void Score::set_highest(unsigned int hScore)
{
    _highestScore = hScore;
}

Score::~Score()
{
    clear_screen();
    _canvas.cursor.abs_move_to({ 0, 0 });
    std::cout << "Final Score: " << _score << std::endl;
#ifdef _MSC_VER
    std::cout << "Press Any Key to Continue..." << std::endl;
    _getch();
#endif // _MSC_VER
}

void Score::operator +=(unsigned int i)
{
    _score += i;
    _length += 1;
    if (_score > _highestScore)
        _highestScore = _score;
    print_score();
}

void Score::reset_length()
{
    _length = 1;
    print_score();
}

