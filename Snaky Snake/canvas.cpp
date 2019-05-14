#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <cassert>
#include "canvas.hpp"
#include "graphic-tools.hpp"

const std::map<PxType, Color> Canvas::colorMap {
    { px_blank, color_null },
    { px_snake, blue },
    { px_wall, white },
    { px_food, red }
};

void Canvas::print_blocks(size_t times, PxType type)
{
    Color color = colorMap.at(type);
    if (color != color_null)
        std::cout << color;
    for (size_t i = 0; i < times; ++i)
        std::cout << "  " << std::flush;
    if (color != color_null)
        std::cout << color_null;
}

Canvas::Canvas(const size_t height, const size_t width)
    : _pxs(std::vector<std::vector<PxType>>(height, std::vector<PxType>(width, px_blank))),
        cursor{}, height(height), width(width)
{
    // Drawing begins here
    clear_screen();
    std::cout << std::endl; // first line on screen holds for display of scores
    print_blocks(width + 2, px_wall); // first row
    std::cout.put('\n');
    for (size_t i = 0; i < height; ++i)
    {
        print_blocks(1, px_wall);
        print_blocks(width, px_blank);
        print_blocks(1, px_wall);
        std::cout.put('\n');
    }
    print_blocks(width + 2, px_wall); // last row
    cursor.abs_move_to({ 0, 0 });
}

Canvas::~Canvas() {}

void Canvas::set(const Coordinate& r, PxType type)
{
    _pxs.at(r.y).at(r.x) = type;
    cursor.move_to(r);
    print_blocks(1, type);
}

Canvas& Canvas::get_instance()
{
    const size_t height(18), width(32);
    static Canvas canvas(height, width);
    return canvas;
}

bool Canvas::is_blank(size_t x, size_t y)
{

    return (!is_wall({ x, y }) &&_pxs.at(y).at(x) == px_blank);
}

bool Canvas::is_food(const Coordinate& r)
{
    return (!is_wall(r) && _pxs.at(r.y).at(r.x) == px_food);
}

bool Canvas::is_wall(const Coordinate& r)
{
    // size_t type variables won't be negative
    return (
        r.x >= width || r.y >= height ||
        _pxs.at(r.y).at(r.x) == px_wall
    );
}

bool Canvas::is_snake(const Coordinate& r)
{
    return (!is_wall(r) && _pxs.at(r.y).at(r.x) == px_snake);
}

Coordinate::Coordinate(size_t x, size_t y)
    : x(x), y(y)
{

}

Coordinate Canvas::get_blank_coord()
{
    std::random_device rdDevice;
    // static std::default_random_engine rdEngine(rdDevice);
    static std::uniform_int_distribution<size_t> rdHeight(0, height - 1);
    static std::uniform_int_distribution<size_t> rdWidth(0, width - 1);
    size_t x, y;
    size_t cnt(0);
    const size_t MAX_CNT(200);
    do
    {
        x = rdWidth(rdDevice);
        y = rdHeight(rdDevice);
        ++cnt;
    } while (!is_blank(x, y) && cnt < MAX_CNT);
    return (cnt < MAX_CNT) ? Coordinate{x, y} : get_first_blank();
}

Coordinate Canvas::get_first_blank()
{
    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            if (is_blank(x, y))
                return {x, y};
        }
    }
    throw NoBlankSpace();
}

Coordinate Coordinate::next(Dir dir) const
{
    bool isDirection(is_direction(dir));
    assert(isDirection);
    switch (dir)
    {
        case up:
            return { x, y - 1 };
        case down:
            return { x, y + 1 };
        case left:
            return { x - 1, y };
        case right:
            return { x + 1, y };
        default:
            throw std::invalid_argument("dir is not a direction");
    }
}

bool Coordinate::operator==(const Coordinate& r) const
{
    return (x == r.x && y == r.y);
}

Coordinate Coordinate::operator=(const Coordinate& r)
{
    x = r.x;
    y = r.y;
    return *this;
}

