#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QGraphicsView>
#include <QMainWindow>
#include <QSharedPointer>
#include <QElapsedTimer>
#include <QKeyEvent>
#include <QProcess>
#include <QTimer>
#include <iostream>
#include <QTime>
#include <qgraphicsitem.h>
#include <QMessageBox>
#include <QDateTime>

#include "controller/gamecontroller.h"
#include "ui_gamewindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class GameWindow;
}
QT_END_NAMESPACE

/**
 * @brief The GameWindow class is the main window for the game.
 */
class GameWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief GameWindow Inherits from QMainWindow and controls the UI in function of the GameController signals.
     * @param parent is a null pointer.
     */
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();
    /**
     * @brief Getter for controller.
     */
    QPointer<GameController> getController() { return m_controller; }
    /**
     * @brief Getter for the UI.
     */
    Ui::GameWindow *getUI() { return m_ui; }
    /**
     * @brief Getter for the timer.
     */
    QTimer *getTimer() { return m_timer; }

private:
    /**
     * @brief eventFilter Filters what events should be captured by the Window depending on the state of the game.
     * @param watched.
     * @param event.
     * @return if the filter was successful.
     */
    bool eventFilter(QObject *watched, QEvent *event) override;
    /**
     * @brief showHelp displays overview of possible commands for the user.
     */
    void showHelp();
    /**
     * @brief initializeCommands maps all the commands into groups and sets their descriptions and corresponding methods.
     */
    void initializeCommands();
    /**
     * @brief updatePathFindTriggerButton Enables path finder button only if valid x and y coordinates filled in.
     */
    void updatePathFindTriggerButton();
    /**
     * @brief m_ui raw pointer to the UI.
     */
    Ui::GameWindow *m_ui;
    /**
     * @brief m_controller GameController inherits from QGraphicsView.
     */
    QPointer<GameController> m_controller;
    /**
     * @brief m_timer counts time as long as the window is open.
     */
    QTimer *m_timer;
    /**
     * @brief moveCommands All the move commands mapped into their description along with their corresponding methods.
     */
    QMap<QString, std::pair<std::function<void()>, QString>> moveCommands;
    /**
     * @brief viewCommands All the commands related to switching between visualizations mapped into their description along with their corresponding methods.
     */
    QMap<QString, std::pair<std::function<void()>, QString>> viewCommands;
    /**
     * @brief gameCommands All the game commands mapped into their description along with their corresponding methods.
     */
    QMap<QString, std::pair<std::function<void()>, QString>> gameCommands;
    /**
     * @brief zoomCommands All the zoom commands mapped into their description along with their corresponding methods.
     */
    QMap<QString, std::pair<std::function<void()>, QString>> zoomCommands;
    // Time variables
    int m_startTime, m_elapsedSeconds;

public slots:
    // UI AND GAME STATE
    /**
     * @brief updateTime updates the time every second and displays it in the UI.
     */
    void updateTime();
    /**
     * @brief processCommand handles user input commands.
     */
    void processCommand();
    /**
     * @brief zoomBySlider connected to UI slider input signal which passes the value as parameter.
     * @param value used to calculates the zoom factor.
     */
    void zoomBySlider(int value);
    /**
     * @brief togglePause toggle between pause and resume, starts and stops timer accordingly, captures UI input button for pausing.
     */
    void togglePause();
    /**
     * @brief updateLevel updates game level, captured by controller.
     * @param level new level.
     * @param enemies number of enemies.
     * @param health_packs number of health packs.
     */
    void updateLevel(unsigned int level, unsigned int enemies, unsigned int health_packs);
    /**
     * @brief gameOver displays summary pop up message when the game is over, captures controller signal.
     */
    void gameOver();
    /**
     * @brief closeEvent Overrides CloseEvent to make sure the process is killed when the X button is clicked.
     * @param event.
     */
    void closeEvent(QCloseEvent *event) override;

    // VISUALIZATION
    /**
     * @brief setSpriteView sets the UI parameters for the sprites visualization and calls corresponding controller method.
     */
    void setSpriteView();
    /**
     * @brief setTextualView sets the UI parameters for the text visualization and calls corresponding controller method.
     */
    void setTextualView();
    /**
     * @brief setColorView sets the UI parameters for the color visualization and calls corresponding controller method.
     */
    void setColorView();

protected:
    /**
     * @brief keyPressEvent handles user key presses and calls the corresponding methods in the controller.
     * @param event.
     */
    void keyPressEvent(QKeyEvent *event) override;
};
#endif // GAMEWINDOW_H
