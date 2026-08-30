#ifndef FILEINDEXER_H
#define FILEINDEXER_H

#include <QObject>
#include <QStringList>
#include <QMutex>

struct IndexedApp {
    QString name;
    QString exec;
    QString icon;
};

class FileIndexer : public QObject {
    Q_OBJECT

public:
    explicit FileIndexer(QObject *parent = nullptr);
    ~FileIndexer() = default;

    void rebuildIndex();
    QStringList getIndexedFiles() const;
    QList<IndexedApp> getIndexedApps() const;

signals:
    void indexingStarted();
    void indexingProgress(int current, int total);
    void indexingFinished(int totalFiles);

private:
    void parseDesktopFile(const QString &filePath, QList<IndexedApp> &appList);

    QStringList m_fileCache;
    QList<IndexedApp> m_appCache;
    mutable QMutex m_mutex;
};

#endif // FILEINDEXER_H