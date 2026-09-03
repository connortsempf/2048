/**
 * @file Scoreboard.hpp
 * @brief Scoreboard Component Definition.
 * @date 2026-01-25
 * @version 1.0.0
 *
 * This file contains the definition of the Scoreboard class, which
 * represents the scoreboard for the 2048 UI.
 */


#pragma once
#include <QPointer>
#include <QTimer>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>
#include "View/Themes/Theme.hpp"


/**
 * @brief Represents a Scoreboard in the 2048 UI.
 */
class Scoreboard : public QWidget {

    Q_OBJECT

    public:
        /**
         * @brief Constructs a Scoreboard object.
         *
         * @param parent The parent QWidget.
         */
        explicit Scoreboard(QWidget *parent = nullptr);

        /**
         * @brief Set the new current score display score.
         *
         * @param currentScore The new current score to display.
         */
        void setCurrentScore(unsigned int currentScore);

        /**
         * @brief Add to the current score display score.
         *
         * @param currentScoreAdded The new current score amount to add.
         */
        void addToCurrentScore(unsigned int currentScoreAdded);

        /**
         * @brief Set the new best score display score.
         *
         * @param bestScore The new best score to display.
         */
        void setBestScore(unsigned int bestScore);

    private:
        /**
         * @brief The width of each score display tab.
         */
        static const unsigned int SCORE_DISPLAY_WIDTH;

        /**
         * @brief The height of each score display tab.
         */
        static const unsigned int SCORE_DISPLAY_HEIGHT;

    private:
        /**
         * @brief Label for the current score text display.
         */
        QLabel* currentScoreScoreText;

        /**
         * @brief Label for the best score text display.
         */
        QLabel* bestScoreScoreText;

        /**
         * @brief Label for the added current score text animation.
         */
        QLabel* addedCurrentScoreAnimationText;

        /**
         * @brief Animation group for the added current score text pop-up.
         */
        QPointer<QParallelAnimationGroup> addedCurrentScoreAnimationGroup;

    private:
        /**
         * @brief Animation when a score display is added to.
         */
        void animateScoreIncrease(unsigned int scoreAdded);
};
