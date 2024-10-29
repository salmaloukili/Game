#ifndef PUBLICENUMS_H
#define PUBLICENUMS_H
#include <QObject>
namespace _pub {
    Q_NAMESPACE
    enum class ObjectType : char {
        Overlay = -1,

        Tile = 0,

        Doorway = 1,
        HealthPack = 2,

        Protagonist = 50,

        _ENEMIES_START = 99,
        Enemy = 100,
        PoisonEnemy = 101,
        MovingEnemy = 102,
        _ENEMIES_END,
    };
    Q_ENUM_NS(ObjectType);

    enum class DataRole : int {
        Type,

        Health,
        Energy,
        Strength,

        PoisonLevel,
        FireLevel,

        Destroyed,

        Position,
        Direction,

        LatestChange,
        ChangeDirection,

        Path,
    };
    Q_ENUM_NS(DataRole);

    enum class Direction : int {
        Right = 0,
        TopRight = 45,
        Up = 90,
        TopLeft = 135,
        Left = 180,
        BottomLeft = 225,
        Down = 270,
        BottomRight = 315,
    };
    Q_ENUM_NS(Direction);

    const Direction EnumDirectionVector[] = {
      Direction::TopLeft,
      Direction::Up,
      Direction::TopRight,
      Direction::Left,
      Direction::Right,
      Direction::BottomLeft,
      Direction::Down,
      Direction::BottomRight,
    };
}
using namespace _pub;
#endif // PUBLICENUMS_H
