#ifndef STYLES_H
#define STYLES_H

#include <QString>

class ThemeManager {
public:
    static QString getStyleSheet(const QString &themeName) {
        QString trimmedTheme = themeName.trimmed();

        if (trimmedTheme.compare("Light", Qt::CaseInsensitive) == 0) {
            return getLightStyle();
        } else if (trimmedTheme.compare("Amber", Qt::CaseInsensitive) == 0) {
            return getAmberStyle();
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
    }

    static QString getLightStyle() {
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

    static QString getAmberStyle() {
        return R"(
        SpotlightWindow {
            background: transparent;
        }
        #ContainerFrame {
            /* Glass gradient: Top-to-bottom light sheen down to translucent amber depth */
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0.0 rgba(255, 220, 130, 0.35),
                stop:0.04 rgba(255, 191, 0, 0.22),
                stop:0.25 rgba(45, 34, 12, 0.65),
                stop:0.85 rgba(26, 20, 8, 0.75),
                stop:1.0 rgba(18, 14, 6, 0.85));

            /* Glass border: Bright top highlight mimicking light hitting the top bevel */
            border: 1px solid rgba(255, 191, 0, 0.25);
            border-top: 1px solid rgba(255, 235, 170, 0.60);
            border-left: 1px solid rgba(255, 220, 130, 0.35);
            border-radius: 37px;
        }
        QLineEdit {
            background-color: transparent;
            border: none;
            color: #fff3d0;
            font-size: 18px;
            padding: 4px 8px;
            selection-background-color: rgba(255, 191, 0, 0.4);
        }
        QLineEdit::placeholder {
            color: rgba(232, 213, 168, 0.5);
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
            border-radius: 12px;
            margin: 2px 4px;
        }
        QListView::item:hover {
            /* Translucent amber gloss on hover */
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(255, 215, 100, 0.25),
                stop:1 rgba(255, 191, 0, 0.08));
            border: 1px solid rgba(255, 215, 100, 0.3);
        }
        QListView::item:selected {
            /* Vibrant liquid amber selection fill */
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(255, 215, 100, 0.40),
                stop:1 rgba(217, 147, 0, 0.25));
            border: 1px solid rgba(255, 225, 130, 0.5);
            color: #ffffff;
        }
        QScrollBar:vertical {
            background: transparent;
            width: 8px;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            /* Glassy scrollbar thumb */
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(255, 210, 100, 0.4),
                stop:1 rgba(255, 191, 0, 0.15));
            border: 1px solid rgba(255, 215, 100, 0.25);
            border-radius: 4px;
            min-height: 20px;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
        QScrollBar:horizontal {
            background: transparent;
            width: 8px;
            margin: 0px;
        }
        QScrollBar::handle:horizontal {
            /* Glassy scrollbar thumb */
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(255, 210, 100, 0.4),
                stop:1 rgba(255, 191, 0, 0.15));
            border: 1px solid rgba(255, 215, 100, 0.25);
            border-radius: 4px;
            min-height: 20px;
        }
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
            height: 0px;
        }
    )";
    }
};

#endif // STYLES_H