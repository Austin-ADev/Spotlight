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