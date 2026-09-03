/**
 * @file 2048UI.cpp
 * @brief 2048 UI Implementation.
 * @date 2026-01-25
 * @version 1.0.0
 *
 * This file contains the implementation of the _2048UI class, which
 * manages the state and coordination of the 2048 UI.
 */


#include "View/2048UI.hpp"


/**
 * @brief Constructs a _2048UI object.
 *
 * @param parent The parent QWidget.
 */
_2048UI::_2048UI(QWidget *parent) : QWidget(parent) {
    // Get Theme //
    Theme::Theme theme = Theme::theme;
    QColor backgroundColor = theme.background.background;
    QColor textColor = theme.text.text;


    // Construct Components //
    QWidget* contentArea = new QWidget(this);
    QLabel* title = new QLabel(contentArea);
    scoreboard = new Scoreboard(contentArea);
    QLabel* headerDirections = new QLabel(contentArea);
    newGame = new NewGame(contentArea);
    gameBoard = new GameBoard(contentArea);
    QLabel* footerDirections = new QLabel(contentArea);


    // Configure Components //
    contentArea->setContentsMargins(0, 0, 0, 0);
    contentArea->setFixedWidth(500);

    title->setText("2048");
    title->setContentsMargins(0, 0, 0, 0);
    title->setAlignment(Qt::AlignCenter);
    title->setFont(QFont("Clear Sans", 50, QFont::Bold));
    QString titleStyle = QString("color: rgb(%1, %2, %3);").arg(textColor.red()).arg(textColor.green()).arg(textColor.blue());
    title->setStyleSheet(titleStyle);

    headerDirections->setText("Join numbers to get to the <b>2048 tile!</b>");
    headerDirections->setContentsMargins(0, 0, 0, 0);
    headerDirections->setAlignment(Qt::AlignCenter);
    headerDirections->setFont(QFont("Clear Sans", 13, QFont::Normal));
    QString headerDirectionsStyle = QString("color: rgb(%1, %2, %3);").arg(textColor.red()).arg(textColor.green()).arg(textColor.blue());
    headerDirections->setStyleSheet(headerDirectionsStyle);

    footerDirections->setText("<b>HOW TO PLAY 2048:</b> Use <b>arrow keys</b> to move the tiles. When two tiles having the same number touch, they <b>join into one!</b>");
    footerDirections->setContentsMargins(0, 15, 0, 0);
    footerDirections->setWordWrap(true);
    footerDirections->setAlignment(Qt::AlignCenter);
    footerDirections->setFont(QFont("Clear Sans", 13, QFont::Normal));
    QString footerDirectionsStyle = QString("color: rgb(%1, %2, %3);").arg(textColor.red()).arg(textColor.green()).arg(textColor.blue());
    footerDirections->setStyleSheet(footerDirectionsStyle);


    // Construct Layouts //
    QHBoxLayout* topHeaderLayout = new QHBoxLayout();
    QHBoxLayout* bottomHeaderLayout = new QHBoxLayout();
    QVBoxLayout* masterHeaderLayout = new QVBoxLayout();
    QVBoxLayout* contentAreaLayout = new QVBoxLayout();
    QVBoxLayout* mainLayout = new QVBoxLayout();


    // Configure Layouts //
    topHeaderLayout->setContentsMargins(0, 0, 0, 0);
    topHeaderLayout->setAlignment(Qt::AlignVCenter);
    topHeaderLayout->addWidget(title);
    topHeaderLayout->addStretch();
    topHeaderLayout->addWidget(scoreboard);

    bottomHeaderLayout->setContentsMargins(0, 0, 0, 0);
    bottomHeaderLayout->setAlignment(Qt::AlignVCenter);
    bottomHeaderLayout->addWidget(headerDirections);
    bottomHeaderLayout->addStretch();
    bottomHeaderLayout->addWidget(newGame);

    masterHeaderLayout->setContentsMargins(0, 0, 0, 10);
    masterHeaderLayout->setAlignment(Qt::AlignCenter);
    masterHeaderLayout->setSpacing(5);
    masterHeaderLayout->addLayout(topHeaderLayout);
    masterHeaderLayout->addLayout(bottomHeaderLayout);

    contentAreaLayout->setContentsMargins(0, 0, 0, 0);
    contentAreaLayout->setAlignment(Qt::AlignCenter);
    contentAreaLayout->addStretch();
    contentAreaLayout->addLayout(masterHeaderLayout);
    contentAreaLayout->addStretch();
    contentAreaLayout->addWidget(gameBoard);
    contentAreaLayout->addSpacing(35);
    contentAreaLayout->addStretch();
    contentAreaLayout->addWidget(footerDirections);
    contentAreaLayout->addStretch();
    contentArea->setLayout(contentAreaLayout);

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(contentArea);


    // Connect and UI Interaction Handle Signals //
    connect(newGame, &NewGame::newGameRequested, this, &_2048UI::newGameRequested);
    connect(gameBoard, &GameBoard::newGameRequested, this, &_2048UI::newGameRequested);


    // Set the UI //
    QString style = QString("background-color: rgb(%1, %2, %3);")
        .arg(backgroundColor.red()).arg(backgroundColor.green()).arg(backgroundColor.blue());
    setContentsMargins(50, 30, 50, 35);
    setStyleSheet(style);
    setLayout(mainLayout);
}


/**
 * @brief Reset the UI for a brand new game.
 */
void _2048UI::reset() {
    scoreboard->setCurrentScore(0);
    gameBoard->reset();
}


/**
 * @brief Commit a combine action.
 *
 * @param combineResult The result of a combine action to implement.
 */
void _2048UI::updateBoard(_2048Engine::CombineResult combineResult) {
    gameBoard->updateBoard(combineResult);
}


/**
 * @brief Set the new current score display score.
 *
 * @param currentScore The new current score to display.
 */
void _2048UI::setCurrentScore(unsigned int currentScore) {
    scoreboard->setCurrentScore(currentScore);
}


/**
 * @brief Add to the current score display score.
 *
 * @param currentScoreAdded The new current score amount to add.
 */
void _2048UI::addToCurrentScore(unsigned int currentScoreAdded) {
    scoreboard->addToCurrentScore(currentScoreAdded);
}


/**
 * @brief Set the new best score display score.
 *
 * @param bestScore The new best score to display.
 */
void _2048UI::setBestScore(unsigned int bestScore) {
    scoreboard->setBestScore(bestScore);
}
