#pragma once
#ifndef GAMEOBJECTMODEL_H
#define GAMEOBJECTMODEL_H

#include "gameobject.h"
#include <QPoint>

/**
 * @brief The GameObjectModel class represents the model of the game world.
 * It holds a grid of GameObject pointers, representing the game world's layout.
 * It connects all of its child objects to a signal. This makes it very convinient to connect
 * and disconnect levels as they change throughout the game since the GameObjects can be simply
 * connected to their parent GameObject, which is then connected to the GameObjectModel
 * for that level, and then it itself is connected to the view/controller to handle
 * the changes in data as needed. As soon as the level is no longer needed a single
 * disconnect has to be done. This does not cause any overhead since the ticks
 * (which work the same way on the opposite direction) are also disconnected.
 */
class GameObjectModel : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Constructor for GameObjectModel.
     * @param world A 2D grid of QPointer to GameObjects representing the game world.
     */
    GameObjectModel(QList<QList<QPointer<GameObject>>> world) {
        m_world = world;
        for(const auto &row : m_world) {
            for(const auto &tile : row) {
                tile->setParent(this);
                connect(tile, &GameObject::dataChanged, this, &GameObjectModel::dataChanged);
                for(const auto &obj : tile->children()) {
                    auto *gameObj = qobject_cast<GameObject *>(obj);
                    connect(gameObj, &GameObject::dataChanged, this, &GameObjectModel::dataChanged);
                    connect(this, &GameObjectModel::tick, gameObj, &GameObject::tick);
                }
            }
        }
    };

    /**
     * @brief Retrieves a behavior attached to a specific GameObject in the world.
     * @tparam T The Behavior type.
     * @param x The x-coordinate in the world grid.
     * @param y The y-coordinate in the world grid.
     * @param type The type of GameObject.
     * @return A shared pointer to the requested Behavior.
     */
    template <typename T, typename std::enable_if<std::is_base_of<Behavior, T>::value>::type>
    const QSharedPointer<T> &getBehavior(int x, int y, ObjectType type) const;

    /**
     * @brief Sets a behavior for a specific GameObject in the world.
     * @tparam T The Behavior type.
     * @param x The x-coordinate in the world grid.
     * @param y The y-coordinate in the world grid.
     * @param type The type of GameObject.
     * @param behavior A shared pointer to the Behavior to be set.
     * @return True if behavior is successfully set, false otherwise.
     */
    template <typename T, typename std::enable_if<std::is_base_of<Behavior, T>::value>::type>
    bool setBehavior(int x, int y, ObjectType type, QSharedPointer<T> behavior);

    /**
     * @brief Retrieves a GameObject of a specific type from a given location.
     * @param x The x-coordinate in the world grid.
     * @param y The y-coordinate in the world grid.
     * @param type The type of GameObject.
     * @return A pointer to the requested GameObject.
     */
    QPointer<GameObject> getObject(int x, int y, ObjectType type) const;

    /**
     * @brief Sets a GameObject at a specific location in the world.
     * @param x The x-coordinate.
     * @param y The y-coordinate.
     * @param object The GameObject to be placed at the specified location.
     */
    void setItem(int x, int y, QPointer<GameObject> object);

    /**
     * @brief Gets the row count of the game world.
     * @return The number of rows in the world grid.
     */
    int getRowCount() const;

    /**
     * @brief Gets the column count of the game world.
     * @return The number of columns in the world grid.
     */
    int getColumnCount() const;

    /**
     * @brief Gets data for all GameObjects in the world.
     * @param [unused] An unused parameter.
     * @return A 2D list of data maps for each GameObject.
     */
    QList<QList<QMap<DataRole, QVariant>>> getAllData(bool) const;

    /**
     * @brief Gets data for all GameObjects in the world.
     * @return A 3D list containing data maps for each GameObject.
     */
    QList<QList<QList<QMap<DataRole, QVariant>>>> getAllData() const;

    /**
     * @brief Retrieves all GameObjects of a specific type.
     * @param type The type of GameObject.
     * @return A list of pointers to the requested GameObjects.
     */
    QList<QPointer<GameObject>> getObject(ObjectType type) const;
    /**
     * @brief getNeighbor Retrieves all neighbors of a tile under specific parameters
     * @param location Location of the tile we want the neighbors of
     * @param direction Direction in which we want the neighbors
     * @param offset How further are we getting the neighbors, offset 0 = only closest immediate neighbors
     * @return the neighbor to the location.
     */
    const QPointer<GameObject> getNeighbor(QPoint location, double direction, int offset) const;

private:
    /**
     * @brief m_world The game world represented as a 2D list of game objects.
     */
    QList<QList<QPointer<GameObject>>> m_world;

signals:
    /**
     * @brief Signal emitted when the data of a game object changes.
     * @param objectData The changed data of the game object.
     */
    void dataChanged(QMap<DataRole, QVariant> objectData);

    /**
     * @brief Signal emitted for each game tick.
     */
    void tick();
};

#endif // GAMEOBJECTMODEL_H
