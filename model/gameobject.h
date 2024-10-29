

#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QEvent>
#include <QPointer>
#include <QVariant>
#include <typeindex>

#include "publicenums.h"
#include "model/behaviors/behavior.h"

/**
 * @brief The GameObject class represents an individual entity within the game world, capable of various interactions and states.
 */
class GameObject : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Constructor for GameObject with initial data.
     * @param objectData A map of data roles to their corresponding values.
     */
    GameObject(QMap<DataRole, QVariant> objectData)
        : m_objectData(objectData) {};

    /**
     * @brief Default constructor for GameObject.
     */
    GameObject() { m_objectData = QMap<DataRole, QVariant>(); };

    /**
     * @brief Destructor for GameObject.
     */
    ~GameObject() {};

    /**
     * @brief Finds a child GameObject of a specified type.
     * @param type The ObjectType to find.
     * @return A pointer to the found GameObject.
     */
    const QPointer<GameObject> findChild(ObjectType type);

    // Data getters and setters.
    /**
     * @brief Gets data for a specific role.
     * @param role The role for which data is requested.
     * @return The data associated with the given role.
     */
    QVariant getData(DataRole role) const;

    /**
     * @brief Gets all data for the GameObject.
     * @return A map of data roles to their values.
     */
    QMap<DataRole, QVariant> getData() const;

    /**
     * @brief Gets all data, optionally including self.
     * @param self Whether to include the GameObject's own data.
     * @return A list of data maps.
     */
    QList<QMap<DataRole, QVariant>> getAllData(bool self = true) const;

    /**
     * @brief Sets data for a specific role.
     * @param role The role to set data for.
     * @param value The value to set.
     */
    void setData(DataRole role, QVariant value);

    /**
     * @brief Sets multiple data roles and values.
     * @param data A list of data role-value pairs.
     */
    void setData(QList<QPair<DataRole, QVariant>> data);

    /**
     * @brief Gets all neighboring GameObjects.
     * @param offset The offset to consider for neighbors.
     * @return A list of pointers to neighboring GameObjects.
     */
    const QList<QPointer<GameObject>> getAllNeighbors(int offset = 0) const;

    /**
     * @brief Gets a neighbor in a specific direction.
     * @param direction The direction in which to find the neighbor.
     * @param offset The offset to consider.
     * @return A pointer to the neighboring GameObject.
     */
    const QPointer<GameObject> getNeighbor(double direction, int offset = 0) const;

    /**
     * @brief Gets a neighbor in a specific direction.
     * @param direction The direction (as enum) in which to find the neighbor.
     * @param offset The offset to consider.
     * @return A pointer to the neighboring GameObject.
     */
    const QPointer<GameObject> getNeighbor(Direction direction, int offset = 0) const;
    /**
     * @brief Sets a behavior for the GameObject.
     * @param behavior The behavior to set.
     */
    template <typename T, typename = std::enable_if<std::is_base_of<Behavior, T>::value>::type>
    void setBehavior(QSharedPointer<T> behavior) {
        m_behaviors[typeid(T)] = behavior;
    }
    /**
     * @brief Gets a behavior of the GameObject.
     * @return A shared pointer to the requested behavior.
     */
    template <typename T, typename = std::enable_if<std::is_base_of<Behavior, T>::value>::type>
    QSharedPointer<T> getBehavior() const {
        auto behavior = m_behaviors[typeid(T)];
        return qSharedPointerDynamicCast<T>(behavior);
    }
    /**
     * @brief Removes a behavior from the GameObject.
     */
    template <typename T, typename = std::enable_if<std::is_base_of<Behavior, T>::value>::type>
    void removeBehavior() {
        m_behaviors.remove(typeid(T));
    }

    /**
     * @brief Gets all behaviors of the GameObject.
     * @return A list of shared pointers to the behaviors.
     */
    template <typename T, typename = std::enable_if<std::is_base_of<Behavior, T>::value>::type>
    const QList<QSharedPointer<T>> getAllBehaviors() const {
        auto list = QList<QSharedPointer<T>>();
        list.append(qSharedPointerDynamicCast<T>(m_behaviors[typeid(T)]));

        for(auto *child : findChildren<GameObject *>()) {
            list.append(qSharedPointerDynamicCast<T>(child->getBehavior<T>()));
        }
        return list;
    };

    /**
     * @brief Event handler override.
     * @param event The event to handle.
     * @return True if event was handled.
     */
    bool event(QEvent *event) override;

    /**
     * @brief Sets the GameObject's data.
     * @param data The data to set.
     */
    void setData(const QMap<DataRole, QVariant> &data);

    /**
     * @brief Equality comparison operator.
     * @param obj The GameObject to compare with.
     * @return True if equal.
     */
    bool operator==(GameObject const &obj) const {
        return m_objectData[DataRole::Type].value<ObjectType>() == obj;
    }

    /**
     * @brief Equality comparison operator.
     * @param type The ObjectType to compare with.
     * @return True if equal.
     */

    bool operator==(ObjectType const &type) const {
        return m_objectData[DataRole::Type].value<ObjectType>() == type;
    }

    /**
     * @brief Checks if the GameObject has a child of a specific type.
     * @param type The ObjectType to check for.
     * @return True if such a child exists.
     */
    bool hasChild(ObjectType type) const;

    /**
     * @brief Checks if the GameObject has a child within a range of types.
     * @param range The range of ObjectTypes to check for.
     * @return True if such a child exists.
     */
    bool hasChild(QPair<ObjectType, ObjectType> range) const;

    /**
     * @brief Finds the nearest GameObject of a specific type or within a range.
     * @param range The range of ObjectTypes to search for.
     * @return A pointer to the nearest GameObject.
     */
    const GameObject *nearest(QPair<ObjectType, ObjectType> range) const;

    /**
     * @brief Finds the nearest GameObject of a specific type.
     * @param type The ObjectType to search for.
     * @return A pointer to the nearest GameObject.
     */
    const GameObject *nearest(ObjectType type) const;

    /**
     * @brief Finds a child GameObject within a range of types.
     * @param range The range of ObjectTypes to search for.
     * @return A pointer to the found GameObject.
     */
    const QPointer<GameObject> findChild(QPair<ObjectType, ObjectType> range);

private:
    /**
     * @brief m_behaviors Map of Behavior std::type_index to their shared pointers.
     */
    QMap<std::type_index, QSharedPointer<Behavior>> m_behaviors;
    /**
     * @brief m_behaviors Map of DataRole to their values QVariant.
     */
    QMap<DataRole, QVariant> m_objectData;

signals:
    /**
     * @brief Signal emitted when the data of the GameObject changes.
     * @param objectData The changed data of the GameObject.
     */
    void dataChanged(QMap<DataRole, QVariant> objectData);
    /**
     * @brief Signal emitted to indicate a game tick.
     */
    void tick();
};

#endif // GAMEOBJECT_H
