#ifndef SPOTLIGHTWINDOW_H
#define SPOTLIGHTWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QListView>
#include <QStringListModel>
#include <QVBoxLayout>
#include <QFrame>
#include <QKeyEvent>
#include "searchengine.h"

class SpotlightWindow : public QWidget {
    Q_OBJECT

public:
    explicit SpotlightWindow(QWidget *parent = nullptr);
    void toggleVisibility();

public slots:
    void setTheme(const QString &themeName);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onSearchTextChanged(const QString &text);
    void onResultActivated(const QModelIndex &index);

private:
    void centerOnScreen();

    QVBoxLayout *outerLayout;
    QFrame *containerFrame;
    QVBoxLayout *innerLayout;
    QLineEdit *searchBar;
    QListView *resultsList;
    QStringListModel *resultsModel;

    SearchEngine searchEngine;
};

#endif // SPOTLIGHTWINDOW_H