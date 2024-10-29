#include <node.h>

#include "gamecontroller.h"
#include "model/modelfactory.h"
#include "model/behaviors/attack.h"
#include "model/behaviors/movement.h"
#include "pathfinder_class.h"
#include "view/renderer/spriterenderer.h"
#include "view/renderer/textrenderer.h"
#include "view/renderer/colorrenderer.h"

void GameController::startGame() {
    m_view = QSharedPointer<GameView>::create(this); // Instantiate the GameView
    m_view->setRenderer(QSharedPointer<SpriteRenderer>::create()); // Instantiate and set the default renderer
    createNewLevel(m_gameLevel); // Create first level
    this->show();
}

void GameController::updateLevel(Direction direction) {
    int newLevel = (direction == Direction::Up) ? m_gameLevel + 1 : m_gameLevel - 1; // Check whether we go level up or level down

    // Invalid level
    if(newLevel < 0 || newLevel > m_models.size()) {
        qDebug() << "Invalid level change request. Current Level: " << m_gameLevel << ", Requested Level: " << newLevel;
        return;
    }
    disconnectCurrentModel();
    // Determine whether to create a new model or use an existing one
    if(newLevel + 1 > m_models.size()) {
        qDebug() << "Creating new model for level " << newLevel;
        createNewLevel(newLevel);

    } else {
        qDebug() << "Switching to existing model for level " << newLevel;
        auto *model = m_models[newLevel].first;
        if(!model) {
            qDebug() << "Error: Model is null at level " << newLevel;
            return;
        }

        m_gameLevel = newLevel;
        m_enemies = 10 * (m_gameLevel + 1) + 25;
        m_health_packs = 5 - (m_gameLevel / 3);

        m_protagonist = model->getObject(ObjectType::Protagonist).at(0);
        m_view->createScene(model->getAllData());
        connectCurrentModel();
    }

    // Signal changes to the window
    emitLevelUpdates();
}

void GameController::createNewLevel(int level) {
    // Set the level parameters
    int tiles = m_levelSize.width() * m_levelSize.height();
    m_gameLevel = level;
    m_enemies = tiles / 20 + (level + 1) * sqrt(tiles) / 10;
    m_health_packs = sqrt(tiles) / 4 - (level / 5);
    // Call the model factory to generate model
    auto model = ObjectModelFactory::createModel(m_enemies, m_health_packs, 0.5f, m_gameLevel,
                                                 m_levelSize.height(), m_levelSize.width());
    m_models.append(model);
    model.first->setParent(this);
    // Set the character aka protagonist
    auto oldCharacter = m_protagonist;
    m_protagonist = model.first->getObject(ObjectType::Protagonist).at(0);

    if(oldCharacter) {
        m_protagonist->setData(oldCharacter->getAllData().at(0));
    }

    // Create new scene
    m_view->createScene(model.first->getAllData());
    connectCurrentModel(); // Reconnect new model
    emitLevelUpdates(); // Signal changes to the window
}

void GameController::disconnectCurrentModel() {
    auto *model = m_models[m_gameLevel].first;
    disconnect(model, &GameObjectModel::dataChanged, m_view.get(), &GameView::dataChanged);
    disconnect(this, &GameController::tick, model, &GameObjectModel::tick);
    disconnect(model, &GameObjectModel::dataChanged, this, &GameController::dataChanged);
}
void GameController::connectCurrentModel() {
    auto *model = m_models[m_gameLevel].first;
    connect(model, &GameObjectModel::dataChanged, m_view.get(), &GameView::dataChanged);
    connect(this, &GameController::tick, model, &GameObjectModel::tick);
    connect(model, &GameObjectModel::dataChanged, this, &GameController::dataChanged);
}

void GameController::emitLevelUpdates() {
    emit enemiesUpdated(m_enemies);
    emit healthPacksUpdated(m_health_packs);
    emit levelUpdated(m_gameLevel);
}

void GameController::dataChanged(QMap<DataRole, QVariant> objectData) {
    // Filter the changes based on their type
    switch(objectData[DataRole::Type].value<ObjectType>()) {
    case ObjectType::Protagonist:
        if(objectData[DataRole::LatestChange].value<DataRole>() == DataRole::Energy) {
            updateEnergy();
        }

        if(objectData[DataRole::LatestChange].value<DataRole>() == DataRole::Health) {
            updateHealth();
        }
        break;
    case ObjectType::Doorway:
        if(objectData[DataRole::Direction].value<Direction>() == Direction::Down) {
            if(m_gameLevel != 0) {
                updateLevel(Direction::Down); // go down a level
            }
        }
        if(objectData[DataRole::Direction].value<Direction>() == Direction::Up) {
            updateLevel(Direction::Up); // go up a level
        }
        break;
    default:
        break;
    }
}
void GameController::automaticAttack() {
    // Find the enemy and attack it until it dies.
    auto target = m_protagonist->getNeighbor(m_protagonist->getData(DataRole::Direction).toDouble())
                    ->findChild({ObjectType::_ENEMIES_START, ObjectType::_ENEMIES_END});

    while(m_gameState != State::GameOver && target && target->getData(DataRole::Health).toInt()) {
        QTime time = QTime::currentTime().addMSecs(200);
        while(QTime::currentTime() < time) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 200);
        }
        characterAttack();
    }
}

