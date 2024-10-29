
#include "colorrenderer.h"

void ColorRenderer::renderGameObject(QMap<DataRole, QVariant> data, GamePixmapItem *item) {
    // It is very difficult to generalize these.
    switch(data[DataRole::Type].value<ObjectType>()) {
    case ObjectType::Tile:
        item->setSprite(renderTile(data).toImage());
        break;
    case ObjectType::Doorway:
        item->setSprite(renderDoorway(data).toImage());
        break;
    case ObjectType::HealthPack:
        item->setSprite(renderHealthPack(data).toImage());
        break;
    case ObjectType::Protagonist:
        item->setSprite(renderProtagonist(data).toImage());
        break;
    case ObjectType::Enemy:
        item->setSprite(renderEnemy(data).toImage());
        break;
    case ObjectType::PoisonEnemy:
        item->setSprite(renderPEnemy(data).toImage());
        break;
    case ObjectType::MovingEnemy:
        item->setSprite(renderMovingEnemy(data).toImage());
        break;
    default:
        item->setSprite(renderEnemy(data).toImage());
        break;
    }
    item->updatePixmap();
    item->setActive(true);
    Renderer::renderGameObject(data, item);
}

QPixmap ColorRenderer::renderTile(
  QMap<DataRole, QVariant> object) {
    float energyLevel = object[DataRole::Energy].toFloat();
    int brightness = 255 - (energyLevel * 255 / 1);
    QColor color(179, 196, 255, brightness);
    QPixmap pixmap(CELL_SIZE, CELL_SIZE);
    pixmap.fill(color);

    if(int poisonLevel = object[DataRole::PoisonLevel].toInt()) {
        QPainter painter(&pixmap);
        painter.fillRect(QRect(QPoint(0, 0), pixmap.size()), QColor(0, 255, 0, poisonLevel * 16));
        painter.drawPixmap(QPoint(0, 0), pixmap);
    }

    return pixmap;
}

QPixmap ColorRenderer::renderDoorway(QMap<DataRole, QVariant>) {
    QPixmap pixmap(CELL_SIZE, CELL_SIZE);
    pixmap.fill(Qt::transparent);

    QColor color(123, 63, 0);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen(Qt::transparent);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.setBrush(QBrush(color));

    QPoint topleft(CELL_SIZE / 5, CELL_SIZE / 5);
    QPoint topright(CELL_SIZE - (CELL_SIZE / 5), CELL_SIZE / 5);
    QPoint bottomleft(CELL_SIZE / 5, CELL_SIZE - (CELL_SIZE / 5));
    QPoint bottomright(CELL_SIZE - (CELL_SIZE / 5), CELL_SIZE - (CELL_SIZE / 5));

    QList<QPoint> points = {topleft, topright, bottomright, bottomleft};

    painter.drawPolygon(points.data(), points.size());

    painter.end();

    return pixmap;
}

QPixmap ColorRenderer::renderHealthPack(
  QMap<DataRole, QVariant> object) {
    int healthLevel = object[DataRole::Health].toInt();
    int minIntensity = 50;
    int maxIntensity = 255;
    int final_color = minIntensity + (maxIntensity - minIntensity) * healthLevel / 100;
    QColor color(0, final_color, 0);

    QPixmap pixmap(CELL_SIZE, CELL_SIZE);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen(Qt::transparent);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.setBrush(QBrush(color));

    QPoint top(CELL_SIZE / 2, CELL_SIZE / 5);
    QPoint right(CELL_SIZE - (CELL_SIZE / 5), CELL_SIZE / 2);
    QPoint bottom(CELL_SIZE / 2, CELL_SIZE - (CELL_SIZE / 5));
    QPoint left(CELL_SIZE / 5, CELL_SIZE / 2);

    QList<QPoint> points = {top, right, bottom, left};

    painter.drawPolygon(points.data(), points.size());

    painter.end();

    return pixmap;
}

QPixmap ColorRenderer::renderProtagonist(
  QMap<DataRole, QVariant> object) {
    QPixmap pixmap(CELL_SIZE, CELL_SIZE);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    QList<QPoint> starPoints = {
      QPoint(CELL_SIZE / 2, 0), // Top point
      QPoint(CELL_SIZE, CELL_SIZE / 2), // Right point
      QPoint(CELL_SIZE / 2, CELL_SIZE), // Bottom point
      QPoint(0, CELL_SIZE / 2), // Left point
      QPoint(CELL_SIZE / 2, 0) // Back to Top point
    };

    QList<QPoint> arrowPoints = {
      QPoint(CELL_SIZE / 2 + 8, 8), // Right-top midpoint
      QPoint(CELL_SIZE / 2, 0), // Top point
      QPoint(CELL_SIZE / 2 - 8, 8), // Left-top midpoint
      QPoint(CELL_SIZE / 2 + 8, 8) // back to Right-top midpoint
    };

    QPolygon rightHalf(starPoints.mid(0, 3));
    QPolygon leftHalf(starPoints.mid(2));
    QPolygon arrow(arrowPoints);

    int healthLevel = object[DataRole::Health].toInt();
    int minIntensity = 50;
    int maxIntensity = 255;
    int final_color = minIntensity + (maxIntensity - minIntensity) * healthLevel / 100;
    QColor color(0, final_color, 0);
    painter.setBrush(QBrush(color));
    painter.drawPolygon(leftHalf);

    int energyLevel = object[DataRole::Energy].toInt();
    minIntensity = 50;
    maxIntensity = 255;
    final_color = minIntensity + (maxIntensity - minIntensity) * energyLevel / 100;
    QColor energyColor(final_color, final_color, 0);
    painter.setBrush(QBrush(energyColor));
    painter.drawPolygon(rightHalf);

    painter.setBrush(QBrush(Qt::black));
    painter.drawPolygon(arrow);

    painter.end();

    int direction = object[DataRole::Direction].toInt();
    pixmap = rotatePixmap(pixmap, direction);

    return pixmap;
}

