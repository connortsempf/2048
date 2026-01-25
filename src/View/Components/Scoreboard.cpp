/**
 * @file Scoreboard.cpp
 * @brief Scoreboard Component Implementation.
 * @date 2026-01-25
 * @version 1.0.0
 *
 * This file contains the implementation of the Scoreboard class, which
 * represents the scoreboard for the 2048 UI.
 */


#include "View/Components/Scoreboard.hpp"


/**
 * @brief The width of each score display tab.
 */
const unsigned int Scoreboard::SCORE_DISPLAY_WIDTH = 130;

/**
 * @brief The height of each score display tab.
 */
const unsigned int Scoreboard::SCORE_DISPLAY_HEIGHT = 60;


/**
 * @brief Constructs a Scoreboard object.
 *
 * @param parent The parent QWidget.
 */
Scoreboard::Scoreboard(QWidget *parent) : QWidget(parent) {
    // Get Theme Colors //
    Theme::Theme theme = Theme::theme;
    QColor backgroundColor = theme.scoreboard.background;
    QColor headerTextColor = theme.scoreboard.headerText;
    QColor scoreTextolor = theme.scoreboard.scoreText;


    // Current Score Tab //
    QWidget* currentScoreTab = new QWidget(this);
    currentScoreTab->setFixedSize(Scoreboard::SCORE_DISPLAY_WIDTH, Scoreboard::SCORE_DISPLAY_HEIGHT);
    currentScoreTab->setContentsMargins(0, 0, 0, 0);
    QString currentScoreTabStyle = QString("border: none; border-radius: 4px; background-color: rgb(%1, %2, %3);")
        .arg(backgroundColor.red()).arg(backgroundColor.green()).arg(backgroundColor.blue());
    currentScoreTab->setStyleSheet(currentScoreTabStyle);

    QLabel* currentScoreHeaderText = new QLabel("SCORE", this);
    currentScoreHeaderText->setContentsMargins(0, 0, 0, 0);
    currentScoreHeaderText->setAlignment(Qt::AlignCenter);
    currentScoreHeaderText->setFont(QFont("Clear Sans", 11, QFont::Medium));
    QString currentScoreHeaderTextStyle = QString("border: none; color: rgb(%1, %2, %3);")
        .arg(headerTextColor.red()).arg(headerTextColor.green()).arg(headerTextColor.blue());
    currentScoreHeaderText->setStyleSheet(currentScoreHeaderTextStyle);

    currentScoreScoreText = new QLabel("0", this);
    currentScoreScoreText->setContentsMargins(0, 0, 0, 0);
    currentScoreScoreText->setAlignment(Qt::AlignCenter);
    currentScoreScoreText->setFont(QFont("Clear Sans", 16, QFont::Bold));
    QString currentScoreScoreTextStyle = QString("border: none; color: rgb(%1, %2, %3);")
        .arg(scoreTextolor.red()).arg(scoreTextolor.green()).arg(scoreTextolor.blue());
    currentScoreScoreText->setStyleSheet(currentScoreScoreTextStyle);

    QVBoxLayout* currentScoreTabLayout = new QVBoxLayout(this);
    currentScoreTabLayout->setContentsMargins(0, 9, 0, 8);
    currentScoreTabLayout->setAlignment(Qt::AlignCenter);
    currentScoreTabLayout->addWidget(currentScoreHeaderText);
    currentScoreTabLayout->addWidget(currentScoreScoreText);
    currentScoreTab->setLayout(currentScoreTabLayout);


    // Best Score Tab //
    QWidget* bestScoreTab = new QWidget(this);
    bestScoreTab->setFixedSize(Scoreboard::SCORE_DISPLAY_WIDTH, Scoreboard::SCORE_DISPLAY_HEIGHT);
    bestScoreTab->setContentsMargins(0, 0, 0, 0);
    QString bestScoreTabStyle = QString("border: none; border-radius: 4px; background-color: rgb(%1, %2, %3);")
        .arg(backgroundColor.red()).arg(backgroundColor.green()).arg(backgroundColor.blue());
    bestScoreTab->setStyleSheet(bestScoreTabStyle);

    QLabel* bestScoreHeaderText = new QLabel("BEST", this);
    bestScoreHeaderText->setContentsMargins(0, 0, 0, 0);
    bestScoreHeaderText->setAlignment(Qt::AlignCenter);
    bestScoreHeaderText->setFont(QFont("Clear Sans", 11, QFont::Medium));
    QString bestScoreHeaderTextStyle = QString("border: none; color: rgb(%1, %2, %3);")
        .arg(headerTextColor.red()).arg(headerTextColor.green()).arg(headerTextColor.blue());
    bestScoreHeaderText->setStyleSheet(bestScoreHeaderTextStyle);

    bestScoreScoreText = new QLabel("0", this);
    bestScoreScoreText->setContentsMargins(0, 0, 0, 0);
    bestScoreScoreText->setAlignment(Qt::AlignCenter);
    bestScoreScoreText->setFont(QFont("Clear Sans", 16, QFont::Bold));
    QString bestScoreScoreTextStyle = QString("border: none; color: rgb(%1, %2, %3);")
        .arg(scoreTextolor.red()).arg(scoreTextolor.green()).arg(scoreTextolor.blue());
    bestScoreScoreText->setStyleSheet(bestScoreScoreTextStyle);

    QVBoxLayout* bestScoreTabLayout = new QVBoxLayout(this);
    bestScoreTabLayout->setContentsMargins(0, 9, 0, 8);
    bestScoreTabLayout->setAlignment(Qt::AlignCenter);
    bestScoreTabLayout->addWidget(bestScoreHeaderText);
    bestScoreTabLayout->addWidget(bestScoreScoreText);
    bestScoreTab->setLayout(bestScoreTabLayout);


    // Added Score Animation Text //
    addedCurrentScoreAnimationText = new QLabel("", this);
    addedCurrentScoreAnimationText->setContentsMargins(0, 0, 0, 0);
    addedCurrentScoreAnimationText->setAlignment(Qt::AlignCenter);
    addedCurrentScoreAnimationText->setGeometry(currentScoreScoreText->geometry());
    addedCurrentScoreAnimationText->setFont(QFont("Clear Sans", 18, QFont::Bold));
    QColor animationTextColor = theme.text.text;
    QString addedCurrentScoreAnimationTextStyle = QString("border: none; color: rgb(%1, %2, %3); background-color: rgba(0, 0, 0, 0)")
        .arg(animationTextColor.red()).arg(animationTextColor.green()).arg(animationTextColor.blue());
    addedCurrentScoreAnimationText->setStyleSheet(addedCurrentScoreAnimationTextStyle);


    // Main Layout //
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(currentScoreTab);
    layout->addWidget(bestScoreTab);


    // Set the Widget //
    setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
}


