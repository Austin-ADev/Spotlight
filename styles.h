#ifndef STYLES_H
#define STYLES_H

#include <QString>

class ThemeManager {
public:
    static QString getStyleSheet(const QString &themeName) {
        if (themeName.toLower() == "light") {
            return R"(
                SpotlightWindow {
                    background: transparent;
                }
                #ContainerFrame {
                    background-color: #FFFFFF;
                    border: 1px solid #E5E7EB;
                    border-radius: 24px;
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

        // Default: Dark Theme
        return R"(
            SpotlightWindow {
                background: transparent;
            }
            #ContainerFrame {
                background-color: #1A1A1E;
                border: 1px solid #2E2E35;
                border-radius: 24px;
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
        if (themeName.toLower() == "amber") {
            return R"(
        SpotlightWindow {
            background: transparent;
        }
        #ContainerFrame {
            background-color: #1a1408;
            border: 1px solid rgba(255, 191, 0, 0.15);
            border-radius: 24px;
        }
        QLineEdit {
            background-color: transparent;
            border: none;
            color: #f5e6c8;
            font-size: 18px;
            padding: 0px 4px;
            selection-background-color: rgba(255, 191, 0, 0.3);
        }
        QLineEdit::placeholder {
            color: #8a7a50;
        }
        QListView {
            background-color: transparent;
            border: none;
            color: #e8d5a8;
            font-size: 15px;
            outline: none;
        }
        QListView::item {
            padding: 10px 12px;
            border-radius: 8px;
        }
        QListView::item:hover {
            background-color: rgba(255, 191, 0, 0.08);
        }
        QListView::item:selected {
            background-color: rgba(255, 191, 0, 0.15);
            color: #fff3d0;
        }
        QScrollBar:vertical {
            background: transparent;
            width: 8px;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background: rgba(255, 191, 0, 0.2);
            border-radius: 4px;
            min-height: 20px;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
    )";
        }
    }
};

#endif // STYLES_H