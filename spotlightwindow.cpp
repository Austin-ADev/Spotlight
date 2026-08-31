#include "spotlightwindow.h"
#include <QGuiApplication>
#include <QScreen>
#include <QSettings>
#include <QKeyEvent>
#include <QStyleOption>
#include <QPainter>
#include <QStyle>

SpotlightWindow::SpotlightWindow(QWidget *parent)
    : QWidget(parent), searchEngine(this)
{
    // Make window borderless, floating, translucent, and hidden from taskbar
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // Outer layout with zero margins
    outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);

    // CRITICAL: Forces window to auto-shrink to visible children size
    outerLayout->setSizeConstraint(QLayout::SetFixedSize);

    // Inner Container Frame
    containerFrame = new QFrame(this);
    containerFrame->setObjectName("ContainerFrame");
    containerFrame->setFixedWidth(650); // Set width on container frame instead of top widget
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

    // Ignore list geometry allocations when hidden
    resultsList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);

    resultsModel = new QStringListModel(this);
    resultsList->setModel(resultsModel);
    resultsList->hide();

    innerLayout->addWidget(resultsList);

    // Connect text updates and result selections
    connect(searchBar, &QLineEdit::textChanged, this, &SpotlightWindow::onSearchTextChanged);
    connect(searchBar, &QLineEdit::returnPressed, this, [this]() {
        QModelIndex topIndex = resultsModel->index(0, 0);
        if (topIndex.isValid()) {
            onResultActivated(topIndex);
        }
    });
    connect(resultsList, &QListView::activated, this, &SpotlightWindow::onResultActivated);
}

void SpotlightWindow::setTheme(const QString &themeName) {
    Q_UNUSED(themeName);
}

void SpotlightWindow::toggleVisibility() {
    if (isVisible()) {
        hide();
    } else {
        searchBar->clear();
        resultsList->hide();

        show();
        centerOnScreen();

        raise();
        activateWindow();
        searchBar->setFocus();
    }
}

void SpotlightWindow::centerOnScreen() {
    // Get the screen where the mouse cursor currently sits
    QScreen *targetScreen = QGuiApplication::screenAt(QCursor::pos());
    if (!targetScreen) {
        targetScreen = QGuiApplication::primaryScreen();
    }

    if (!targetScreen) return;

    QRect screenGeometry = targetScreen->availableGeometry();

    // Horizontally centered
    int x = screenGeometry.x() + (screenGeometry.width() - width()) / 2;

    // Position vertically at ~25% from the top of the available screen area (macOS style)
    int y = screenGeometry.y() + static_cast<int>(screenGeometry.height() * 0.25);

    move(x, y);
}

void SpotlightWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
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

    // Keep top edge fixed at 25% height while window resizes downwards
    centerOnScreen();
}

void SpotlightWindow::onResultActivated(const QModelIndex &index) {
    QString selectedResult = resultsModel->data(index, Qt::DisplayRole).toString();

    searchEngine.executeAction(selectedResult);

    hide();
}
void SpotlightWindow::focusOutEvent(QFocusEvent *event) {
    QWidget::focusOutEvent(event);

    // Hide the window when focus is lost instead of killing the application process
    hide();
}