/**
 * @brief Set the new current score display score.
 *
 * @param currentScore The new current score to display.
 */
void Scoreboard::setCurrentScore(unsigned int currentScore) {
    currentScoreScoreText->setText(QString::number(currentScore));
    // if (currentScore > bestScoreScoreText->text().toInt()) setBestScore(currentScore);
}

/**
 * @brief Add to the current score display score.
 *
 * @param currentScoreAdded The new current score amount to add.
 */
void Scoreboard::addToCurrentScore(unsigned int currentScoreAdded) {
    currentScoreScoreText->setText(QString::number(currentScoreScoreText->text().toInt() + currentScoreAdded));
    if (currentScoreAdded > 0) animateScoreIncrease(currentScoreAdded);
}


/**
 * @brief Set the new best score display score.
 *
 * @param bestScore The new best score to display.
 */
void Scoreboard::setBestScore(unsigned int bestScore) {
    bestScoreScoreText->setText(QString::number(bestScore));
}


/**
 * @brief Animation when a score display is added to.
 */
void Scoreboard::animateScoreIncrease(unsigned int scoreAdded) {
    addedCurrentScoreAnimationText->setText("+" + QString::number(scoreAdded));
    addedCurrentScoreAnimationText->setGeometry(currentScoreScoreText->geometry());
    addedCurrentScoreAnimationText->raise();
    addedCurrentScoreAnimationText->show();

    // Clean Up the Old Animation if Interrupted //
    if (addedCurrentScoreAnimationGroup) {
        addedCurrentScoreAnimationGroup->stop();
        delete addedCurrentScoreAnimationGroup;
    }

    // Fade Out Animation //
    QGraphicsOpacityEffect* transparencyEffect = new QGraphicsOpacityEffect();
    addedCurrentScoreAnimationText->setGraphicsEffect(transparencyEffect);
    QPropertyAnimation* fadeOutAnimation = new QPropertyAnimation(transparencyEffect, "opacity");
    fadeOutAnimation->setDuration(500);
    fadeOutAnimation->setStartValue(1.0);
    fadeOutAnimation->setEndValue(0.0);
    fadeOutAnimation->setEasingCurve(QEasingCurve::InQuad);

    // Rise Upward Animation //
    QPropertyAnimation* moveUpAnimation = new QPropertyAnimation(addedCurrentScoreAnimationText, "geometry");
    QRect startGeometry = addedCurrentScoreAnimationText->geometry();
    QRect endGeometry = startGeometry;
    endGeometry.translate(0, -30);
    moveUpAnimation->setDuration(800);
    moveUpAnimation->setStartValue(startGeometry);
    moveUpAnimation->setEndValue(endGeometry);
    moveUpAnimation->setEasingCurve(QEasingCurve::OutQuad);

    // Create the Parallel Animation Grouping //
    addedCurrentScoreAnimationGroup = new QParallelAnimationGroup();
    addedCurrentScoreAnimationGroup->addAnimation(fadeOutAnimation);
    addedCurrentScoreAnimationGroup->addAnimation(moveUpAnimation);
    addedCurrentScoreAnimationGroup->start();
}
