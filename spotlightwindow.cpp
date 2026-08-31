#include "spotlightwindow.h"
#include "styles.h"
#include <QGuiApplication>
#include <QScreen>
#include <QSettings>

SpotlightWindow::SpotlightWindow(QWidget *parent)
    : QWidget(parent), searchEngine(this)
{
    // Make window borderless, floating, and hidden from taskbar
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    setFixedWidth(650);

    // Outer layout with zero margins
    outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);

    // Inner Container Frame
    containerFrame = new QFrame(this);
    containerFrame->setObjectName("ContainerFrame");
    outerLayout->addWidget(containerFrame);

    // Layout inside container
    innerLayout = new QVBoxLayout(containerFrame);
    innerLayout->setContentsMargins(16, 16, 16, 16);
    innerLayout->setSpacing(12);

    // Search Input Bar
    searchBar = new QLineEdit(containerFrame);
    searchBar->setPlaceholderText("Search apps, files, and actions");
    searchBar->setFixedHeight(40);
    searchBar->setFocus();
    innerLayout->addWidget(searchBar);

    // Results List View & Model
    resultsList = new QListView(containerFrame);
    resultsList->setFixedHeight(200);

    resultsModel = new QStringListModel(this);
    resultsList->setModel(resultsModel);
    resultsList->hide();

    innerLayout->addWidget(resultsList);

    // Load dynamic theme stylesheet from QSettings
    QSettings configSettings("SpotlightApp", "Config");
    QString savedTheme = configSettings.value("theme", "Dark").toString();
    setTheme(savedTheme);

    // Connect text updates and result selections
    connect(searchBar, &QLineEdit::textChanged, this, &SpotlightWindow::onSearchTextChanged);
    connect(searchBar, &QLineEdit::returnPressed, this, [this]() {
        QModelIndex topIndex = resultsModel->index(0, 0);
        if (topIndex.isValid()) {
            onResultActivated(topIndex);
        }
    });
    connect(resultsList, &QListView::activated, this, &SpotlightWindow::onResultActivated);

    adjustSize();
}

void SpotlightWindow::setTheme(const QString &themeName) {
    setStyleSheet(ThemeManager::getStyleSheet(themeName));
}

void SpotlightWindow::toggleVisibility() {
    if (isVisible()) {
        hide();
    } else {
        searchBar->clear();
        resultsList->hide();
        adjustSize();

        centerOnScreen();
        show();
        raise();
        activateWindow();
        searchBar->setFocus();
    }
}

void SpotlightWindow::centerOnScreen() {
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    if (!primaryScreen) return;

    QRect screenGeometry = primaryScreen->availableGeometry();
    int x = screenGeometry.x() + (screenGeometry.width() - width()) / 2;
    int y = screenGeometry.y() + (screenGeometry.height() * 0.25);

    move(x, y);
}

void SpotlightWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        hide();
    } else if (event->key() == Qt::Key_Down && resultsList->isVisible()) {
        resultsList->setFocus();
        resultsList->setCurrentIndex(resultsModel->index(0, 0));
    } else {
        QWidget::keyPressEvent(event);
    }
}

void SpotlightWindow::onSearchTextChanged(const QString &text) {
    if (text.trimmed().isEmpty()) {
        resultsList->hide();
    } else {
        QStringList results = searchEngine.getResults(text);
        resultsModel->setStringList(results);

        if (!results.isEmpty()) {
            resultsList->show();
        } else {
            resultsList->hide();
        }
    }
    adjustSize();
    centerOnScreen();
}

void SpotlightWindow::onResultActivated(const QModelIndex &index) {
    QString selectedResult = resultsModel->data(index, Qt::DisplayRole).toString();

    searchEngine.executeAction(selectedResult);

    hide();
}