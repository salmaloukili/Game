#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QThread>
#include <QCoreApplication>
#include <QCoreApplication>
#include <qdatetime.h>
#include <QDateTime>

#include "node.h"
#include "model/gameobjectmodel.h"
#include "view/gameview.h"

/**
 * @brief The GameController class holds all the models and the view. It directs the user input and the game state.
 */
class GameController : public QGraphicsView {
    Q_OBJECT
public:
    /**
     * @brief GameController controls the state of the game, has instance of the model data.
     */
    GameController(QSize size = {40, 25})
        : QGraphicsView()
        , m_gameLevel(0)
        , m_gameState(State::Running)
        , m_gameView(View::Sprite)
        , m_levelSize(size) {};
    /**
     * @brief The State enum, Game states can be Running, Paused or GameOver.
     */
    enum class State {
        Running,
        Paused,
        GameOver,
    };
    /**
     * @brief The View enum, Game visualizations can be Text, Color or Sprites.
     */
    enum class View {
        Text,
        Sprite,
        Color,
    };
    /**
     * @brief startGame starts the game at level 1.
     */
    void startGame();
    /**
     * @brief updateEnergy retrieves lastest protagonist energy level upon data change and triggers corresponding slot in the GameWindow.
     */
    void updateEnergy();
    /**
     * @brief updateHealth retrieves lastest protagonist health level upon data change and triggers corresponding slot in the GameWindow.
     */
    void updateHealth();
    /**
     * @brief characterMove call model behavior to move the protagonist.
     * @param to in which direction to move.
     */
    void characterMove(Direction to);
    /**
     * @brief characterAtttack attacks enemy in current protagonist direction.
     */
    void characterAttack();
    /**
     * @brief updateLevel handles going up and down the levels (upon stepping on doorways), captures data change in the model and triggers GameWindow.
     * @param direction UP to go to next level, DOWN to go to previous level, parameter passed from data change.
     */
    void updateLevel(Direction direction);
    /**
     * @brief updateGameView calls the new visualization renderer of the scene upon switching views, captures data change in the model and triggers GameWindow.
     * @param view Text, Color or Sprite.
     */
    void updateGameView(View view);
    /**
     * @brief dataChanged captures any model changes and filters to call corresponding methods.
     * @param objectData the data that changed.
     */
    void dataChanged(QMap<DataRole, QVariant> objectData);
    /**
     * @brief emitLevelUpdates emits changing level, health packs and enemies signals upon changing levels, signals captured by GameWindow.
     */
    void emitLevelUpdates();
    /**
     * @brief createNewLevel creates new level upon going up levels.
     * @param level the level number.
     */
    void createNewLevel(int level);
    /**
     * @brief automaticAttack Attack function used by the pathfinder, to automatically attack enemies in the path or when the enrgy is low.
     */
    void automaticAttack();
    /**
     * @brief executePath Executes the moves returned y the pathfinder.
     * @param path to take.
     * @param fully Boolean indicating whether or not to keep executing throughout new levels, so keep finding for the rest of the game.
     */
    void executePath(std::vector<int> path, bool fully = false);
    ///@{
    /**
     * @brief Getters and setters
     **/
    void setState(State new_state) { m_gameState = new_state; }
    void setView(QSharedPointer<GameView> view) { m_view = view; } // GameView
    State getState() { return m_gameState; }
    QSharedPointer<GameView> getView() { return m_view; } // GameView
    View getGameView() { return m_gameView; } // Visualization enum
    ///@}
public slots:
    /**
     * @brief pathFinder automatically routes the protagonist to given position using the algorithm for the shortest path.
     * @param x coordinate of the world grid.
     * @param y coordinate of the world grid.
     */
    void pathFinder(int x = -1, int y = -1);

signals:
    /**
     * @brief tick Emitted when a turn is complete.
     */
    void tick();
    /**
     * @brief gameOver This is emitted when the game is over.
     */
    void gameOver();
    /**
     * @brief energyUpdated Emitted whenever the protagonist energy level changes, captured by GameWindow to be displayed in the UI.
     * @param energy amount of energy
     */
    void energyUpdated(int energy);
    /**
     * @brief healthUpdated Emitted whenever the protagonist health level changes, captured by GameWindow to be displayed in the UI.
     * @param health amount of health
     */
    void healthUpdated(int health);
    /**
     * @brief enemiesUpdated Emitted when the level of the game changes and thus the number of enemies for that level, captured by GameWindow to be displayed in the UI.
     * @param enemies number of enemies
     */
    void enemiesUpdated(int enemies);
    /**
     * @brief healthPacksUpdated Emitted when the level of the game changes and thus the number of health packs for that level, captured by GameWindow to be displayed in the UI.
     * @param health_packs number of health packs
     */
    void healthPacksUpdated(int health_packs);
    /**
     * @brief levelUpdated Emitted when the level of the game changes, captured by GameWindow to be displayed in the UI.
     * @param level
     */
    void levelUpdated(int level);

private:
    /**
     * @brief m_model List of the different game models for different levels, holds all game data and logic.
     */
    QList<QPair<GameObjectModel *, std::vector<Node>>> m_models;
    /**
     * @brief m_view The scene of the controller.
     */
    QSharedPointer<GameView> m_view;
    /**
     * @brief m_character The protagonist of the game.
     */
    QPointer<GameObject> m_protagonist;
    /**
     * @brief m_gameLevel current game level.
     */
    int m_gameLevel;
    /**
     * @brief m_gameState current game state.
     */
    State m_gameState;
    /**
     * @brief m_gameView current game visualization (Text, Color, Sprite).
     */
    View m_gameView;
    /**
     * @brief m_health_packs Number of current health packs in the game.
     */
    int m_health_packs;
    /**
     * @brief m_enemies Number of current enemies in the game.
     */
    int m_enemies;
    /**
     * @brief m_levelSize Size of the levels.
     */
    QSize m_levelSize;
    /**
     * @brief disconnectCurrentModel disconnects current model upon changing levels.
     */
    void disconnectCurrentModel();
    /**
     * @brief connectCurrentModel connects new level model.
     */
    void connectCurrentModel();
};

#endif // GAMECONTROLLER_H
