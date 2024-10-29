#include "gamepixmapitem.h"
#include "qbitmap.h"
#include "qpainter.h"
#include "publicenums.h"
#include <QPropertyAnimation>
#include <QRegion>

QColor GamePixmapItem::getTint() const {
    return m_tint;
}
void GamePixmapItem::setTint(const QColor &newTint, bool onParent) {
    if(onParent) {
        // this will tint the base pixmap.
        updatePixmap();
        auto pixmap = this->pixmap();
        QPainter painter(&pixmap);
        painter.fillRect(QRect(QPoint(0, 0), pixmap.size()), newTint);
        painter.setCompositionMode(QPainter::CompositionMode_SoftLight);
        painter.drawPixmap(QPoint(0, 0), pixmap);
        this->setPixmap(pixmap);
    } else {
        setTint(newTint);
    }
};

void GamePixmapItem::setTint(const QColor &newTint) {
    if(m_tint == newTint)
        return;
    m_tint = newTint;
    updateOverlay();
    emit tintChanged();
}

void GamePixmapItem::updateOverlay() {
    // Some things require an overlay so the color can turn back to normal once the animation is over.
    QPixmap currentPixmap = this->pixmap();
    QPixmap overlay(currentPixmap.size());
    overlay.fill(Qt::transparent);
    QPainter painter(&overlay);
    painter.setClipRegion(QRegion(currentPixmap.mask()));
    painter.setCompositionMode(QPainter::CompositionMode_Overlay);
    painter.fillRect(overlay.rect(), m_tint);
    painter.end();

    auto children = this->childItems();
    QGraphicsPixmapItem *child = nullptr;
    if(!children.empty()) {
        for(const auto &obj : children) {
            if(obj->data((int)DataRole::Type).value<ObjectType>() == ObjectType::Overlay) {
                child = dynamic_cast<QGraphicsPixmapItem *>(obj);
            }
        }
    }

    if(child == nullptr) {
        child = new QGraphicsPixmapItem();
        child->setData((int)DataRole::Type, QVariant::fromValue<ObjectType>(ObjectType::Overlay));
    }
    child->setParentItem(this);
    child->setPixmap(overlay);
}

void GamePixmapItem::addAnimation(QPropertyAnimation *animation) {
    animation->setTargetObject(this);
    m_animationGroup->addAnimation(animation);
    animation->start();
    if(m_animationGroup->state() == QAnimationGroup::State::Stopped) {
        m_animationGroup->start();
    }
}

const QPointer<QParallelAnimationGroup> GamePixmapItem::animationGroup() const {
    return m_animationGroup;
}

QPointF GamePixmapItem::scaling() const {
    return m_scaling;
}

void GamePixmapItem::setScaling(QPointF newScaling) {
    m_scaling = newScaling;
    updatePixmap();
    emit scalingChanged();
}

void GamePixmapItem::updatePixmap() {
    int x = m_frame.x() * m_frameDimension.width();
    int y = m_frame.y() * m_frameDimension.height();

    QImage frame = m_sprite.copy(x, y, m_frameDimension.width(), m_frameDimension.height());
    auto sc = QPoint(1, 1) - (m_scaling / 10);

    this->setPixmap(QPixmap::fromImage(frame).scaled(CELL_SIZE * sc.x(), CELL_SIZE * sc.y(),
                                                     Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

QImage GamePixmapItem::sprite() const {
    return m_sprite;
}

void GamePixmapItem::setSprite(QImage newSprite) {
    if(m_sprite == newSprite)
        return;

    if(m_frameDimension.isEmpty()) {
        m_frameDimension = newSprite.size();
    }
    m_sprite = newSprite;
    emit spriteChanged();
}

QSize GamePixmapItem::frameDimension() const {
    return m_frameDimension;
}

void GamePixmapItem::setFrameDimension(const QSize &newFrameDimension) {
    if(m_frameDimension == newFrameDimension)
        return;
    m_frameDimension = newFrameDimension;
    emit frameDimensionChanged();
}

QPoint GamePixmapItem::frame() const {
    return m_frame;
}

void GamePixmapItem::setFrame(QPoint newFrame) {
    if(m_frame == newFrame) {
        return;
    }
    m_frame = newFrame;
    updatePixmap();
    emit frameChanged();
}
