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

#ifndef STYLES_H
#define STYLES_H

#include <QString>
#include <QChar>

class ThemeManager {
public:
    static QString getStyleSheet(const QString &themeName) {
        // Clean non-breaking spaces (\u00A0) and regular whitespace
        QString trimmedTheme = themeName;
        trimmedTheme.replace(QChar(0x00A0), ' ');
        trimmedTheme = trimmedTheme.trimmed();

        if (trimmedTheme.compare("Light", Qt::CaseInsensitive) == 0) {
            return getLightStyle();
        } else if (trimmedTheme.compare("Amber", Qt::CaseInsensitive) == 0) {
            return getAmberStyle();
        } else if (trimmedTheme.compare("Ocean", Qt::CaseInsensitive) == 0) {
            return getOceanStyle();
        } else if (trimmedTheme.compare("Rose", Qt::CaseInsensitive) == 0) {
            return getRoseStyle();
        } else if (trimmedTheme.compare("Midnight", Qt::CaseInsensitive) == 0) {
            return getMidnightStyle();
        } else if (trimmedTheme.compare("Forest", Qt::CaseInsensitive) == 0 ||
                   trimmedTheme.compare("Forrest", Qt::CaseInsensitive) == 0) {
            return getForestStyle();
        }

        return getDarkStyle(); // Default fallback
    }

private:
    static QString getDarkStyle() {
        return R"(
            SpotlightWindow {
                background: transparent;
            }
            #ContainerFrame {
                background-color: #1A1A1E;
                border: 1px solid #2E2E35;
                border-radius: 37px;
            }
            QLineEdit {
                background-color: transparent;
                border: none;
                color: #FFFFFF;
                font-size: 18px;
                padding: 0px 4px;
                selection-background-color: #3B82F6;
            }
            QLineEdit::placeholder {
                color: #6B7280;
            }
            QListView {
                background-color: transparent;
                border: none;
                color: #E5E7EB;
                font-size: 15px;
                outline: none;
            }
            QListView::item {
                padding: 10px 12px;
                border-radius: 8px;
            }
            QListView::item:hover {
                background-color: #27272A;
            }
            QListView::item:selected {
                background-color: #3F3F46;
                color: #FFFFFF;
            }
            QScrollBar:vertical {
                background: transparent;
                width: 8px;
                margin: 0px;
            }
            QScrollBar::handle:vertical {
                background: #3F3F46;
                border-radius: 4px;
                min-height: 20px;
            }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                height: 0px;
            }
        )";
    }

    static QString getLightStyle() {
        return R"(
            SpotlightWindow {
                background: transparent;
            }
            #ContainerFrame {
                background-color: #FFFFFF;
                border: 1px solid #E5E7EB;
                border-radius: 37px;
            }
            QLineEdit {
                background-color: transparent;
                border: none;
                color: #111827;
                font-size: 18px;
                padding: 0px 4px;
                selection-background-color: #3B82F6;
            }
            QLineEdit::placeholder {
                color: #9CA3AF;
            }
            QListView {
                background-color: transparent;
                border: none;
                color: #1F2937;
                font-size: 15px;
                outline: none;
            }
            QListView::item {
                padding: 10px 12px;
                border-radius: 8px;
            }
            QListView::item:hover {
                background-color: #F3F4F6;
            }
            QListView::item:selected {
                background-color: #E5E7EB;
                color: #111827;
            }
            QScrollBar:vertical {
                background: transparent;
                width: 8px;
                margin: 0px;
            }
            QScrollBar::handle:vertical {
                background: #D1D5DB;
                border-radius: 4px;
                min-height: 20px;
            }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                height: 0px;
            }
        )";
    }

    static QString getAmberStyle() {
        return R"(
        SpotlightWindow {
            background: transparent;
        }
        #ContainerFrame {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(52, 36, 16, 0.92),
                stop:0.08 rgba(40, 28, 12, 0.90),
                stop:0.5 rgba(26, 18, 8, 0.88),
                stop:1 rgba(18, 12, 6, 0.92));
            border: 1px solid rgba(245, 158, 11, 0.2);
            border-top: 1px solid rgba(252, 211, 77, 0.35);
            border-radius: 37px;
        }
        QLineEdit {
            background-color: transparent;
            border: none;
            color: #fef3c7;
            font-size: 18px;
            padding: 0px 4px;
            selection-background-color: rgba(245, 158, 11, 0.3);
        }
        QLineEdit::placeholder {
            color: #8a6a4a;
        }
        QListView {
            background-color: transparent;
            border: none;
            color: #fde68a;
            font-size: 15px;
            outline: none;
        }
        QListView::item {
            padding: 10px 12px;
            border-radius: 8px;
        }
        QListView::item:hover {
            background-color: rgba(245, 158, 11, 0.08);
        }
        QListView::item:selected {
            background-color: rgba(245, 158, 11, 0.15);
            color: #fffbeb;
        }
        QScrollBar:vertical {
            background: transparent;
            width: 8px;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background: rgba(245, 158, 11, 0.2);
            border-radius: 4px;
            min-height: 20px;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
    )";
    }

    static QString getOceanStyle() {
        return R"(
            SpotlightWindow {
                background: transparent;
            }
            #ContainerFrame {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 rgba(16, 30, 52, 0.92),
                    stop:0.08 rgba(12, 24, 44, 0.90),
                    stop:0.5 rgba(8, 16, 32, 0.88),
                    stop:1 rgba(6, 12, 24, 0.92));
                border: 1px solid rgba(96, 165, 250, 0.2);
                border-top: 1px solid rgba(147, 197, 253, 0.35);
                border-radius: 37px;
            }
            QLineEdit {
                background-color: transparent;
                border: none;
                color: #d0e4ff;
                font-size: 18px;
                padding: 0px 4px;
                selection-background-color: rgba(96, 165, 250, 0.3);
            }
            QLineEdit::placeholder {
                color: #5a7a9a;
            }
            QListView {
                background-color: transparent;
                border: none;
                color: #b8d4f0;
                font-size: 15px;
                outline: none;
            }
            QListView::item {
                padding: 10px 12px;
                border-radius: 8px;
            }
            QListView::item:hover {
                background-color: rgba(96, 165, 250, 0.08);
            }
            QListView::item:selected {
                background-color: rgba(96, 165, 250, 0.15);
                color: #e0f0ff;
            }
            QScrollBar:vertical {
                background: transparent;
                width: 8px;
                margin: 0px;
            }
            QScrollBar::handle:vertical {
                background: rgba(96, 165, 250, 0.2);
                border-radius: 4px;
                min-height: 20px;
            }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                height: 0px;
            }
        )";
    }

    static QString getRoseStyle() {
        return R"(
        SpotlightWindow {
            background: transparent;
        }
        #ContainerFrame {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(52, 16, 32, 0.92),
                stop:0.08 rgba(40, 12, 24, 0.90),
                stop:0.5 rgba(26, 8, 16, 0.88),
                stop:1 rgba(18, 6, 12, 0.92));
            border: 1px solid rgba(249, 115, 155, 0.2);
            border-top: 1px solid rgba(253, 186, 206, 0.35);
            border-radius: 37px;
        }
        QLineEdit {
            background-color: transparent;
            border: none;
            color: #f5dce8;
            font-size: 18px;
            padding: 0px 4px;
            selection-background-color: rgba(249, 115, 155, 0.3);
        }
        QLineEdit::placeholder {
            color: #8a5a6a;
        }
        QListView {
            background-color: transparent;
            border: none;
            color: #e8b8c8;
            font-size: 15px;
            outline: none;
        }
        QListView::item {
            padding: 10px 12px;
            border-radius: 8px;
        }
        QListView::item:hover {
            background-color: rgba(249, 115, 155, 0.08);
        }
        QListView::item:selected {
            background-color: rgba(249, 115, 155, 0.15);
            color: #ffe8f0;
        }
        QScrollBar:vertical {
            background: transparent;
            width: 8px;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background: rgba(249, 115, 155, 0.2);
            border-radius: 4px;
            min-height: 20px;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
    )";
    }

    static QString getMidnightStyle() {
        return R"(
        SpotlightWindow {
            background: transparent;
        }
        #ContainerFrame {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(24, 18, 48, 0.92),
                stop:0.08 rgba(18, 14, 38, 0.90),
                stop:0.5 rgba(12, 10, 28, 0.88),
                stop:1 rgba(8, 6, 20, 0.92));
            border: 1px solid rgba(139, 92, 246, 0.2);
            border-top: 1px solid rgba(196, 181, 253, 0.35);
            border-radius: 37px;
        }
        QLineEdit {
            background-color: transparent;
            border: none;
            color: #e0d4f5;
            font-size: 18px;
            padding: 0px 4px;
            selection-background-color: rgba(139, 92, 246, 0.3);
        }
        QLineEdit::placeholder {
            color: #6a5a8a;
        }
        QListView {
            background-color: transparent;
            border: none;
            color: #c8b8e0;
            font-size: 15px;
            outline: none;
        }
        QListView::item {
            padding: 10px 12px;
            border-radius: 8px;
        }
        QListView::item:hover {
            background-color: rgba(139, 92, 246, 0.08);
        }
        QListView::item:selected {
            background-color: rgba(139, 92, 246, 0.15);
            color: #f0e8ff;
        }
        QScrollBar:vertical {
            background: transparent;
            width: 8px;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background: rgba(139, 92, 246, 0.2);
            border-radius: 4px;
            min-height: 20px;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
    )";
    }

    static QString getForestStyle() {
        return R"(
        SpotlightWindow {
            background: transparent;
        }
        #ContainerFrame {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(14, 36, 22, 0.92),
                stop:0.08 rgba(10, 28, 16, 0.90),
                stop:0.5 rgba(6, 20, 12, 0.88),
                stop:1 rgba(4, 14, 8, 0.92));
            border: 1px solid rgba(34, 197, 94, 0.2);
            border-top: 1px solid rgba(134, 239, 172, 0.35);
            border-radius: 37px;
        }
        QLineEdit {
            background-color: transparent;
            border: none;
            color: #d4f0dc;
            font-size: 18px;
            padding: 0px 4px;
            selection-background-color: rgba(34, 197, 94, 0.3);
        }
        QLineEdit::placeholder {
            color: #5a8a6a;
        }
        QListView {
            background-color: transparent;
            border: none;
            color: #b8e0c8;
            font-size: 15px;
            outline: none;
        }
        QListView::item {
            padding: 10px 12px;
            border-radius: 8px;
        }
        QListView::item:hover {
            background-color: rgba(34, 197, 94, 0.08);
        }
        QListView::item:selected {
            background-color: rgba(34, 197, 94, 0.15);
            color: #e8fff0;
        }
        QScrollBar:vertical {
            background: transparent;
            width: 8px;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background: rgba(34, 197, 94, 0.2);
            border-radius: 4px;
            min-height: 20px;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
    )";
    }
};

#endif // STYLES_H