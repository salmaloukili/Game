#include "gamewindow.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QInputDialog>
#include <QSpinBox>

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::GameWindow)
    , m_timer(new QTimer(this))
    , m_startTime(QDateTime::currentDateTime().toSecsSinceEpoch())
    , m_elapsedSeconds(0) {
    // CHOOSE GAME SIZE
    QDialog dialog(this);
    QFormLayout form(&dialog);

    form.addRow(new QLabel("Select Game Size"));

    QSpinBox *colSpinBox = new QSpinBox(&dialog);
    colSpinBox->setRange(20, 2000);
    colSpinBox->setValue(40);
    form.addRow("Width", colSpinBox);

    QSpinBox *rowSpinBox = new QSpinBox(&dialog);
    rowSpinBox->setRange(20, 2000);
    colSpinBox->setValue(30);
    form.addRow("Height", rowSpinBox);

    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    if(dialog.exec() == QDialog::Accepted) {
        // START GAME
        m_controller = QPointer<GameController>(new GameController({
          colSpinBox->cleanText().toInt(),
          rowSpinBox->cleanText().toInt(),
        }));
    }

    // SETUP UI, CONTROLLER AND VIEW
    m_controller->setParent(this);
    m_ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);

    // OTHER SETUP
    initializeCommands(); // Initializes the user commands as lists and maps them to slots/methods and desriptions
    qApp->installEventFilter(this); // Event filter for diffrent modes and visualizations
    this->setFocusPolicy(Qt::StrongFocus);

    // UI INITIAL PARAMETERS
    m_ui->colour_mode->setChecked(false);
    m_ui->text_mode->setChecked(false);
    m_ui->sprite_mode->setChecked(true);
    m_ui->health->setValue(100);
    m_ui->energy->setValue(100);
    m_ui->lcdTime->display("00:00");
    m_ui->textEdit->hide();
    m_ui->plainTextEdit->hide();
    m_ui->type_command->hide();

    // ZOOM INITIAL SETUP
    m_ui->horizontalSlider->setMinimum(-37);
    m_ui->horizontalSlider->setMaximum(32);
    m_ui->horizontalSlider->setValue(-32);
    zoomBySlider(-32);

    // START GAME
    m_controller->startGame();
    m_ui->lcdHealth->display(4);
    m_ui->lcdEnemies->display(27);
    m_ui->lcdLevel->display(1);
    m_ui->graphicsView->setScene(m_controller->getView().data()); // SET SCENE ACCORDING TO LEVEL AND VISUALIZATION
    m_ui->graphicsView->show();

    // START TIMER
    m_timer->start(1000);

    // SIGNALS AND SLOTS

    // Pathfinder trigger
    connect(m_ui->path_find_trigger, &QPushButton::clicked, [this]() {
        int x = m_ui->x_path->text().toInt();
        int y = m_ui->y_path->text().toInt();
        m_controller->pathFinder(x, y);
    });
    // Execute user input commands
    QObject::connect(m_ui->textEdit, &QLineEdit::returnPressed, this, &GameWindow::processCommand);
    // Timer and pause / resume states
    connect(m_timer, &QTimer::timeout, this, &GameWindow::updateTime);
    connect(m_ui->pause, &QPushButton::clicked, this, &GameWindow::togglePause);
    connect(m_ui->automatic, &QAction::changed, m_ui->manual, &QAction::toggle);
    connect(m_ui->manual, &QAction::changed, m_ui->automatic, &QAction::toggle);
    // Zoom control
    connect(m_ui->horizontalSlider, &QSlider::valueChanged, this, &GameWindow::zoomBySlider);
    // Switch between visualizations
    connect(m_ui->sprite_mode, &QAction::triggered, this, &GameWindow::setSpriteView);
    connect(m_ui->text_mode, &QAction::triggered, this, &GameWindow::setTextualView);
    connect(m_ui->colour_mode, &QAction::triggered, this, &GameWindow::setColorView);
    // Signals from the controller
    connect(m_controller.data(), &GameController::energyUpdated, m_ui->energy, &QProgressBar::setValue);
    connect(m_controller.data(), &GameController::healthUpdated, m_ui->health, &QProgressBar::setValue);
    connect(m_controller.data(), &GameController::enemiesUpdated, this, [this](unsigned int enemies) {
        m_ui->lcdEnemies->display((int)(enemies));
    });
    connect(m_controller.data(), &GameController::healthPacksUpdated, this, [this](unsigned int healthPacks) {
        m_ui->lcdHealth->display((int)(healthPacks));
    });
    connect(m_controller.data(), &GameController::levelUpdated, this, [this](unsigned int level) {
        m_ui->lcdLevel->display((int)(level + 1));
    });
    connect(m_controller.data(), &GameController::gameOver, this, &GameWindow::gameOver);
    // Enable pathfinder button
    connect(m_ui->x_path, &QLineEdit::textChanged, this, [this]() {
        updatePathFindTriggerButton();
    });
    connect(m_ui->y_path, &QLineEdit::textChanged, this, [this]() {
        updatePathFindTriggerButton();
    });
}

