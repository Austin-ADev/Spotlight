#define exprtk_disable_string_capabilities
#define exprtk_disable_vector_io
#define exprtk_disable_break_continue
#define exprtk_disable_loop_statements
#define exprtk_disable_conditional_statements

#include "exprtk.hpp"
#include "searchengine.h"
#include "settingsdialog.h"
#include "styles.h"
#include <QApplication>
#include <QProcess>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QUrl>
#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <string>

SearchEngine::SearchEngine(QObject *parent)
    : QObject(parent)
{
    m_indexer.rebuildIndex();
}

FileIndexer* SearchEngine::getIndexer() {
    return &m_indexer;
}

void SearchEngine::refreshIndex() {
    m_indexer.rebuildIndex();
}

static bool evaluateExpression(const QString &expr, double &outResult) {
    std::string expression_string = expr.toStdString();

    typedef exprtk::symbol_table<double> symbol_table_t;
    typedef exprtk::expression<double>   expression_t;
    typedef exprtk::parser<double>       parser_t;

    symbol_table_t symbol_table;
    symbol_table.add_constants(); // Adds pi, e, etc.

    expression_t expression;
    expression.register_symbol_table(symbol_table);

    parser_t parser;
    if (parser.compile(expression_string, expression)) {
        outResult = expression.value();
        return true;
    }

    return false;
}

QStringList SearchEngine::getResults(const QString &input) {
    QString trimmed = input.trimmed();
    if (trimmed.isEmpty()) return QStringList();

    if (trimmed.startsWith("=")) {
        return handleMath(trimmed.mid(1).trimmed());
    }
    if (trimmed.startsWith(">")) {
        return handleCommands(trimmed.mid(1).trimmed());
    }
    if (trimmed.startsWith("godot:", Qt::CaseInsensitive)) {
        return handleGodot(trimmed.mid(6).trimmed());
    }

    return handleFileSearch(trimmed);
}

QStringList SearchEngine::handleMath(const QString &query) {
    QStringList results;
    if (query.isEmpty()) {
        results << "Math Mode: Type an expression (e.g. = 1+3*5, = sin(0.5)*pi)";
        return results;
    }

    double resultValue = 0.0;
    if (evaluateExpression(query, resultValue)) {
        results << "= " + query + "  ⟶  " + QString::number(resultValue, 'g', 10);
    } else {
        results << "= " + query + " ...";
    }

    return results;
}

struct CommandItem {
    QString trigger;
    QString displayText;
};

QStringList SearchEngine::handleCommands(const QString &query) {
    QStringList results;

    const QList<CommandItem> coreCommands = {
        { "spotlight settings", "Action: Open Spotlight Settings" },
        { "settings",           "Action: Open Spotlight Settings" },
        { "terminal",           "Action: Launch Terminal" },
        { "wifi on",            "Action: Turn Wi-Fi On" },
        { "wifi off",           "Action: Turn Wi-Fi Off" }
    };

    if (query.isEmpty()) {
        results << "Command Mode: Type to search installed apps or commands";
        return results;
    }

    for (const auto &cmd : coreCommands) {
        if (cmd.trigger.contains(query, Qt::CaseInsensitive)) {
            results << cmd.displayText;
        }
    }

    QSettings configSettings("SpotlightApp", "Config");
    bool searchCmds = configSettings.value("searchCommands", true).toBool();

    if (searchCmds) {
        const QList<IndexedApp> apps = m_indexer.getIndexedApps();
        for (const IndexedApp &app : apps) {
            if (app.name.contains(query, Qt::CaseInsensitive)) {
                results << "App: " + app.name + " (" + app.exec + ")";
            }
        }
    }

    QSettings settings("SpotlightApp", "Commands");
    settings.beginGroup("CustomCommands");
    QStringList keys = settings.childKeys();

    for (const QString &key : keys) {
        if (key.contains(query, Qt::CaseInsensitive)) {
            QString targetCmd = settings.value(key).toString();
            results << "Custom Action [" + key + "]: " + targetCmd;
        }
    }
    settings.endGroup();

    results << "Run command: " + query;

    return results;
}

