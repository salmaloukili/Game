#include "gameobjectmodel.h"
#include <QTransform>
#include <math.h>
int GameObjectModel::getRowCount() const {
    return m_world[0].size();
}

int GameObjectModel::getColumnCount() const {
    return m_world.size();
}

const QPointer<GameObject> GameObjectModel::getNeighbor(QPoint location, double direction, int offset) const {
    // The other method of using simple trig was causing some issues with big offsets.
    // This might be somewhat slower but works quite well. The whole game is based on directions relative to
    // the GameObjects since this makes behaviors very easy to program and fits well with the way objects are displayed.
    qreal qoffset = static_cast<qreal>(offset + 1);

    QTransform tf;
    // We make a line with length 2*qoffset^2, since calculating the square root is slow and pointless.
    // It just has to be longer than the biggest diagonal in a square.
    auto line = tf.rotate(-direction).map(QLineF({0, 0}, {2 * qoffset * qoffset, 0}));

    QPointF pointF;
    QLineF bounds {qoffset, -qoffset, qoffset, qoffset};
    // We loop and check for an intersection with a line. The line is rotated until an intersection is found.
    while(line.intersects(bounds, &pointF) != QLineF::BoundedIntersection) {
        tf.reset();
        bounds = tf.rotate(90).map(bounds);
    }

    // This intersection is the grid location relative to the object.
    auto point = pointF.toPoint();
    int x = location.x() + point.x();
    int y = location.y() + point.y();

    // No tile access allowed in the void.
    if(0 > x || 0 > y || x >= getColumnCount() || y >= getRowCount()) {
        return QPointer<GameObject>(nullptr);
    }
    return m_world.at(x).at(y);
}

QList<QList<QMap<DataRole, QVariant>>> GameObjectModel::getAllData(bool) const {
    QList<QList<QMap<DataRole, QVariant>>> list;
    for(int x = 0; x < getColumnCount(); ++x) {
        list.append(QList<QMap<DataRole, QVariant>>());
        for(int y = 0; y < getRowCount(); ++y) {
            list[x].append(m_world[x][y]->getData());
        }
    }
    return list;
}

QList<QList<QList<QMap<DataRole, QVariant>>>> GameObjectModel::getAllData() const {
    // Probably linear time comp. makes a lot of copies so might be a bit slow with huge worlds.
    QList<QList<QList<QMap<DataRole, QVariant>>>> list;
    for(int x = 0; x < getColumnCount(); ++x) {
        list.append(QList<QList<QMap<DataRole, QVariant>>>());
        for(int y = 0; y < getRowCount(); ++y) {
            list[x].append(m_world[x][y]->getAllData());
        }
    }
    return list;
}

QPointer<GameObject> GameObjectModel::getObject(int x, int y, ObjectType type) const {
    if(y > getRowCount() || x > getColumnCount()) {
        return QPointer<GameObject>(nullptr);
    }

    auto tile = m_world[x][y];
    if(type == ObjectType::Tile) {
        return tile;
    }

    return tile->findChild(type);
}

QList<QPointer<GameObject>> GameObjectModel::getObject(ObjectType type) const {
    QList<QPointer<GameObject>> list {};
    for(int x = 0; x < getColumnCount(); ++x) {
        for(int y = 0; y < getRowCount(); ++y) {
            if(auto tile = getObject(x, y, type)) {
                list.append(tile);
            }
        }
    }
    return list;
}

void GameObjectModel::setItem(int x, int y, QPointer<GameObject> object) {
    if(y > getRowCount() || x > getColumnCount()) {
        throw "Cannot set outside range";
    }

    if(object->getData(DataRole::Type).value<ObjectType>() == ObjectType::Tile) {
        delete m_world[x][y];
        m_world[x][y] = object;
        return;
    }

    object->setParent(m_world[x][y]);
}
