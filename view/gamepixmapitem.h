#ifndef GAMEPIXMAPITEM_H
#define GAMEPIXMAPITEM_H

#include <QObject>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include <QPointer>
#include <qgraphicsitem.h>

/**
 * @brief The GamePixmapItem class is the custom child class of QGraphicsPixmapItem.
 * to define and store animations. It inherits from QObject as to make it dynamic.
 */
class GamePixmapItem : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
    // Needed to animate them using QPropertyAnimation (meta)
    Q_PROPERTY(QColor tint READ getTint WRITE setTint NOTIFY tintChanged FINAL)
    Q_PROPERTY(QPoint frame READ frame WRITE setFrame NOTIFY frameChanged FINAL)
    Q_PROPERTY(QPointF scaling READ scaling WRITE setScaling NOTIFY scalingChanged FINAL)
    Q_PROPERTY(QPointF pos READ pos WRITE setPos NOTIFY posChanged FINAL)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged FINAL)

public:
    /**
     * @brief GamePixmapItem constructor which also initializes m_animationGroup.
     */
    explicit GamePixmapItem()
        : QObject()
        , QGraphicsPixmapItem() {
        m_animationGroup = new QParallelAnimationGroup;
        m_animationGroup->setParent(this);
    };
    /**
     * @brief frame is m_frame getter.
     * @return m_frame which is the current frame visualized.
     */
    QPoint frame() const;
    /**
     * @brief setFrame sets new frame.
     * @param newFrame new frame to visualize.
     */
    void setFrame(QPoint newFrame);
    /**
     * @brief getTint is m_tint getter.
     * @return m_tint.
     */
    QColor getTint() const;
    /**
     * @brief setTint sets tint on the pixmap of the size CELL_SIZE^2.
     * @param newTint color of the overlay.
     */
    void setTint(const QColor &newTint);
    /**
     * @brief setTint sets tint of the object taking into account its contour lines.
     * @param newTint color of the overlay.
     * @param onParent if true makes new transparent pixmap to overlay.
     * in order not to permanently change the sprites color.
     */
    void setTint(const QColor &newTint, bool onParent);
    /**
     * @brief sprite gets the m_sprite.
     * @return m_sprite.
     */
    QImage sprite() const;
    /**
     * @brief setSprite sets newSprite.
     * @param newSprite the new sprite to visualize as a part of animation.
     */
    void setSprite(QImage newSprite);
    /**
     * @brief frameDimension gets m_frameDimension.
     * @return m_frameDimension.
     */
    QSize frameDimension() const;
    /**
     * @brief setFrameDimension sets newFrameDimension.
     * @param newFrameDimension the new frame dimension to visualize as a part of animation.
     */
    void setFrameDimension(const QSize &newFrameDimension);
    /**
     * @brief updatePixmap updates pixmap to new dimensions.
     */
    void updatePixmap();
    /**
     * @brief updateOverlay updates overlay to new color.
     */
    void updateOverlay();
    /**
     * @brief addAnimation adds new animation and starts it.
     * @param animation the new animation.
     */
    void addAnimation(QPropertyAnimation *animation);
    /**
     * @brief animationGroup.
     * @return current animation group.
     */
    const QPointer<QParallelAnimationGroup> animationGroup() const;
    /**
     * @brief scaling.
     * @return current scaling of the pixmap.
     */
    QPointF scaling() const;
    /**
     * @brief setScaling sets new scaling of the pixmap.
     * @param newScaling.
     */
    void setScaling(QPointF newScaling);

protected:
    /**
     * @brief CELL_SIZE constant cell size.
     */
    inline static constexpr int CELL_SIZE = 50;

private:
    /**
     * @brief m_opacity Opacity of the base QPixmap.
     */
    int m_opacity = 255;
    /**
     * @brief m_tint The tint of the overlay.
     */
    QColor m_tint;
    /**
     * @brief m_frameDimension The size of a single frame.
     */
    QSize m_frameDimension;
    /**
     * @brief m_frame The selected frame, sprites can have more than one row.
     */
    QPoint m_frame {0, 0};
    /**
     * @brief m_sprite stores the current sprite.
     */
    QImage m_sprite;
    /**
     * @brief m_scaling stores the current scaling of the sprite.
     */
    QPointF m_scaling {0, 0};
    /**
     * @brief m_animationGroup stores every animation.
     */
    QPointer<QParallelAnimationGroup> m_animationGroup;

signals:
    void tintChanged();
    void frameChanged();
    void spriteChanged();
    void frameDimensionChanged();
    void scalingChanged();
    void posChanged();
    void opacityChanged();
};

#endif // GAMEPIXMAPITEM_H
