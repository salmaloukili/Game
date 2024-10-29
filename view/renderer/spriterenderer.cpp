#include "spriterenderer.h"
#include <QPainter>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QTimer>
#include <iostream>

GamePixmapItem *SpriteRenderer::renderGameObject(QMap<DataRole, QVariant> data) {
    // Create the item
    auto *item = Renderer::renderGameObject(data);

    ObjectType type = data[DataRole::Type].value<ObjectType>();
    switch(type) {
    case ObjectType::Tile:
        // We could also save the whole tile sprite since
        // images are shared data in Qt. I'm more partial to this.
        item->setSprite(m_tiles.copy(getTileRect(data)));
        item->updatePixmap();
        return item;
    case ObjectType::Doorway:
        // When the frame dimension is not set it
        item->setSprite(QImage(":/images/doorway.png"));
        item->updatePixmap();
        return item;
    case ObjectType::HealthPack:
        item->setSprite(QImage(":/images/health_pack.png"));
        item->updatePixmap();
        return item;
    default:
        item->setSprite(m_characters.copy(getCharacterRect(type)));
        item->setFrameDimension(m_charSize);
        item->setFrame({calculateFrame(data[DataRole::Direction], m_charMap[type].alive.x()),
                        item->frame().y()});
        item->updatePixmap();
        item->addAnimation(Renderer::animateBounce());

        if(type == ObjectType::MovingEnemy) {
            item->addAnimation(Renderer::animateHide());
        }
        return item;
    }
}

void SpriteRenderer::renderGameObject(QMap<DataRole, QVariant> data, GamePixmapItem *item) {
    DataRole change = data[DataRole::LatestChange].value<DataRole>();
    ObjectType type = data[DataRole::Type].value<ObjectType>();
    // Animates every time the data changes. They all run in parallel so they don't affect eachother.
    switch(change) {
    case DataRole::Direction:
        item->setFrame({calculateFrame(data[DataRole::Direction], m_charMap[type].alive.x()),
                        item->frame().y()});
        // So they look like ghosts
        if(data[DataRole::Type].value<ObjectType>() == ObjectType::MovingEnemy) {
            item->animationGroup()->clear();
            item->addAnimation(animateHide());
        }
        break;
    case DataRole::Health:
        if((int)type > 49) {
            if(!data[DataRole::Health].toInt()) {
                item->animationGroup()->clear();
                item->addAnimation(animateDeath(m_charMap[item->data((int)DataRole::Type).value<ObjectType>()].dead));
                item->setTint({0, 0, 0, 0});
                return;
            }
        }
        break;
    default:
        break;
    }
    Renderer::renderGameObject(data, item);
}

QRect SpriteRenderer::getTileRect(QMap<DataRole, QVariant> data) {
    float energyLevel = round(data[DataRole::Energy].toFloat() * 19);
    int tile = energyLevel == INFINITY ? TILE_COUNT - 1 : energyLevel;
    return {tile * m_tileSize.width(), 0, m_tileSize.width(), m_tileSize.height()};
}

QRect SpriteRenderer::getCharacterRect(ObjectType type) {
    CharacterData data = m_charMap[type];
    int maxX = data.alive.x() > data.dead.x() ? data.alive.x() : data.dead.x();

    return {
      0,
      data.alive.y() * m_charSize.height(),
      m_charSize.width() * (maxX + 1),
      m_charSize.height() * 2,
    };
}

int SpriteRenderer::calculateFrame(QVariant direction, int numPOVs) {
    return (((direction.toInt() / 45) + 2) % (numPOVs + 1));
}
QPropertyAnimation *SpriteRenderer::animateDeath(QPoint frame) {
    QPropertyAnimation *anim = new QPropertyAnimation();
    anim->setPropertyName("frame");
    anim->setDuration(250 * frame.x());
    anim->setStartValue(QPoint(0, 1));
    anim->setEndValue(QPoint(frame.x(), 1));
    anim->setLoopCount(1);
    return anim;
}
