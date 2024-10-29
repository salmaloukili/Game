#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include "renderer.h"

#include <QColor>
#include <QPoint>
#include <QPropertyAnimation>
#include <QTimer>
/**
 * @brief The SpriteRenderer class handles the conversion of data to GamePixmapItems.
 * It also animates behaviors not generalizable such as death, which in this view
 * change the frame from the sprite.
 */
class SpriteRenderer : public Renderer {
public:
    /**
     * @brief SpriteRenderer Default constructor. Calculates the size of the sprites and caches them.
     * The caching is not strictly necessary, since QImages are shared objects so they never get copied
     * unless explicitly asked to. The sizes do not need to be computed every time so they are calculated once.
     */
    SpriteRenderer() {
        m_characters = QImage(":/images/characters.png");
        m_tiles = QImage(":/images/tiles.png");
        m_charSize = QSize(m_characters.width() / CHAR_MAX_X, m_characters.height() / CHAR_MAX_Y);
        m_tileSize = QSize(m_tiles.width() / TILE_COUNT, m_tiles.width() / TILE_COUNT);
    }

    /**
     * @brief The SpriteSettings enum is used to crop the sprites.
     */
    enum SpriteSettings {
        TILE_COUNT = 21,
        CHAR_MAX_X = 10,
        CHAR_MAX_Y = 8,
    };

    /**
     * @brief The CharacterData struct stores a pair of points for the character sprite.
     * Since not all characters have the same size when they are alive or dead this is needed.
     * It is usually done in a JSON file and loaded in but for small sprite sheets its fine to do in code.
     */
    struct CharacterData {
        QPoint alive;
        QPoint dead;
    };

    /**
     * @brief m_charMap Maps the ObjectTypes to their respective sprite.
     */
    inline static const QMap<ObjectType, CharacterData> m_charMap = {
      {ObjectType::Protagonist, {{7, 0}, {9, 1}}},
      {ObjectType::MovingEnemy, {{7, 2}, {4, 3}}},
      {ObjectType::PoisonEnemy, {{7, 4}, {4, 5}}},
      {ObjectType::Enemy, {{7, 6}, {2, 7}}},
    };

    /**
     * @brief renderGameObject Overloaded function from Renderer. Converts the data into the appropriate Sprite.
     * This function adds animations or changes colors depending on the data. It does not actually re-render
     * the sprites every time data is changed.
     * @param data The GameObject data The GameObject data.
     * @param item The GamePixmapItem to write the data on.
     */
    void renderGameObject(QMap<DataRole, QVariant> data, GamePixmapItem *item) override;

    /**
     * @brief renderGameObject Overloaded from Renderer. Makes a new GamePixmapItem and adds the appropriate sprite.
     * This function is only run once in the SpriteRenderer. Which means that changes to the objects are
     * applied to the same sprite. The other renderers do not have this feature since the bulk of the changes
     * are very difficult to animate using parameters and not generalizable.
     * @param data The GameObject data
     * @return A GamePixmapItem containing a sprite sheets for the correct GameObject.
     */
    GamePixmapItem *renderGameObject(QMap<DataRole, QVariant> data) override;

private:
    /**
     * @brief sliceFrames Function used to slice a sprite sheets into the desired frame.
     * @param image Image to be sliced
     * @param diagonal The line to slice it in multiples of the frame size.
     * @param frameSize Size of the frame in (x,y), all of our sprites are squares but that is not always the case.
     * @return The croped QImage
     */
    QImage sliceFrames(QImage image, QLine diagonal, QPoint frameSize);

    /**
     * @brief getTileRect Returns which tile sprite to use based on the energy and TILE_COUNT.
     * @param data the GameObject data.
     * @return A rectangle to crop the tile sprite with.
     */
    QRect getTileRect(QMap<DataRole, QVariant> data);

    /**
     * @brief getCharacterRect Returns which ALIVE tile sprite to use based ObjectType.
     * @param type The GameObject type.
     * @return A rectangle to crop the character sprite with.
     */
    QRect getCharacterRect(ObjectType type);

    /**
     * @brief calculateFrame Return the frame based on a direction and POVnum.
     * @param direction The Direction of the GameObject.
     * @param numPOVs The number of point of views the sprites have.
     * @return int Frame number
     */
    int calculateFrame(QVariant direction, int POVnum);

    /**
     * @brief animateDeath Makes an animation of the death of a character.
     * @param frame The last frame in the sprite.
     * @return QPropertyAnimation of the tiles to change.
     */
    QPropertyAnimation *animateDeath(QPoint frame);

    ///@{
    /**
     * @brief  Cached images for tiles and characters.
     */
    QImage m_tiles, m_characters;
    ///@}

    ///@{
    /**
     * @brief  Cached Cached sizes for the images.
     */
    QSize m_charSize, m_tileSize;
    ///@}
};

#endif // SPRITERENDERER_H