QStringList SearchEngine::handleGodot(const QString &query) {
    QStringList results;

    if (query.isEmpty()) {
        results << "Godot: Type project name to search projects"
                << "Godot: Open Main Projects Directory";
        return results;
    }

    int count = 0;
    const QStringList files = m_indexer.getIndexedFiles();
    for (const QString &filePath : files) {
        if (filePath.endsWith("project.godot", Qt::CaseInsensitive)) {
            QFileInfo fileInfo(filePath);
            QString projectDirName = fileInfo.dir().dirName();

            if (projectDirName.contains(query, Qt::CaseInsensitive) || query == "*") {
                results << "Godot Project: " + projectDirName + " (" + fileInfo.absolutePath() + ")";
                count++;
                if (count >= 6) break;
            }
        }
    }

    if (results.isEmpty()) {
        results << "Godot: Launch Engine";
    }

    return results;
}

QStringList SearchEngine::handleFileSearch(const QString &query) {
    QStringList results;
    int count = 0;

    const QStringList files = m_indexer.getIndexedFiles();
    for (const QString &filePath : files) {
        if (filePath.contains(query, Qt::CaseInsensitive)) {
            results << "File: " + filePath;
            count++;
            if (count >= 8) break;
        }
    }

    return results;
}

void SearchEngine::executeAction(const QString &selectedResult) {
    if (selectedResult == "Action: Open Spotlight Settings") {
        openSettingsWindow();
    }
    else if (selectedResult == "Action: Launch Terminal") {
        launchTerminal();
    }
    else if (selectedResult == "Action: Turn Wi-Fi Off") {
        toggleWifi(false);
    }
    else if (selectedResult == "Action: Turn Wi-Fi On") {
        toggleWifi(true);
    }
    else if (selectedResult.startsWith("App: ")) {
        int startExec = selectedResult.indexOf("(");
        int endExec = selectedResult.lastIndexOf(")");
        if (startExec != -1 && endExec != -1) {
            QString execCmd = selectedResult.mid(startExec + 1, endExec - startExec - 1);
            QProcess::startDetached("sh", QStringList() << "-c" << execCmd);
        }
    }
    else if (selectedResult.startsWith("Custom Action [")) {
        int splitIdx = selectedResult.indexOf("]: ");
        if (splitIdx != -1) {
            QString cmd = selectedResult.mid(splitIdx + 3);
            QProcess::startDetached("sh", QStringList() << "-c" << cmd);
        }
    }
    else if (selectedResult.startsWith("Godot Project: ")) {
        int startPath = selectedResult.indexOf("(");
        int endPath = selectedResult.lastIndexOf(")");
        if (startPath != -1 && endPath != -1) {
            QString projectPath = selectedResult.mid(startPath + 1, endPath - startPath - 1);

            if (!QProcess::startDetached("godot", QStringList() << "--path" << projectPath)) {
                QStringList flatpakArgs;
                flatpakArgs << "run" << "--filesystem=host" << "org.godotengine.Godot" << "--path" << projectPath;

                if (!QProcess::startDetached("flatpak", flatpakArgs)) {
                    openFile(projectPath);
                }
            }
        }
    }
    else if (selectedResult == "Godot: Launch Engine") {
        if (!QProcess::startDetached("godot", QStringList())) {
            QProcess::startDetached("flatpak", QStringList() << "run" << "org.godotengine.Godot");
        }
    }
    else if (selectedResult == "Godot: Open Main Projects Directory") {
        openFile(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    }
    else if (selectedResult.startsWith("Run command: ")) {
        QString cmd = selectedResult.mid(13);
        QProcess::startDetached("sh", QStringList() << "-c" << cmd);
    }
    else if (selectedResult.startsWith("File: ")) {
        openFile(selectedResult.mid(6));
    }
}

void SearchEngine::openSettingsWindow() {
    SettingsDialog *dialog = new SettingsDialog();
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->connectIndexerSignals(&m_indexer);

    connect(dialog, &SettingsDialog::triggerReindex, this, &SearchEngine::refreshIndex);

    connect(dialog, &SettingsDialog::themeChanged, [](const QString &themeName) {
        QString qss = ThemeManager::getStyleSheet(themeName);
        qDebug() << "Theme name received:" << themeName;
        qDebug() << "Loaded stylesheet length:" << qss.length();
        qDebug() << "Loaded stylesheet content:" << qss;

        qApp->setStyleSheet(qss);
    });

    dialog->show();
}

void SearchEngine::launchTerminal() {
    QProcess::startDetached("x-terminal-emulator", QStringList());
}

void SearchEngine::toggleWifi(bool enable) {
    QProcess::startDetached("nmcli", QStringList() << "radio" << "wifi" << (enable ? "on" : "off"));
}

void SearchEngine::openFile(const QString &filePath) {
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}