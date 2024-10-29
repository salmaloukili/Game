#include <QApplication>
#include <QProcess>
#include <QFile>

#include "ui_gamewindow.h"
#include "view/gamewindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Set Dark Mode
    QFile f(":qdarkstyle/dark/darkstyle.qss");
    f.open(QFile::ReadOnly | QFile::Text);
    QTextStream ts(&f);
    qApp->setStyleSheet(ts.readAll());

    // Make qDebug output function and line
    qSetMessagePattern("%{function}(%{line}): %{message}");

    // Create the main classes of the game: GameWindow, GameView, GameController
    GameWindow w;

    // connect quit and rerun game

    QObject::connect(w.getUI()->quit_game, &QPushButton::clicked, &app, [&w] {
        w.getController()->setState(GameController::State::GameOver);
        QCoreApplication::quit();
    });

    QObject::connect(w.getUI()->rerun_game_2, &QPushButton::clicked, [] {
        QCoreApplication::quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    });

    w.show();
    return app.exec();
}
