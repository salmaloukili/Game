#ifndef NODE_H
#define NODE_H

#include <QMap>
#include <QVariant>

#include "world.h"
#include "publicenums.h"

/**
 * @brief The Node class wraps the tiles from the World Library in order to make them compatible with the pathfinder.
 */
struct Node : public Tile {
    /**
     * @brief Node empty constructor.
     */
    Node()
        : Tile(0, 0, 0) {};
    /**
     * @brief Node Constructor.
     * @param x The X position of the tile.
     * @param y The Y position of the tile.
     * @param val The energy level of the tile.
     */
    Node(int x, int y, float val)
        : Tile(x, y, val) {};
    /**
     * @brief Node Tile copy constructor.
     * @param t the tile to copy.
     */
    Node(const Tile &t)
        : Tile(t) {};
    /**
     * @brief Node copy constructor.
     * @param n the node to copy.
     */
    Node(const Node &n)
        : Tile(n.getXPos(), n.getYPos(), n.getValue())
        , f(n.f)
        , g(n.g)
        , h(n.h)
        , visited(n.visited)
        , prev(n.prev) {};
    /**
     * @brief Node GameObject data constructor.
     * @param obj The object data to copy.
     */
    Node(const QMap<DataRole, QVariant> &obj)
        : Tile(obj[DataRole::Position].toPoint().x(),
               obj[DataRole::Position].toPoint().y(),
               obj.contains(DataRole::Energy) ? obj[DataRole::Energy].toFloat() : 0) {
    }
    virtual ~Node() {};
    /**
     * @brief operator = The assignment overload for Nodes.
     * @param n the node on the left.
     * @return New node.
     */
    Node &operator=(const Node &n) {
        if(this != &n) {
            Tile::operator=(n);
            f = n.f;
            g = n.g;
            h = n.h;
            visited = n.visited;
            prev = n.prev;
        }
        return *this;
    }
    ///@{
    /**
     * @brief (f, g, h) the weights for the model.
     */
    float f = 0.0f, g = 0.0f, h = 0.0f;
    ///@}
    /**
     * @brief visited if this Node has been visited.
     */
    bool visited = false;
    /**
     * @brief prev pointer to the previous Node.
     */
    Node *prev = nullptr;
};

#endif // NODE_H
