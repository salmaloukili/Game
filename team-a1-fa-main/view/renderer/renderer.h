#ifndef RENDERER_H
#define RENDERER_H

#include "publicenums.h"
#include "qpixmap.h"
#include "view/gamepixmapitem.h"

/**
 * @brief The Renderer class is a base class for the renderers
 * that use different visualizations for objects
 */
class Renderer {
public:
    /**
     * @brief ~Renderer virtual destructor
     */
    virtual ~Renderer() {};

    /**
     * @brief renderGameObjects renders the initial state of the world
     * @param objectData is the full data which is parsed one by one
     * @return tile The GamePixmap with the data visualized
     */
    GamePixmapItem *renderGameObjects(QList<QMap<DataRole, QVariant>> objectData);

    /**
     * @brief renderGameObject creates GamePixmapItem and sets the objectData on it
     * This function is overloaded in some child classes of Renderer
     * @param objectData The GameObject data to set on the item
     * @param item The GamePixmapItem which is teh separate tile in the world
     */
    virtual GamePixmapItem *renderGameObject(QMap<DataRole, QVariant> objectData);

    /**
     * @brief renderGameObject applies latest change to the object visually to show change of the state
     * Gets overloaded in Renderer's child classes
     * @param objectData the data of the item
     * @param item the item to animate changes of
     */
    virtual void renderGameObject(QMap<DataRole, QVariant> objectData, GamePixmapItem *item);

    /**
     * @brief rotatePixmap rotated the pixmap based on direction
     * @param originalPixmap initial pixmap of the object
     * @param direction the direction in which pixmap is turned
     * @return QPixmap The rotated pixmap
     */
    QPixmap rotatePixmap(const QPixmap &originalPixmap, int direction);

    /**
     * @brief rotateImage rotates the image based on direction
     * @param image initial image of the object
     * @param direction the direction in which image is turned
     * @return QImage the rotated image
     */
    QImage rotateImage(const QImage &image, int direction);

protected:
    /**
     * @brief CELL_SIZE is constant size of the grid cell
     */
    inline static constexpr int CELL_SIZE = 50;

protected:
    /**
     * @brief animateHealthPack animates the protagonist obtain of the healthpack
     * @param health the health level of the object
     * @param item the GamePixmapItem of the health pack
     * @return QImage The animated QImage of the health pack
     */
    QImage animateHealthPack(int health, GamePixmapItem *item);

    /**
     * @brief animateTint animates tint over the pixmap to visualize damage
     * @param final
     * @param initial
     * @return QPropertyAnimation
     */
    QPropertyAnimation *animateTint(QColor final, QColor initial = {0, 0, 0, 0});

    /**
     * @brief animateAttack animates the attack by slightly moving the pixmap in the direction of dir
     * @param dir the direction of the attack
     * @param attacking boolean indication the if the object is attacking or is getting atacked
     * @return QPropertyAnimation The animation of the attack
     */
    QPropertyAnimation *animateAttack(int dir, bool attacking);

    /**
     * @brief animateBounce animates bouncing movement of the protagonist
     * @return QPropertyAnimation the animation of the bounce
     */
    QPropertyAnimation *animateBounce();

    /**
     * @brief animateHealth animates health gain of the protagonist
     * @param dir indicates if health is gained or lost
     * @return QPropertyAnimation the animation of health loss or gain
     */
    QPropertyAnimation *animateHealth(Direction dir);

    /**
     * @brief animateHide animates the change of the opacity in the object, implemented on the moving enemy in our case
     * @return QPropertyAnimation the animation of the breathing color of the object
     */
    QPropertyAnimation *animateHide();
};

#endif // RENDERER_H
