#include "gameobject.h"
#include "gameobjectmodel.h"

#include <QChar>
#include <iostream>
#include <ranges>
#include <algorithm>

const QPointer<GameObject> GameObject::getNeighbor(double direction, int offset) const {
    // Overloaded function that takes an angle as its parameter. This function
    // propagates itself by calling its parent until it is not a GameObject.
    if(auto prt = qobject_cast<GameObject *>(parent())) {
        return prt->getNeighbor(direction, offset);
    }
    // The parent of the last GameObject parent is a GameObject model, so it asks it for its neighbor, neighbor.
    return qobject_cast<GameObjectModel *>(parent())->getNeighbor(m_objectData[DataRole::Position].toPoint(), direction, offset);
}
const QPointer<GameObject> GameObject::getNeighbor(Direction direction, int offset) const {
    // Kept for laziness reasons.
    return getNeighbor(static_cast<double>(direction), offset);
}
const QList<QPointer<GameObject>> GameObject::getAllNeighbors(int offset) const {
    // I think this ended up looking pretty sweet.
    auto list = QList<QPointer<GameObject>>();

    // Quick reminder that int do not magically cast to doubles. The function
    // accesses neighbors using angles, by dividing 360 deg by the offset
    // * 8 we get a geometric increase on the number of tiles.
    // This might? not work for big offsets, too afraid to try.
    for(double i = 0; i < 360; i += 360 / (((double)offset + 1) * 8)) {
        list.append(getNeighbor(i, offset));
    }
    return list;
}

bool GameObject::event(QEvent *event) {
    if(event->type() == QEvent::ParentChange) {
        auto data = getData();
        // Pass on the data from the parent to the view, the position is the new position after the parent change.
        // This means that the change direction + the new position form a vector of the movement of the object.
        data[DataRole::Position] = qobject_cast<GameObject *>(parent())->getData(DataRole::Position);
        data[DataRole::LatestChange] = QVariant::fromValue<DataRole>(DataRole::Position);
        data[DataRole::ChangeDirection] = getData(DataRole::Direction);

        // Debug for protagonist only
        if(data[DataRole::Type].value<ObjectType>() == ObjectType::Protagonist) {
            qDebug() << data[DataRole::Type].toString() << "Moved To: (" << data[DataRole::Position].toPoint().x() << ", "
                     << data[DataRole::Position].toPoint().y() << ")" << data[DataRole::ChangeDirection].toString();
        }
        emit dataChanged(data);
        return true;
    }

    return QObject::event(event);
}

void GameObject::setData(DataRole role, QVariant value) {
    // Directions are not only angles in a plane, but can also be interpreted as directions of change.
    Direction dir = value.toFloat() > m_objectData[role].toFloat() ? Direction::Up : Direction::Down;
    m_objectData[role] = value;

    if(m_objectData[DataRole::Type].value<ObjectType>() == ObjectType::Protagonist) {
        qDebug() << m_objectData[DataRole::Type].toString() << "Data Changed: " << QVariant::fromValue<DataRole>(role).toString()
                 << " : " << m_objectData[role].toFloat() << ":" << QVariant::fromValue<Direction>(dir).toString();
    }

    auto data = getData();
    if(parent() && parent()->inherits("GameObject")) {
        // Tiles do not have a parent that has a position.
        data[DataRole::Position] = qobject_cast<GameObject *>(parent())->getData(DataRole::Position);
    }
    // These data roles are never stored in the GameObjects
    data[DataRole::LatestChange] = QVariant::fromValue<DataRole>(role);
    data[DataRole::ChangeDirection] = QVariant::fromValue<Direction>(dir);

    emit dataChanged(data);
}

void GameObject::setData(QList<QPair<DataRole, QVariant>> data) {
    for(const auto &pair : data) {
        m_objectData[pair.first] = pair.second;
    }
}

void GameObject::setData(const QMap<DataRole, QVariant> &data) {
    m_objectData = QMap<DataRole, QVariant>(data);
}

const QPointer<GameObject> GameObject::findChild(ObjectType type) {
    auto children = findChildren<GameObject *>();
    for(auto child : children) {
        if(child->getData(DataRole::Type).value<ObjectType>() == type) {
            return child;
        }
    }
    return nullptr;
}
const QPointer<GameObject> GameObject::findChild(QPair<ObjectType, ObjectType> range) {
    // Object type can have ranges to find several objects related to eachother.
    auto children = findChildren<GameObject *>();
    for(auto child : children) {
        int type = child->getData(DataRole::Type).toInt();
        if(type >= (int)range.first && type <= (int)range.second) {
            return child;
        }
    }
    return nullptr;
}

bool GameObject::hasChild(QPair<ObjectType, ObjectType> range) const {
    auto children = getAllData(false);

    return std::any_of(children.begin(), children.end(), [&range](const QMap<DataRole, QVariant> &obj) {
        int type = obj[DataRole::Type].toInt();
        return type >= (int)range.first && type <= (int)range.second;
    });
}

bool GameObject::hasChild(ObjectType type) const {
    return hasChild({type, type});
}

const GameObject *GameObject::nearest(ObjectType type) const {
    return nearest({type, type});
}

const GameObject *GameObject::nearest(QPair<ObjectType, ObjectType> range) const {
    auto filter = [&range](QPointer<GameObject> obj) { return obj.isNull() ? false : obj->hasChild(range); };
    auto comp = [](QPointer<GameObject> a) { return a.isNull(); };
    int i = 0;

    while(true) {
        auto tiles = getAllNeighbors(i);
        if(std::all_of(tiles.begin(), tiles.end(), comp)) {
            // When the map has been exhausted do not loop any more.
            return nullptr;
        }
        auto it = std::find_if(tiles.begin(), tiles.end(), filter);
        if(it != tiles.end()) {
            return *it;
        }

        i++;
    };
}

QVariant GameObject::getData(DataRole role) const {
    return m_objectData[role];
}

QMap<DataRole, QVariant> GameObject::getData() const {
    return m_objectData;
}

QList<QMap<DataRole, QVariant>> GameObject::getAllData(bool self) const {
    QList<QMap<DataRole, QVariant>> list;
    if(self) {
        list.append(getData());
    }

    for(auto child : children()) {
        list.append(qobject_cast<GameObject *>(child)->getData());
    }
    return list;
}
