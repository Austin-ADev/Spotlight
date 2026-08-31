#include "searchengine.h"
#include "settingsdialog.h"
#include "styles.h"
#include <QApplication>
#include <QProcess>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QUrl>
#include <QStack>
#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <cmath>

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
    QString clean = expr;
    clean.remove(' ');
    if (clean.isEmpty()) return false;

    QVector<QString> tokens;
    QString currentToken;

    for (int i = 0; i < clean.length(); ++i) {
        QChar c = clean[i];
        if (c.isDigit() || c == '.') {
            currentToken += c;
        } else if (QString("+-*/%^()").contains(c)) {
            if (!currentToken.isEmpty()) {
                tokens.append(currentToken);
                currentToken.clear();
            }
            if (c == '-' && (tokens.isEmpty() || QString("+-*/%^(").contains(tokens.last()))) {
                currentToken += c;
            } else {
                tokens.append(QString(c));
            }
        } else {
            return false;
        }
    }
    if (!currentToken.isEmpty()) {
        tokens.append(currentToken);
    }

    QStack<double> values;
    QStack<QChar> ops;

    auto precedence = [](QChar op) {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/' || op == '%') return 2;
        if (op == '^') return 3;
        return 0;
    };

    auto applyOp = [](QChar op, double b, double a) -> double {
        switch (op.toLatin1()) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return (b != 0) ? a / b : 0;
        case '%': return fmod(a, b);
        case '^': return pow(a, b);
        default: return 0;
        }
    };

    for (int i = 0; i < tokens.size(); ++i) {
        QString token = tokens[i];
        bool isNumber = false;
        double val = token.toDouble(&isNumber);

        if (isNumber) {
            values.push(val);
        } else if (token == "(") {
            ops.push('(');
        } else if (token == ")") {
            while (!ops.isEmpty() && ops.top() != '(') {
                if (values.size() < 2) return false;
                double val2 = values.pop();
                double val1 = values.pop();
                QChar op = ops.pop();
                values.push(applyOp(op, val2, val1));
            }
            if (!ops.isEmpty()) ops.pop();
        } else if (token.length() == 1 && QString("+-*/%^").contains(token[0])) {
            QChar op = token[0];
            while (!ops.isEmpty() && precedence(ops.top()) >= precedence(op)) {
                if (values.size() < 2) return false;
                double val2 = values.pop();
                double val1 = values.pop();
                QChar opTop = ops.pop();
                values.push(applyOp(opTop, val2, val1));
            }
            ops.push(op);
        } else {
            return false;
        }
    }

    while (!ops.isEmpty()) {
        if (values.size() < 2) return false;
        double val2 = values.pop();
        double val1 = values.pop();
        QChar op = ops.pop();
        values.push(applyOp(op, val2, val1));
    }

    if (values.size() == 1) {
        outResult = values.pop();
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
        results << "Math Mode: Type an expression (e.g. = 1+3*5)";
        return results;
    }

    double resultValue = 0.0;
    if (evaluateExpression(query, resultValue)) {
        results << "= " + query + "  ⟶  " + QString::number(resultValue);
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
        qApp->setStyleSheet(ThemeManager::getStyleSheet(themeName));
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