#ifndef SNAKYSNAKES_CANVAS_HPP
#define SNAKYSNAKES_CANVAS_HPP

#include <cstddef>
#include <map>
#include <string>
#include <vector>
#include <exception>
#include "graphic-tools.hpp"

enum PxType
{
    px_blank, px_wall, px_snake, px_food
};

struct Coordinate
{
    size_t x, y;
    Coordinate(size_t x, size_t y);
    Coordinate next(Dir dir) const;
    bool operator ==(const Coordinate& r) const;
    Coordinate operator =(const Coordinate& r);
};


// singleton
class Canvas {
    static const std::map<PxType, Color> colorMap;
    std::vector<std::vector<PxType>> _pxs;

    static void print_blocks(size_t times, PxType type);
    Canvas(const size_t height, const size_t width);
public:
    Cursor cursor;
    const size_t height, width;
    bool is_blank(size_t x, size_t y);
    bool is_food(const Coordinate& r);
    bool is_wall(const Coordinate& r);
    bool is_snake(const Coordinate& r);
    Coordinate get_blank_coord();
    Coordinate get_first_blank();
    static Canvas& get_instance();
    void set(const Coordinate& r, PxType type);
    ~Canvas();
    Canvas(Canvas&) = delete;
    Canvas& operator=(Canvas&) = delete;
};

class NoBlankSpace : public std::exception
{
public:
    const char* what() const noexcept
    {
        return "Not enough space left.";
    }
    ~NoBlankSpace() = default;
};


#endif // SNAKYSNAKES_CANVAS_HPP

