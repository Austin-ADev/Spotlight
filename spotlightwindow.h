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

#ifndef SPOTLIGHTWINDOW_H
#define SPOTLIGHTWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QListView>
#include <QStringListModel>
#include <QVBoxLayout>
#include <QFrame>
#include <QKeyEvent>
#include <QPaintEvent>
#include "searchengine.h"

class SpotlightWindow : public QWidget {
    Q_OBJECT

public:
    explicit SpotlightWindow(QWidget *parent = nullptr);
    void toggleVisibility();
    void centerOnScreen();

public slots:
    void setTheme(const QString &themeName);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void changeEvent(QEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private slots:
    void onSearchTextChanged(const QString &text);
    void onResultActivated(const QModelIndex &index);

private:
    QVBoxLayout *outerLayout;
    QFrame *containerFrame;
    QVBoxLayout *innerLayout;
    QLineEdit *searchBar;
    QListView *resultsList;
    QStringListModel *resultsModel;

    SearchEngine searchEngine;
};

#endif // SPOTLIGHTWINDOW_H