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