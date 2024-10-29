#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QGraphicsScene>
#include <QObject>
#include <QList>
#include "view/renderer/renderer.h"

/**
 * @brief The GameView class is the scene where all the visuals of the world are placed.
 */
class GameView : public QGraphicsScene {
    Q_OBJECT

public:
    /**
     * @brief GameView constructor which is explicit for safety purposes.
     * @param parent is the parent of the GameView which is GameController that inherits from QGraphicsView.
     */
    explicit GameView(QObject *parent = nullptr);

    /**
     * @brief createScene creates the full scene of the gameObjects.
     * passed to it to visualize based on the renderer to use.
     * @param gameObjects is the data of every object in the scene.
     * @param renderer is the Renderer to use to visualize.
     */
    void createScene(const QList<QList<QList<QMap<DataRole, QVariant>>>> &gameObjects,
                     QSharedPointer<Renderer> renderer = nullptr);

    /**
     * @brief setRenderer sets Renderer.
     * @param newRenderer is newly set Renderer class to perform visualizations with.
     */
    void setRenderer(QSharedPointer<Renderer> newRenderer);

private:
    /**
     * @brief getPixmapItem gets a pxmp from the list in a point.
     * @param x coordinate on the 2D plane.
     * @param y coordinate on the 2D plane.
     * @param type the GameObject type.
     * @return Pointer to the Pixmap.
     */
    GamePixmapItem *getPixmapItem(int x, int y, QVariant type);
    /**
     * @brief m_renderer stores the current Renderer of the scene.
     */
    QSharedPointer<Renderer> m_renderer;
    /**
     * @brief m_tiles Store the graphical representation of each GameObject
     */
    QList<QList<GamePixmapItem *>> m_tiles;

public slots:
    /**
     * @brief dataChanged is the slot to monitor the change of properties of any object in the scene.
     * @param objectData is the changed data of the object.
     */
    void dataChanged(QMap<DataRole, QVariant> objectData);
};

#endif // GAMEVIEW_H
