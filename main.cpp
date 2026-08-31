/*
 * Copyright (C) 2026 Austin ADev
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <QApplication>
#include <QWidget>
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

    const QString serverName = "SpotlightApp_SingleInstance_Lock";

    QLocalSocket socket;
    socket.connectToServer(serverName);

    if (socket.waitForConnected(500)) {
        socket.write("TOGGLE");
        socket.waitForBytesWritten(1000);
        socket.disconnectFromServer();
        return 0;
    }

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