void GameWindow::keyPressEvent(QKeyEvent *event) {
    if(m_controller->getState() == GameController::State::Running && m_controller->getGameView() != GameController::View::Text) {
        switch(event->key()) {
        case Qt::Key_Up:
            m_controller->characterMove(Direction::Up);
            break;
        case Qt::Key_Down:
            m_controller->characterMove(Direction::Down);
            break;
        case Qt::Key_Left:
            m_controller->characterMove(Direction::Left);
            break;
        case Qt::Key_Right:
            m_controller->characterMove(Direction::Right);
            break;
        case Qt::Key_Space:
            m_controller->characterAttack();
            break;
        default:
            QMainWindow::keyPressEvent(event);
        }
    }
}

void GameWindow::initializeCommands() {
    // Move commands
    moveCommands["l"] = {[this]() { m_controller->characterMove(Direction::Left); }, "Move Left"};
    moveCommands["r"] = {[this]() { m_controller->characterMove(Direction::Right); }, "Move Right"};
    moveCommands["u"] = {[this]() { m_controller->characterMove(Direction::Up); }, "Move Up"};
    moveCommands["d"] = {[this]() { m_controller->characterMove(Direction::Down); }, "Move Down"};
    moveCommands["a"] = {[this]() { m_controller->characterAttack(); }, "Attack"};

    // View commands
    viewCommands["t"] = {[this]() { setTextualView(); }, "Switch to Textual View"};
    viewCommands["c"] = {[this]() { setColorView(); }, "Switch to Color View"};
    viewCommands["s"] = {[this]() { setSpriteView(); }, "Switch to Sprite View"};

    // Game commands
    gameCommands["q"] = {[this]() { QApplication::quit(); }, "Quit Game"};
    gameCommands["p"] = {[this]() { togglePause(); }, "Pause/Resume Game"};
    gameCommands["r"] = {[this]() {
                             QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
                             QApplication::quit();
                         },
                         "Restart Game"};

    // Zoom commands
    zoomCommands["+"] = {[this]() {
                             int maxZoom = m_ui->horizontalSlider->maximum();
                             m_ui->horizontalSlider->setValue(maxZoom);
                             zoomBySlider(maxZoom);
                         },
                         "Zoom In"};
    zoomCommands["-"] = {[this]() {
                             int minZoom = m_ui->horizontalSlider->minimum();
                             m_ui->horizontalSlider->setValue(minZoom);
                             zoomBySlider(minZoom);
                         },
                         "Zoom Out"};
}

void GameWindow::processCommand() {
    QString errorMessage = "Invalid command. Type '?' for a list of possible commands.";
    QString command = m_ui->textEdit->text().trimmed().toLower();

    if(command == '?') {
        showHelp();
    } else {
        QStringList commandParts = command.split(' ');

        if(commandParts.size() == 4 && commandParts[0] == "go" && commandParts[1] == "to") {
            bool okX, okY;
            int x = commandParts[2].toInt(&okX);
            int y = commandParts[3].toInt(&okY);

            if(okX && okY) {
                m_controller->pathFinder(x, y);
            }
        } else if(commandParts.size() == 2) {
            QString commandType = commandParts[0];
            QString commandAction = commandParts[1];

            if(commandType == "move" && moveCommands.contains(commandAction)) {
                moveCommands[commandAction].first();
            } else if(commandType == "view" && viewCommands.contains(commandAction)) {
                viewCommands[commandAction].first();
            } else if(commandType == "game" && gameCommands.contains(commandAction)) {
                gameCommands[commandAction].first();
            } else if(commandType == "zoom" && zoomCommands.contains(commandAction)) {
                zoomCommands[commandAction].first();
            } else {
                m_ui->plainTextEdit->setPlainText(errorMessage);
            }
        } else {
            m_ui->plainTextEdit->setPlainText(errorMessage);
        }
    }
    m_ui->textEdit->clear();
}

void GameWindow::showHelp() {
    QString helpMessage = "Available commands:\n";
    // Move Commands
    helpMessage += "\nMove Commands:\n";
    for(const auto &cmd : moveCommands.keys()) {
        helpMessage += "move " + cmd + " - " + moveCommands[cmd].second + "\n";
    }
    // View Commands
    helpMessage += "\nView Commands:\n";
    for(const auto &cmd : viewCommands.keys()) {
        helpMessage += "view " + cmd + " - " + viewCommands[cmd].second + "\n";
    }
    // Game Commands
    helpMessage += "\nGame Commands:\n";
    for(const auto &cmd : gameCommands.keys()) {
        helpMessage += "game " + cmd + " - " + gameCommands[cmd].second + "\n";
    }
    // Zoom Commands
    helpMessage += "\nZoom Commands:\n";
    for(const auto &cmd : zoomCommands.keys()) {
        helpMessage += "zoom " + cmd + " - " + zoomCommands[cmd].second + "\n";
    }

    helpMessage += "\ngo to x y - Moves the protagonist to the specified coordinates x and y.\n If non-valid coordinates, protagonist moves automatically to the doorway (next level)";
    m_ui->plainTextEdit->setPlainText(helpMessage);
}

