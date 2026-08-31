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

#include "spotlightwindow.h"
#include <QGuiApplication>
#include <QScreen>
#include <QSettings>
#include <QKeyEvent>
#include <QStyleOption>
#include <QPainter>
#include <QStyle>
#include <QEvent>
#include <QMouseEvent>

SpotlightWindow::SpotlightWindow(QWidget *parent)
    : QWidget(parent), searchEngine(this)
{
    // Use Qt::ToolTip or Qt::Window to ensure X11 accepts window focus state properly
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);

    outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSizeConstraint(QLayout::SetFixedSize);

    containerFrame = new QFrame(this);
    containerFrame->setObjectName("ContainerFrame");
    containerFrame->setFixedWidth(650);
    outerLayout->addWidget(containerFrame);

    innerLayout = new QVBoxLayout(containerFrame);
    innerLayout->setContentsMargins(16, 16, 16, 16);
    innerLayout->setSpacing(12);

    searchBar = new QLineEdit(containerFrame);
    searchBar->setPlaceholderText("Search apps, files, and actions");
    searchBar->setFixedHeight(40);
    innerLayout->addWidget(searchBar);

    resultsList = new QListView(containerFrame);
    resultsList->setFixedHeight(200);
    resultsList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);

    resultsModel = new QStringListModel(this);
    resultsList->setModel(resultsModel);
    resultsList->hide();

    innerLayout->addWidget(resultsList);

    connect(searchBar, &QLineEdit::textChanged, this, &SpotlightWindow::onSearchTextChanged);
    connect(searchBar, &QLineEdit::returnPressed, this, [this]() {
        QModelIndex topIndex = resultsModel->index(0, 0);
        if (topIndex.isValid()) {
            onResultActivated(topIndex);
        }
    });
    connect(resultsList, &QListView::activated, this, &SpotlightWindow::onResultActivated);
}

void SpotlightWindow::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    qApp->installEventFilter(this);
}

void SpotlightWindow::hideEvent(QHideEvent *event) {
    QWidget::hideEvent(event);
    qApp->removeEventFilter(this);
}

bool SpotlightWindow::eventFilter(QObject *watched, QEvent *event) {
    // Intercept mouse clicks anywhere in the app/screen
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        // Check if the click happened outside this window's geometry
        if (!geometry().contains(mouseEvent->globalPosition().toPoint())) {
            hide();
            return true; // Consume the event
        }
    }

    // Intercept loss of window focus
    if (event->type() == QEvent::ApplicationDeactivate || event->type() == QEvent::WindowDeactivate) {
        hide();
    }

    return QWidget::eventFilter(watched, event);
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
    QScreen *targetScreen = QGuiApplication::screenAt(QCursor::pos());
    if (!targetScreen) {
        targetScreen = QGuiApplication::primaryScreen();
    }

    if (!targetScreen) return;

    QRect screenGeometry = targetScreen->availableGeometry();
    int x = screenGeometry.x() + (screenGeometry.width() - width()) / 2;
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

void SpotlightWindow::changeEvent(QEvent *event) {
    if (event->type() == QEvent::ActivationChange) {
        if (!isActiveWindow() && isVisible()) {
            hide();
        }
    }
    QWidget::changeEvent(event);
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

    centerOnScreen();
}

void SpotlightWindow::onResultActivated(const QModelIndex &index) {
    QString selectedResult = resultsModel->data(index, Qt::DisplayRole).toString();
    searchEngine.executeAction(selectedResult);
    hide();
}