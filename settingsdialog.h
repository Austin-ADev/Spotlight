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