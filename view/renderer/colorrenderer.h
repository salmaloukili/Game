#ifndef COLORRENDERER_H
#define COLORRENDERER_H

#include "renderer.h"
#include <algorithm>
#include <QBrush>
#include <QPainter>
#include <iostream>
#include <QtGlobal>

/**
 * @brief The ColorRenderer class makes shapes filled with colors to represent the world.
 */
class ColorRenderer : public Renderer {
public:
    /**
     * @brief ColorRenderer default constructor.
     * Performs rendering of objects for color view.
     */
    ColorRenderer() {};
    /**
     * @brief renderGameObject Makes the color and shape representation from the given data.
     * @param data The GameObject data indicating objectType.
     * @param item The Pixmap item on which the object's sprite is set.
     */
    void renderGameObject(QMap<DataRole, QVariant> objectData, GamePixmapItem *item) override;

private:
    /**
     * @brief renderTile Renders the shape and color of the tile object.
     * @param object The object's data.
     * @return QPixmap of the tile.
     */
    QPixmap renderTile(QMap<DataRole, QVariant> object);
    /**
     * @brief renderDoorway Renders the shape and color of the doorway object.
     * @param object The object's data.
     * @return QPixmap of the doorway object.
     */
    QPixmap renderDoorway(QMap<DataRole, QVariant> object);
    /**
     * @brief renderHealthPack Renders the shape and color of the health pack object.
     * @param object The object's data such as health level.
     * @return QPixmap of the health pack object.
     */
    QPixmap renderHealthPack(QMap<DataRole, QVariant> object);
    /**
     * @brief renderProtagonist Renders the shape and color of the protagonist object.
     * @param object The object's data such as health level and energy level.
     * @return QPixmap of the protagonist object.
     */
    QPixmap renderProtagonist(QMap<DataRole, QVariant> object);
    /**
     * @brief renderEnemy Renders the shape and color of the enemy object.
     * @param object The object's data such as health level.
     * @return QPixmap of the enemy object.
     */
    QPixmap renderEnemy(QMap<DataRole, QVariant> object);
    /**
     * @brief renderPEnemy Renders the shape and color of the PEnemy object.
     * @param object The object's data such as health level and poison level.
     * @return QPixmap of the PEnemy object.
     */
    QPixmap renderPEnemy(QMap<DataRole, QVariant> object);
    /**
     * @brief renderMovingEnemy Renders the shape and color of the moving enemy object.
     * @param object The object's data such as health level and energy level.
     * @return QPixmap of the moving enemy object.
     */
    QPixmap renderMovingEnemy(QMap<DataRole, QVariant> object);
};

#endif // COLORRENDERER_H
