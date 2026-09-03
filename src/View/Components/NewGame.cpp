/**
 * @file NewGame.cpp
 * @brief New Game Button Component Implementation.
 * @date 2026-01-25
 * @version 1.0.0
 *
 * This file contains the definition of the NewGame class, which
 * represents the new game button in the 2048 UI.
 */


#include "View/Components/NewGame.hpp"


/**
 * @brief Constructs a NewGame object.
 *
 * @param parent The parent QWidget.
 */
NewGame::NewGame(QWidget *parent) : QWidget(parent) {
    // Get Theme Colors //
    Theme::Theme theme = Theme::theme;
    QColor backgroundColor = theme.button.background;
    QColor textColor = theme.button.text;

    // New Game Button //
    QPushButton* newGameButton = new QPushButton("New Game", this);
    newGameButton->setContentsMargins(0, 0, 0, 0);
    newGameButton->setFixedSize(125, 40);
    newGameButton->setFont(QFont("Clear Sans", 13, QFont::Medium));
    QString newGameButtonStyle = QString("border: none; border-radius: 4px; padding-top: -2.5px; color: rgb(%1, %2, %3); background-color: rgb(%4, %5, %6);")
        .arg(textColor.red()).arg(textColor.green()).arg(textColor.blue())
        .arg(backgroundColor.red()).arg(backgroundColor.green()).arg(backgroundColor.blue());
    newGameButton->setStyleSheet(newGameButtonStyle);
    newGameButton->setCursor(Qt::PointingHandCursor);
    connect(newGameButton, &QPushButton::clicked, this, &NewGame::newGameRequested);

    // Main Layout //
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(newGameButton);

    // Widget //
    setContentsMargins(0, 0, 0, 0);
    setFixedSize(125, 40);
    setLayout(layout);
}
