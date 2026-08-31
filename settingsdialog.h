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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QProgressBar>
#include <QSystemTrayIcon>

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog() = default;

    void connectIndexerSignals(class FileIndexer *indexer);

signals:
    void triggerReindex();
    void themeChanged(const QString &newTheme);

private slots:
    void onReindexClicked();
    void onSearchCommandsToggled(bool checked);
    void onThemeComboChanged(const QString &themeName);
    void onIndexingStarted();
    void onIndexingProgress(int current, int total);
    void onIndexingFinished(int totalFiles);
    void onBackClicked();

private:
    QPushButton *m_backBtn;
    QPushButton *m_reindexBtn;
    QCheckBox *m_searchCommandsCheckBox;
    QComboBox *m_themeComboBox;
    QProgressBar *m_progressBar;
    QLabel *m_statusLabel;
    QSystemTrayIcon *m_trayIcon;
    bool m_isIndexing;
};

#endif // SETTINGSDIALOG_H