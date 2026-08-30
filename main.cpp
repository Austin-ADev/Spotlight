#include <QApplication>
#include <QWindow>
#include "spotlightwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Keep app running in background when window hides
    app.setQuitOnLastWindowClosed(false);

    SpotlightWindow spotlight;
    spotlight.hide();

    // Auto-hide when clicking outside the window
    QObject::connect(&app, &QGuiApplication::focusWindowChanged, [&spotlight](QWindow *focusedWindow) {
        if (focusedWindow == nullptr && spotlight.isVisible()) {
            spotlight.hide();
        }
    });

    spotlight.toggleVisibility();

    return app.exec();
}