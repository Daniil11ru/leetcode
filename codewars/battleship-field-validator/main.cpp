#include <vector>
#include <map>
#include <optional>
#include <functional>
#include <iostream>

const int ROW_COUNT = 10;
const int COLUMN_COUNT = 10;

const int BATTLESHIP_COUNT = 1;
const int CRUISER_COUNT = 2;
const int DESTROYER_COUNT = 3;
const int SUBMARINE_COUNT = 4;

const int BATTLESHIP_SIZE = 4;
const int CRUISER_SIZE = 3;
const int DESTROYER_SIZE = 2;
const int SUBMARINE_SIZE = 1;

enum class ShipOrientation
{
    UNDEFINED,
    HORIZONTAL,
    VERTICAL
};

class Coordinate
{
private:
    const int x_;
    const int y_;

public:
    Coordinate(const int x, const int y) : x_(x), y_(y) {}
    Coordinate &operator=(const Coordinate &other)
    {
        return *this;
    }
    friend bool operator==(const Coordinate &lhs, const Coordinate &rhs)
    {
        return lhs.x() == rhs.x() && lhs.y() == rhs.y();
    }
    const int x() const
    {
        return x_;
    }
    const int y() const
    {
        return y_;
    }
};

class Ship
{
private:
    const Coordinate start_;
    int size_;
    ShipOrientation orientation_;

public:
    Ship(const Coordinate &start) : start_(start), size_(SUBMARINE_SIZE), orientation_(ShipOrientation::UNDEFINED) {}
    const Coordinate &start() const
    {
        return start_;
    }
    const int size() const
    {
        return size_;
    }
    const ShipOrientation orientation() const
    {
        return orientation_;
    }
    const bool contains(const Coordinate &coordinate) const
    {
        if (orientation_ == ShipOrientation::UNDEFINED && start_ == coordinate)
        {
            return true;
        }

        if (start_.y() == coordinate.y() && orientation_ == ShipOrientation::HORIZONTAL)
        {
            int start = start_.x();
            int end = start + size_ - 1;
            if (coordinate.x() >= start && coordinate.x() <= end)
            {
                return true;
            }
        }

        if (start_.x() == coordinate.x() && orientation_ == ShipOrientation::VERTICAL)
        {
            int start = start_.y();
            int end = start + size_ - 1;
            if (coordinate.y() >= start && coordinate.y() <= end)
            {
                return true;
            }
        }

        return false;
    }
    void increaseSize()
    {
        ++size_;
    }
    void setOrientation(const ShipOrientation orientation)
    {
        orientation_ = orientation;
    }
};

class Fleet
{
private:
    std::vector<Ship> ships_;

public:
    Fleet() {}
    void add(const Ship &ship)
    {
        ships_.push_back(ship);
    }
    const std::vector<Ship> &ships() const
    {
        return ships_;
    }
    std::optional<std::reference_wrapper<Ship>> ship(const Coordinate &coordinate)
    {
        for (auto &&ship : ships_)
        {
            if (ship.contains(coordinate))
            {
                return ship;
            }
        }
        return std::nullopt;
    }
};

bool validateBattlefield(std::vector<std::vector<int>> field)
{
    Fleet fleet;

    for (int rowIndex = 0; rowIndex < ROW_COUNT; ++rowIndex)
    {
        for (int columnIndex = 0; columnIndex < COLUMN_COUNT; ++columnIndex)
        {
            if (field[rowIndex][columnIndex] != 1)
            {
                continue;
            }

            const bool hasTopLeft = (columnIndex > 0) && (rowIndex > 0) && (field[rowIndex - 1][columnIndex - 1] == 1);
            const bool hasTopRight = (columnIndex < (COLUMN_COUNT - 1)) && (rowIndex > 0) && (field[rowIndex - 1][columnIndex + 1] == 1);

            if (hasTopLeft || hasTopRight)
            {
                return false;
            }

            const bool hasTop = (rowIndex > 0) && (field[rowIndex - 1][columnIndex] == 1);
            const bool hasLeft = (columnIndex > 0) && (field[rowIndex][columnIndex - 1] == 1);

            if (hasTop && hasLeft)
            {
                return false;
            }

            if (!hasTop && !hasLeft)
            {
                fleet.add(Ship(Coordinate(columnIndex, rowIndex)));
                continue;
            }

            const bool horizontal = hasLeft;

            const Coordinate neighborCoordinate = horizontal
                                                      ? Coordinate(columnIndex - 1, rowIndex)
                                                      : Coordinate(columnIndex, rowIndex - 1);

            Ship &ship = fleet.ship(neighborCoordinate).value().get();
            ship.setOrientation(horizontal ? ShipOrientation::HORIZONTAL
                                           : ShipOrientation::VERTICAL);
            ship.increaseSize();

            if (ship.size() > BATTLESHIP_SIZE)
            {
                return false;
            }
        }
    }

    int battleshipCount = 0, cruiserCount = 0, destroyerCount = 0, submarineCount = 0;

    for (auto &&ship : fleet.ships())
    {
        switch (ship.size())
        {
        case BATTLESHIP_SIZE:
            ++battleshipCount;
            break;
        case CRUISER_SIZE:
            ++cruiserCount;
            break;
        case DESTROYER_SIZE:
            ++destroyerCount;
            break;
        case SUBMARINE_SIZE:
            ++submarineCount;
            break;
        default:
            return false;
        }
    }

    std::cout << battleshipCount << std::endl
              << cruiserCount << std::endl
              << destroyerCount << std::endl
              << submarineCount << std::endl;

    if (battleshipCount == BATTLESHIP_COUNT && cruiserCount == CRUISER_COUNT && destroyerCount == DESTROYER_COUNT && submarineCount == SUBMARINE_COUNT)
    {
        return true;
    }

    return false;
}

int main()
{
    std::vector<std::vector<int>> battlefield = {
        std::vector<int>{1, 0, 0, 0, 0, 1, 1, 0, 0, 0},
        std::vector<int>{1, 0, 1, 0, 0, 0, 0, 0, 1, 0},
        std::vector<int>{1, 0, 1, 0, 1, 1, 1, 0, 1, 0},
        std::vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        std::vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        std::vector<int>{0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
        std::vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        std::vector<int>{0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        std::vector<int>{0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
        std::vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    std::cout << validateBattlefield(battlefield);
}