void GameController::executePath(std::vector<int> path, bool full) {
    // Tile at the start position
    auto first_tile = qobject_cast<GameObject *>(m_protagonist->parent());
    for(int move : path) {
        // Assign the path tiles to DataRole Path
        first_tile = first_tile->getNeighbor(((45 * move + 90) % 360));
        first_tile->setData(DataRole::Path, true);
    }

    for(int move : path) {
        Direction direction = (Direction)((45 * move + 90) % 360);

        // Quick Delay for visualization
        QTime dieTime = QTime::currentTime().addMSecs(100);
        while(QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

        if(direction != m_protagonist->getData(DataRole::Direction).value<Direction>()) {
            characterMove(direction);
        }

        // Check whether enemy is on the way of the path and attack it
        if(auto tile = m_protagonist->getNeighbor(direction)) {
            if(tile->hasChild({ObjectType::_ENEMIES_START, ObjectType::_ENEMIES_END})) {
                automaticAttack();
            }
        }
        // Play fully automatic
        if(full) {
            QPointer<const GameObject> obj;
            // Find enemy or healthpack if energy or health too low. Number is sort of arbitrary
            if(m_protagonist->getData(DataRole::Energy).toInt() < 80 || m_protagonist->getData(DataRole::PoisonLevel).toInt() > 15) {
                obj = m_protagonist->nearest({ObjectType::_ENEMIES_START, ObjectType::_ENEMIES_END});

            } else if(m_protagonist->getData(DataRole::Health).toInt() < 80) {
                obj = m_protagonist->nearest(ObjectType::HealthPack);
            }
            // Can be that there are no HP or enemies left.
            if(obj) {
                QPoint objPos = obj->getData(DataRole::Position).toPoint();
                QPoint charPos = m_protagonist->getData(DataRole::Position).toPoint();
                QPoint doorPos(m_models[m_gameLevel].first->getColumnCount(),
                               m_models[m_gameLevel].first->getRowCount());
                int distObj = (objPos - charPos).manhattanLength();
                int distDoor = (doorPos - charPos).manhattanLength();
                // Check if the distance to the door is smaller than the distance to the object.
                if(distObj < distDoor) {
                    pathFinder(objPos.x(), objPos.y());
                }
                // After we go to the object, return to the pathfinder function that called this.
                // That function will schedule itself after this function finishes.
                return;
            }
        }
        characterMove(direction);
    }
}

void GameController::pathFinder(int x, int y) {
    bool full = (x == -1 && y == -1);
    auto nodes = m_models[m_gameLevel].second; // Node class for the pathfinder

    int rows = m_models[m_gameLevel].first->getRowCount();
    int cols = m_models[m_gameLevel].first->getColumnCount();

    // Get protagonist position in the world = start position of the pathfinder
    auto pos = static_cast<GameObject *>(m_protagonist->parent())->getData(DataRole::Position).toPoint();

    Comparator<Node> comp = [](const Node &a, const Node &b) {
        return a.h > b.h;
    };
    // Check for non valid input position
    if(x >= cols || y >= rows || x < 0 || y < 0) {
        y = rows - 1;
        x = cols - 1;
    }

    auto *start = &nodes[cols * pos.y() + pos.x()];
    auto *dest = &nodes[cols * y + x];
    PathFinder<Node, Node> pathFinder(nodes, start, dest, comp, cols, 0.001f);

    // Call the algorithm
    auto path = pathFinder.A_star();

    executePath(path, full);

    // Run the method again in the next event loop if the game is on full auto.
    if(full) {
        QMetaObject::invokeMethod(this, "pathFinder", Qt::QueuedConnection, -1, -1);
    }
}

void GameController::updateEnergy() {
    int protagonistEnergy = m_protagonist->getData(DataRole::Energy).toInt();
    emit energyUpdated(protagonistEnergy);

    if(protagonistEnergy == 0) {
        m_gameState = State::GameOver;
        emit gameOver();
    }
}

void GameController::updateHealth() {
    int protagonistHealth = m_protagonist->getData(DataRole::Health).toInt();
    emit healthUpdated(protagonistHealth);

    if(protagonistHealth == 0) {
        m_gameState = State::GameOver;
        emit gameOver();
    }
}

void GameController::characterMove(Direction to) {
    while(m_gameState == State::Paused)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    if(m_gameState == State::Running) {
        if(auto move = m_protagonist->getBehavior<Movement>()) {
            move->stepOn(to);
            emit tick();
        }
    }
}

void GameController::characterAttack() {
    while(m_gameState == State::Paused)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    if(m_gameState == State::Running) {
        if(auto attack = m_protagonist->getBehavior<Attack>()) {
            attack->attack();
            emit tick();
        }
    }
}

void GameController::updateGameView(View view) {
    auto data = m_models[m_gameLevel].first->getAllData();
    QSharedPointer<Renderer> renderer;

    switch(view) {
    case View::Sprite:
        renderer = QSharedPointer<SpriteRenderer>::create();
        break;
    case View::Color:
        renderer = QSharedPointer<ColorRenderer>::create();
        break;
    case View::Text:
        renderer = QSharedPointer<TextRenderer>::create();
        break;
    }

    m_view->createScene(data, renderer);
    m_gameView = view;
}
