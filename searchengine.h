#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <QObject>
#include <QStringList>
#include "fileindexer.h"

class SearchEngine : public QObject {
    Q_OBJECT

public:
    explicit SearchEngine(QObject *parent = nullptr);
    ~SearchEngine() = default;

    // Add these declarations to your public interface:
    FileIndexer* getIndexer();
    void refreshIndex();

    QStringList getResults(const QString &input);
    void executeAction(const QString &selectedResult);

private:
    QStringList handleMath(const QString &query);
    QStringList handleCommands(const QString &query);
    QStringList handleGodot(const QString &query);
    QStringList handleFileSearch(const QString &query);

    void openSettingsWindow();
    void launchTerminal();
    void toggleWifi(bool enable);
    void openFile(const QString &filePath);

    FileIndexer m_indexer;
};

#endif // SEARCHENGINE_H