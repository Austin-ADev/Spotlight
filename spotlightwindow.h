#ifndef SPOTLIGHTWINDOW_H
#define SPOTLIGHTWINDOW_H

#include <QWidget>
#include <QFrame>
#include <QLineEdit>
#include <QListView>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QStringListModel>
#include "searchengine.h"

class SpotlightWindow : public QWidget {
    Q_OBJECT

public:
    explicit SpotlightWindow(QWidget *parent = nullptr);
    void toggleVisibility();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onSearchTextChanged(const QString &text);
    void onResultActivated(const QModelIndex &index);

private:
    QFrame *containerFrame;
    QLineEdit *searchBar;
    QListView *resultsList;
    QVBoxLayout *outerLayout;
    QVBoxLayout *innerLayout;
    QStringListModel *resultsModel;

    SearchEngine searchEngine;

    void centerOnScreen();
};

#endif // SPOTLIGHTWINDOW_H