void GameWindow::updateLevel(unsigned int level, unsigned int enemies, unsigned int health_packs) {
    m_ui->lcdLevel->display((int)level);
    m_controller->startGame();
    m_ui->lcdEnemies->display((int)enemies);
    m_ui->lcdHealth->display((int)health_packs);
}

void GameWindow::setSpriteView() {
    m_controller->updateGameView(GameController::View::Sprite);
    m_ui->colour_mode->setChecked(false);
    m_ui->text_mode->setChecked(false);
    m_ui->sprite_mode->setChecked(true);
    m_ui->textEdit->hide();
    m_ui->plainTextEdit->hide();
    m_ui->type_command->hide();
    m_ui->path_find_trigger->show();
    m_ui->x_path->show();
    m_ui->y_path->show();
    m_ui->label->show();
    m_ui->label_2->show();
    m_ui->label_3->show();
}
void GameWindow::setTextualView() {
    m_controller->updateGameView(GameController::View::Text);
    m_ui->colour_mode->setChecked(false);
    m_ui->text_mode->setChecked(true);
    m_ui->sprite_mode->setChecked(false);
    m_ui->textEdit->show();
    m_ui->plainTextEdit->show();
    m_ui->type_command->show();
    m_ui->path_find_trigger->hide();
    m_ui->x_path->hide();
    m_ui->y_path->hide();
    m_ui->label->hide();
    m_ui->label_2->hide();
    m_ui->label_3->hide();
}
void GameWindow::setColorView() {
    m_controller->updateGameView(GameController::View::Color);
    m_ui->colour_mode->setChecked(true);
    m_ui->text_mode->setChecked(false);
    m_ui->sprite_mode->setChecked(false);
    m_ui->textEdit->hide();
    m_ui->plainTextEdit->hide();
    m_ui->type_command->hide();
    m_ui->path_find_trigger->show();
    m_ui->x_path->show();
    m_ui->y_path->show();
    m_ui->label->show();
    m_ui->label_2->show();
    m_ui->label_3->show();
}

void GameWindow::togglePause() {
    if(m_controller->getState() == GameController::State::Paused) {
        connect(m_timer, &QTimer::timeout, this, &GameWindow::updateTime);
        m_controller->setState(GameController::State::Running);
        m_ui->pause->setText("Pause Game");
    } else {
        disconnect(m_timer, &QTimer::timeout, this, &GameWindow::updateTime);
        m_controller->setState(GameController::State::Paused);
        m_ui->pause->setText("Resume Game");
    }
}
void GameWindow::updateTime() {
    m_elapsedSeconds++;
    m_ui->lcdTime->display(QDateTime::fromSecsSinceEpoch(m_elapsedSeconds).toUTC().toString("mm:ss"));
}

void GameWindow::gameOver() {
    QString summary;
    summary += "Game Summary:\n";
    summary += "Level Reached: " + QString::number(m_ui->lcdLevel->intValue()) + "\n";
    summary += "Final Health: " + QString::number(m_ui->health->value()) + "\n";
    summary += "Final Energy: " + QString::number(m_ui->energy->value()) + "\n";

    QString totalTime = QString::number(m_elapsedSeconds);
    summary += "Total Time: " + totalTime + " seconds\n";

    QString gameOverText = "Game Over\nThanks for playing!\n\n" + summary;

    QMessageBox gameOverBox;
    gameOverBox.setWindowTitle("Game Over");
    gameOverBox.setText(gameOverText);

    QAbstractButton *playAgainButton = gameOverBox.addButton("Play Again", QMessageBox::YesRole);
    QAbstractButton *quitButton = gameOverBox.addButton("Quit", QMessageBox::NoRole);

    gameOverBox.exec();

    if(gameOverBox.clickedButton() == playAgainButton) {
        QCoreApplication::quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    } else if(gameOverBox.clickedButton() == quitButton) {
        QApplication::quit();
    }
}

void GameWindow::zoomBySlider(int value) {
    qreal scaleFactor = 1.0 + (value / 50.0);
    m_ui->graphicsView->resetTransform();
    m_ui->graphicsView->scale(scaleFactor, scaleFactor);
}

bool GameWindow::eventFilter(QObject *watched, QEvent *event) {
    if(m_controller->getGameView() != GameController::View::Text && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(m_ui->x_path->hasFocus() || m_ui->y_path->hasFocus()) {
            return QObject::eventFilter(watched, event);
        } else {
            keyPressEvent(keyEvent);
            return true;
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void GameWindow::updatePathFindTriggerButton() {
    bool okX, okY;
    m_ui->x_path->text().toInt(&okX);
    m_ui->y_path->text().toInt(&okY);
    bool enableButton = okX && okY;
    m_ui->path_find_trigger->setEnabled(enableButton);
}

void GameWindow::closeEvent(QCloseEvent *event) {
    // Perform the same actions as quit button
    m_controller->setState(GameController::State::GameOver);
    QMainWindow::closeEvent(event);
}

// DESTRUCTOR

GameWindow::~GameWindow() {
    delete m_ui;
    delete m_timer;
}
