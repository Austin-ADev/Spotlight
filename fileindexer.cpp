#include "fileindexer.h"
#include <QThread>
#include <QDirIterator>
#include <QStandardPaths>
#include <QMutexLocker>
#include <QSettings>
#include <QRegularExpression>

FileIndexer::FileIndexer(QObject *parent)
    : QObject(parent)
{
}

void FileIndexer::parseDesktopFile(const QString &filePath, QList<IndexedApp> &appList) {
    QSettings desktopFile(filePath, QSettings::IniFormat);
    desktopFile.beginGroup("Desktop Entry");

    if (desktopFile.value("NoDisplay", false).toBool() ||
        desktopFile.value("Hidden", false).toBool() ||
        desktopFile.value("Type").toString() != "Application") {
        return;
    }

    QString name = desktopFile.value("Name").toString();
    QString exec = desktopFile.value("Exec").toString();

    if (name.isEmpty() || exec.isEmpty()) return;

    static const QRegularExpression fieldCodes("%[fFdDnNuUiImMsS]");
    exec.remove(fieldCodes);
    exec = exec.trimmed();

    IndexedApp app;
    app.name = name;
    app.exec = exec;
    app.icon = desktopFile.value("Icon").toString();

    appList.append(app);
}

void FileIndexer::rebuildIndex() {
    emit indexingStarted();

    QThread *thread = QThread::create([this]() {
        QStringList newFileIndex;
        QList<IndexedApp> newAppIndex;

        QStringList appPaths = {
            "/usr/share/applications",
            "/var/lib/flatpak/exports/share/applications",
            QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.local/share/flatpak/exports/share/applications",
            QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.local/share/applications"
        };

        for (const QString &dirPath : appPaths) {
            QDirIterator it(dirPath, QStringList() << "*.desktop", QDir::Files, QDirIterator::Subdirectories);
            while (it.hasNext()) {
                it.next();
                parseDesktopFile(it.filePath(), newAppIndex);
            }
        }

        QStringList pathsToScan = {
            QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
        };

        int scannedCount = 0;
        int maxEstimate = 5000;

        for (const QString &dirPath : pathsToScan) {
            QDirIterator it(dirPath, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
            while (it.hasNext() && scannedCount < maxEstimate) {
                it.next();
                newFileIndex.append(it.filePath());
                scannedCount++;

                if (scannedCount % 100 == 0) {
                    emit indexingProgress(scannedCount, maxEstimate);
                }
            }
        }

        {
            QMutexLocker locker(&m_mutex);
            m_fileCache = newFileIndex;
            m_appCache = newAppIndex;
        }

        int totalIndexed = newFileIndex.size() + newAppIndex.size();
        emit indexingProgress(maxEstimate, maxEstimate);
        emit indexingFinished(totalIndexed);
    });

    thread->start();
}

QStringList FileIndexer::getIndexedFiles() const {
    QMutexLocker locker(&m_mutex);
    return m_fileCache;
}

QList<IndexedApp> FileIndexer::getIndexedApps() const {
    QMutexLocker locker(&m_mutex);
    return m_appCache;
}