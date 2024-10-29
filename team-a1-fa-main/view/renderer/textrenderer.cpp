#include <QFont>
#include <QPainter>
#include <QPen>
#include <QRandomGenerator>
#include "textrenderer.h"
#define TO_CHAR(v) ((v * 255) / 100)

void TextRenderer::renderGameObject(QMap<DataRole, QVariant> data, GamePixmapItem *item) {
    QImage image;
    switch(data[DataRole::Type].value<ObjectType>()) {
    case ObjectType::Tile:
        image = renderTile(data);
        break;
    case ObjectType::Doorway:
        image = rotateImage(renderCharacter("||", {0, 0, 0, 255}), 180);
        break;
    case ObjectType::HealthPack:
        image = rotateImage(renderCharacter("c[]", {43, 255, 0}), 180);
        break;
    case ObjectType::Protagonist:
        image = renderCharacter("Å",
                                {
                                  0,
                                  0,
                                  0,
                                  TO_CHAR(data[DataRole::Health].toInt()),
                                },
                                data[DataRole::Direction].toInt());
        break;
    case ObjectType::Enemy:
        image = renderCharacter("#", {
                                       255,
                                       0,
                                       0,
                                       TO_CHAR(data[DataRole::Health].toInt()),
                                     },
                                data[DataRole::Direction].toInt());
        break;
    case ObjectType::PoisonEnemy:
        image = renderCharacter("ⓧ", {
                                       0,
                                       255,
                                       0,
                                       TO_CHAR(data[DataRole::Health].toInt()),
                                     },
                                data[DataRole::Direction].toInt());
        break;
    case ObjectType::MovingEnemy:
        image = renderCharacter("@", {
                                       255,
                                       255,
                                       0,
                                       TO_CHAR(data[DataRole::Health].toInt()),
                                     },
                                data[DataRole::Direction].toInt());
        break;
    default:
        break;
    }
    item->setSprite(image);
    item->updatePixmap();
    item->setActive(true);
    Renderer::renderGameObject(data, item);
}

QImage TextRenderer::renderTile(QMap<DataRole, QVariant> data) {
    // The Pixmaps have to be transparent, text is AAd by default
    QPixmap pixmap(CELL_SIZE, CELL_SIZE);
    pixmap.fill({227, 239, 255, 255});
    QPainter painter(&pixmap);

    QFont font = painter.font();
    font.setBold(true);
    font.setKerning(false);
    font.setFixedPitch(true);
    font.setPointSize(CELL_SIZE / 4); // Set the font size relative to cell size
    font.setLetterSpacing(QFont::AbsoluteSpacing, 0);
    font.setWeight(QFont::Black);
    painter.setFont(font);
    painter.setLayoutDirection(Qt::LeftToRight);

    // This has to be calculated to know the offset of the underscore characters
    QFontMetrics fontMetrics(font);
    int linePosition = ((CELL_SIZE - fontMetrics.horizontalAdvance("_")) / 4);

    // Draw the bottom lines, we don't need top lines unless we are at the top
    // The offsets are a bit arbitrary on the y ax.
    painter.drawText(linePosition - 4, CELL_SIZE - 3, "_");
    painter.drawText(2 * linePosition - 4, CELL_SIZE - 3, "_");
    painter.drawText(3 * linePosition - 4, CELL_SIZE - 3, "_");
    painter.drawText(4 * linePosition - 4, CELL_SIZE - 3, "_");

    if(!data[DataRole::Position].toPoint().y()) {
        painter.drawText(linePosition - 4, 0, "_");
        painter.drawText(2 * linePosition - 4, 0, "_");
        painter.drawText(3 * linePosition - 4, 0, "_");
        painter.drawText(4 * linePosition - 4, 0, "_");
    }

    // The | characters are much longer than the _ so we make them smaller
    font.setPointSize(CELL_SIZE / 8);
    font.setStretch(125);
    painter.setFont(font);

    // This looks extremely funky but it is what it is
    painter.drawText(-1, CELL_SIZE / 4 - 2, "|");
    painter.drawText(-1, 2 * (CELL_SIZE / 4) - 2, "|");
    painter.drawText(-1, 3 * (CELL_SIZE / 4) - 2, "|");
    painter.drawText(-1, CELL_SIZE - 2, "|");
    // Since the renderer has no idea about the size of the world,
    // It cannot simply know where the
    painter.drawText(CELL_SIZE - 2, CELL_SIZE / 4 - 2, "|");
    painter.drawText(CELL_SIZE - 2, 2 * (CELL_SIZE / 4) - 2, "|");
    painter.drawText(CELL_SIZE - 2, 3 * (CELL_SIZE / 4) - 2, "|");
    painter.drawText(CELL_SIZE - 2, CELL_SIZE - 2, "|");

    if(data[DataRole::Energy] == INFINITY) {
        for(int i = 0; i < CELL_SIZE; i++)
            for(int j = 0; j < CELL_SIZE; j++)
                painter.drawText(i, j, ".");
    }

    if(int poisonLevel = data[DataRole::PoisonLevel].toInt()) {
        int maxDots = CELL_SIZE;
        int numberOfDots = (maxDots * poisonLevel);
        painter.setPen(Qt::green);
        for(int i = 0; i < numberOfDots; ++i) {
            int randomX = QRandomGenerator::global()->bounded(CELL_SIZE);
            int randomY = QRandomGenerator::global()->bounded(CELL_SIZE);
            painter.drawText(randomX, randomY, ".");
        }
    }

    if(float energy = data[DataRole::Energy].toFloat()) {
        int maxDots = CELL_SIZE;
        int numberOfDots = round(((float)maxDots * energy));
        painter.setPen(Qt::blue);
        for(int i = 0; i < numberOfDots; ++i) {
            int randomX = QRandomGenerator::global()->bounded(CELL_SIZE);
            int randomY = QRandomGenerator::global()->bounded(CELL_SIZE);
            painter.drawText(randomX, randomY, ".");
        }
    }

    return pixmap.toImage();
}

QImage TextRenderer::renderCharacter(QString str, QColor color, int direction) {
    QPixmap pixmap(CELL_SIZE, CELL_SIZE);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QFont font = painter.font();
    font.setBold(true);
    font.setKerning(false);
    font.setFixedPitch(true);
    font.setPointSize(CELL_SIZE / 1.7); // Set the font size relative to cell size
    font.setLetterSpacing(QFont::AbsoluteSpacing, 0);
    font.setWeight(QFont::Black);
    painter.setFont(font);

    QPen pen = painter.pen();
    pen.setColor(color);
    painter.setPen(pen);

    painter.drawText(pixmap.rect(), Qt::AlignCenter, QString(str));
    painter.end();

    return rotateImage(pixmap.toImage(), direction);
}
