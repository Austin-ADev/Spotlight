#include "settingsdialog.h"
#include "fileindexer.h"
#include <QSettings>
#include <QCoreApplication>
#include <QStyle>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent), m_isIndexing(false)
{
    setWindowTitle("Spotlight Settings");
    setFixedSize(380, 240);

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Settings options
    QSettings settings("SpotlightApp", "Config");
    bool searchCmds = settings.value("searchCommands", true).toBool();

    m_searchCommandsCheckBox = new QCheckBox("Include Applications & Commands in Search", this);
    m_searchCommandsCheckBox->setChecked(searchCmds);
    layout->addWidget(m_searchCommandsCheckBox);

    connect(m_searchCommandsCheckBox, &QCheckBox::toggled, this, &SettingsDialog::onSearchCommandsToggled);

    // Re-index Button
    m_reindexBtn = new QPushButton("Re-index Files & Applications", this);
    layout->addWidget(m_reindexBtn);
    connect(m_reindexBtn, &QPushButton::clicked, this, &SettingsDialog::onReindexClicked);

    // Progress Bar
    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    m_progressBar->setVisible(false);
    layout->addWidget(m_progressBar);

    // Status Label
    m_statusLabel = new QLabel("", this);
    layout->addWidget(m_statusLabel);

    // Setup Tray Icon for Background Notifications
    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(style()->standardIcon(QStyle::SP_ComputerIcon));
    m_trayIcon->show();

    setLayout(layout);
}

void SettingsDialog::connectIndexerSignals(FileIndexer *indexer) {
    if (!indexer) return;
    connect(indexer, &FileIndexer::indexingStarted, this, &SettingsDialog::onIndexingStarted);
    connect(indexer, &FileIndexer::indexingProgress, this, &SettingsDialog::onIndexingProgress);
    connect(indexer, &FileIndexer::indexingFinished, this, &SettingsDialog::onIndexingFinished);
}

void SettingsDialog::onSearchCommandsToggled(bool checked) {
    QSettings settings("SpotlightApp", "Config");
    settings.setValue("searchCommands", checked);
}

void SettingsDialog::onReindexClicked() {
    emit triggerReindex();
}

void SettingsDialog::onIndexingStarted() {
    m_isIndexing = true;
    m_reindexBtn->setEnabled(false);
    m_progressBar->setVisible(true);
    m_progressBar->setValue(0);
    m_statusLabel->setText("Indexing in progress...");
}

void SettingsDialog::onIndexingProgress(int current, int total) {
    if (total > 0) {
        int percent = (current * 100) / total;
        m_progressBar->setValue(percent);
    }
}

void SettingsDialog::onIndexingFinished(int totalFiles) {
    m_isIndexing = false;
    m_progressBar->setValue(100);
    m_statusLabel->setText(QString("Indexing complete! (%1 items)").arg(totalFiles));
    m_reindexBtn->setEnabled(true);

    // Send Desktop Notification
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        m_trayIcon->showMessage(
            "Spotlight Indexer",
            QString("Indexing completed successfully. %1 items cached.").arg(totalFiles),
            QSystemTrayIcon::Information,
            5000
            );
    }

    // Close application instances if running in background mode
    if (isHidden()) {
        QCoreApplication::quit();
    }
}