QPixmap ColorRenderer::renderEnemy(
  QMap<DataRole, QVariant> object) {
    QPixmap pixmap(CELL_SIZE, CELL_SIZE);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    QList<QPoint> trianglePoints = {
      QPoint(CELL_SIZE / 2, CELL_SIZE / 5), // Top
      QPoint(CELL_SIZE - (CELL_SIZE / 5), CELL_SIZE - (CELL_SIZE / 5)), // Right bottom
      QPoint(CELL_SIZE / 2, CELL_SIZE - (CELL_SIZE / 5)), // Bottom centre
      QPoint(CELL_SIZE / 5, CELL_SIZE - (CELL_SIZE / 5)), // Left bott0m
      QPoint(CELL_SIZE / 2, CELL_SIZE / 5) // Back to Top point
    };

    int healthLevel = object[DataRole::Health].toInt();
    int minIntensity = 50;
    int maxIntensity = 255;
    int final_color = minIntensity + (maxIntensity - minIntensity) * healthLevel / 100;
    QColor color(0, final_color, 0);
    painter.setBrush(QBrush(color));
    painter.drawPolygon(trianglePoints);

    painter.end();

    int direction = object[DataRole::Direction].toInt();
    pixmap = rotatePixmap(pixmap, direction);

    return pixmap;
}

QPixmap ColorRenderer::renderPEnemy(QMap<DataRole, QVariant> object) {
    QPixmap pixmap(CELL_SIZE, CELL_SIZE);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    QList<QPoint> trianglePoints = {
      QPoint(CELL_SIZE / 2, CELL_SIZE / 5), // Top
      QPoint(CELL_SIZE - (CELL_SIZE / 5), CELL_SIZE - (CELL_SIZE / 5)), // Right bottom
      QPoint(CELL_SIZE / 2, CELL_SIZE - (CELL_SIZE / 5)), // Bottom centre
      QPoint(CELL_SIZE / 5, CELL_SIZE - (CELL_SIZE / 5)), // Left bott0m
      QPoint(CELL_SIZE / 2, CELL_SIZE / 5) // Back to Top point
    };

    QPolygon rightHalf(trianglePoints.mid(0, 3));
    QPolygon leftHalf(trianglePoints.mid(2));

    int healthLevel = object[DataRole::Health].toInt();
    int minIntensity = 50;
    int maxIntensity = 255;
    int final_color = minIntensity + (maxIntensity - minIntensity) * healthLevel / 100;
    QColor color(0, final_color, 0);
    painter.setBrush(QBrush(color));
    painter.drawPolygon(leftHalf);

    int poisonLevel = object[DataRole::PoisonLevel].toInt();
    minIntensity = 50;
    maxIntensity = 200;
    final_color = minIntensity + (maxIntensity - minIntensity) * poisonLevel / 100;
    color.setRgbF(final_color, 0, final_color);
    painter.setBrush(QBrush(color));
    painter.drawPolygon(rightHalf);

    painter.end();

    int direction = object[DataRole::Direction].toInt();
    pixmap = rotatePixmap(pixmap, direction);

    return pixmap;
}

QPixmap ColorRenderer::renderMovingEnemy(QMap<DataRole, QVariant> object) {
    QPixmap pixmap(CELL_SIZE, CELL_SIZE);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    QList<QPoint> trianglePoints = {
      QPoint(CELL_SIZE / 2, CELL_SIZE / 5), // Top
      QPoint(CELL_SIZE - (CELL_SIZE / 5), CELL_SIZE - (CELL_SIZE / 5)), // Right bottom
      QPoint(CELL_SIZE / 2, CELL_SIZE - (CELL_SIZE / 5)), // Bottom centre
      QPoint(CELL_SIZE / 5, CELL_SIZE - (CELL_SIZE / 5)), // Left bott0m
      QPoint(CELL_SIZE / 2, CELL_SIZE / 5) // Back to Top point
    };

    QPolygon rightHalf(trianglePoints.mid(0, 3));
    QPolygon leftHalf(trianglePoints.mid(2));

    int healthLevel = object[DataRole::Health].toInt();
    int minIntensity = 50;
    int maxIntensity = 255;
    int final_color = minIntensity + (maxIntensity - minIntensity) * healthLevel / 100;
    QColor color(final_color, 0, 0);
    painter.setBrush(QBrush(color));
    painter.drawPolygon(leftHalf);

    int energy = object[DataRole::Energy].toInt();
    minIntensity = 50;
    maxIntensity = 200;
    final_color = minIntensity + (maxIntensity - minIntensity) * energy / 100;
    color.setRgbF(0, final_color, final_color);
    painter.setBrush(QBrush(color));
    painter.drawPolygon(rightHalf);

    painter.end();

    int direction = object[DataRole::Direction].toInt();
    pixmap = rotatePixmap(pixmap, direction);

    return pixmap;
}
