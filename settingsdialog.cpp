#include "settingsdialog.h"
#include "fileindexer.h"
#include "styles.h"
#include <QSettings>
#include <QCoreApplication>
#include <QStyle>
#include <QVBoxLayout>
#include <QHBoxLayout>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent), m_isIndexing(false)
{
    setWindowTitle("Spotlight Settings");
    setFixedSize(380, 320);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Top Navigation / Back Button
    QHBoxLayout *topLayout = new QHBoxLayout();
    m_backBtn = new QPushButton("← Back", this);
    m_backBtn->setFixedWidth(80);
    topLayout->addWidget(m_backBtn);
    topLayout->addStretch();
    mainLayout->addLayout(topLayout);

    connect(m_backBtn, &QPushButton::clicked, this, &SettingsDialog::onBackClicked);

    // Theme Selection Controls
    QHBoxLayout *themeLayout = new QHBoxLayout();
    QLabel *themeLabel = new QLabel("App Theme:", this);
    m_themeComboBox = new QComboBox(this);
    m_themeComboBox->addItems({"Dark", "Light"});

    QSettings configSettings("SpotlightApp", "Config");
    QString savedTheme = configSettings.value("theme", "Dark").toString();
    m_themeComboBox->setCurrentText(savedTheme);

    themeLayout->addWidget(themeLabel);
    themeLayout->addWidget(m_themeComboBox);
    mainLayout->addLayout(themeLayout);

    connect(m_themeComboBox, &QComboBox::currentTextChanged, this, &SettingsDialog::onThemeComboChanged);

    // Search Options
    bool searchCmds = configSettings.value("searchCommands", true).toBool();
    m_searchCommandsCheckBox = new QCheckBox("Include Applications & Commands", this);
    m_searchCommandsCheckBox->setChecked(searchCmds);
    mainLayout->addWidget(m_searchCommandsCheckBox);

    connect(m_searchCommandsCheckBox, &QCheckBox::toggled, this, &SettingsDialog::onSearchCommandsToggled);

    // Re-index Button
    m_reindexBtn = new QPushButton("Re-index Files & Applications", this);
    mainLayout->addWidget(m_reindexBtn);
    connect(m_reindexBtn, &QPushButton::clicked, this, &SettingsDialog::onReindexClicked);

    // Progress Bar & Status
    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 100);
    m_progressBar->setVisible(false);
    mainLayout->addWidget(m_progressBar);

    m_statusLabel = new QLabel("", this);
    mainLayout->addWidget(m_statusLabel);

    // Setup Tray
    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(style()->standardIcon(QStyle::SP_ComputerIcon));
    m_trayIcon->show();

    setLayout(mainLayout);
}

void SettingsDialog::onThemeComboChanged(const QString &themeName) {
    QSettings configSettings("SpotlightApp", "Config");
    configSettings.setValue("theme", themeName);
    emit themeChanged(themeName);
}

void SettingsDialog::onBackClicked() {
    this->close();
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

    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        m_trayIcon->showMessage(
            "Spotlight Indexer",
            QString("Indexing completed successfully. %1 items cached.").arg(totalFiles),
            QSystemTrayIcon::Information,
            5000
            );
    }

    if (isHidden()) {
        QCoreApplication::quit();
    }
}