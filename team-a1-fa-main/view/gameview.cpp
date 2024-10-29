#include "gameview.h"
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>

GameView::GameView(QObject *parent)
    : QGraphicsScene(parent) {
    // Initialize the m_tiles with empty shared pointers
}

void GameView::createScene(
  const QList<QList<QList<QMap<DataRole, QVariant>>>> &gameObjects,
  QSharedPointer<Renderer> renderer) {
    if(renderer) {
        setRenderer(renderer);
    }

    if(!m_tiles.empty()) {
        clear();
    }

    m_tiles = QList<QList<GamePixmapItem *>>(gameObjects.size());

    for(int x = 0; x < gameObjects.size(); ++x) {
        QList<GamePixmapItem *> rowItems(gameObjects[0].size());
        m_tiles[x] = (rowItems);
        for(int y = 0; y < gameObjects[0].size(); ++y) {
            if(x < gameObjects.size() && y < gameObjects[x].size() && !gameObjects[x][y].empty()) {
                auto *item = m_renderer->renderGameObjects(gameObjects[x][y]);
                item->setPos(x * item->pixmap().width(), y * item->pixmap().height());
                m_tiles[x][y] = item; // Store the shared pointer in m_tiles
                addItem(item);
            }
        }
    }
}

void GameView::setRenderer(QSharedPointer<Renderer> newRenderer) {
    m_renderer = std::move(newRenderer);
}

GamePixmapItem *GameView::getPixmapItem(int x, int y, QVariant type) {
    auto tile = m_tiles[x][y];
    switch(type.value<ObjectType>()) {
    case ObjectType::Tile:
        return tile;
    default:
        for(auto child : tile->childItems()) {
            if(child->data((int)DataRole::Type) == type) {
                return dynamic_cast<GamePixmapItem *>(child);
            }
        }
        return nullptr;
    }
}

void GameView::dataChanged(QMap<DataRole, QVariant> objectData) {
    auto position = objectData[DataRole::Position].toPoint();
    // The changes made here are only because the renderers have no access to the world.
    if(objectData[DataRole::LatestChange].value<DataRole>() == DataRole::Position) {
        // The direction tells us where the object moved to. Since the data stored in it
        // has the new position, we want to get the GamePixmapItem that is located in the tile
        // opposite to the movement direction.
        double direction = objectData[DataRole::Direction].toDouble();
        double angleRad = -direction * M_PI / 180.0;

        // Simple math, the angle is negative since (0,0) on an array is inverted wrt the
        // QGraphicsScene the scene fills up to down while the model fills down to up.
        int x = position.x() - round(cos(angleRad));
        int y = position.y() - round(sin(angleRad));

        // Perhaps an ID would be better in this case.
        auto changedObject = getPixmapItem(x, y, objectData[DataRole::Type]);

        QVariant type = QVariant::fromValue<ObjectType>(ObjectType::Tile);
        changedObject->setParentItem(getPixmapItem(position.x(), position.y(), type));

    } else if(objectData[DataRole::Destroyed].toBool()) {
        // This removes the object from the scene.
        delete getPixmapItem(position.x(), position.y(), objectData[DataRole::Type]);
    } else {
        // For every other change we pass it to the renderer.
        auto *obj = getPixmapItem(position.x(), position.y(), objectData[DataRole::Type]);
        m_renderer->renderGameObject(objectData, obj);
    }
}
