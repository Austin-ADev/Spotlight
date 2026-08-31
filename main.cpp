#include <QApplication>
#include <QWidget>        // Added to fix QWidget parameter errors
#include <QLocalServer>
#include <QLocalSocket>
#include <QSettings>
#include <QDebug>
#include "spotlightwindow.h"
#include "styles.h"

int main(int argc, char *argv[]) {
#if defined(Q_OS_LINUX)
    qputenv("QT_QPA_PLATFORM", "xcb");
#endif

    QApplication app(argc, argv);

    // Unique server key for single instance locking
    const QString serverName = "SpotlightApp_SingleInstance_Lock";

    // 1. Try connecting to an existing running instance
    QLocalSocket socket;
    socket.connectToServer(serverName);

    if (socket.waitForConnected(500)) {
        // An instance is ALREADY running! Send a message to wake it up, then exit immediately.
        socket.write("TOGGLE");
        socket.waitForBytesWritten(1000);
        socket.disconnectFromServer();
        return 0; // Kills this extra process before it creates a window
    }

    // 2. No instance was found running. We are the main primary process!
    // Clean up any old leftover sockets from previous crashes
    QLocalServer::removeServer(serverName);

    QLocalServer server;
    if (!server.listen(serverName)) {
        qWarning() << "Could not start local IPC server lock.";
    }

    QApplication::setQuitOnLastWindowClosed(false);

    QSettings configSettings("SpotlightApp", "Config");
    QString savedTheme = configSettings.value("theme", "Dark").toString();
    app.setStyleSheet(ThemeManager::getStyleSheet(savedTheme));

    SpotlightWindow spotlight;
    spotlight.ensurePolished();
    spotlight.adjustSize();
    spotlight.centerOnScreen();
    spotlight.show();
    spotlight.raise();
    spotlight.activateWindow();

    // 3. Listen for future launch attempts and toggle visibility when they connect
    QObject::connect(&server, &QLocalServer::newConnection, [&server, &spotlight]() {
        QLocalSocket *clientConnection = server.nextPendingConnection();
        if (clientConnection) {
            QObject::connect(clientConnection, &QLocalSocket::readyRead, [clientConnection, &spotlight]() {
                QByteArray message = clientConnection->readAll();
                if (message == "TOGGLE") {
                    spotlight.toggleVisibility();
                }
            });
        }
    });

    return app.exec();
}