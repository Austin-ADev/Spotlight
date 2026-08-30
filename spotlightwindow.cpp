#include "spotlightwindow.h"
#include <QGuiApplication>
#include <QScreen>

SpotlightWindow::SpotlightWindow(QWidget *parent)
    : QWidget(parent), searchEngine(this)
{
    // Make window borderless, floating, and hidden from the taskbar
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

    // Styling to match dark slate look
    setStyleSheet(
        "#ContainerFrame {"
        "  background-color: #1A1A1E;"
        "  border: 1px solid #2E2E35;"
        "  border-radius: 30px;"
        "}"
        "QLineEdit {"
        "  background-color: transparent;"
        "  border: none;"
        "  color: #FFFFFF;"
        "  font-size: 18px;"
        "  padding: 0px 4px;"
        "  selection-background-color: #3B82F6;"
        "}"
        "QLineEdit::placeholder {"
        "  color: #6B7280;"
        "}"
        "QListView {"
        "  background-color: transparent;"
        "  border: none;"
        "  color: #E5E7EB;"
        "  font-size: 15px;"
        "  outline: none;"
        "}"
        "QListView::item {"
        "  padding: 10px 12px;"
        "  border-radius: 8px;"
        "}"
        "QListView::item:hover {"
        "  background-color: #27272A;"
        "}"
        "QListView::item:selected {"
        "  background-color: #3F3F46;"
        "  color: #FFFFFF;"
        "}"
        "QScrollBar:vertical {"
        "  background: transparent;"
        "  width: 8px;"
        "  margin: 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "  background: #3F3F46;"
        "  border-radius: 4px;"
        "  min-height: 20px;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "  height: 0px;"
        "}"
        );

    // Connect text updates and result selections
    connect(searchBar, &QLineEdit::textChanged, this, &SpotlightWindow::onSearchTextChanged);
    connect(searchBar, &QLineEdit::returnPressed, this, [this]() {
        // Execute top item if Enter is pressed inside the search bar
        QModelIndex topIndex = resultsModel->index(0, 0);
        if (topIndex.isValid()) {
            onResultActivated(topIndex);
        }
    });
    connect(resultsList, &QListView::activated, this, &SpotlightWindow::onResultActivated);

    adjustSize();
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
        // Allow Arrow Down to move focus into the list
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
        // Fetch prefix-filtered list from SearchEngine
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

    // Send selected string back to SearchEngine to execute actions
    searchEngine.executeAction(selectedResult);

    // Automatically hide Spotlight overlay after execution
    hide();
}