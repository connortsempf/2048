/**
 * @file 2048UI.hpp
 * @brief 2048 UI Definition.
 * @date 2026-01-25
 * @version 1.0.0
 *
 * This file contains the definition of the 2048UI class, which
 * manages the state and coordination of the 2048 UI.
 */


#pragma once
#include <string>
#include <QWidget>
#include <QLabel>
#include <QStackedLayout>
#include <QVBoxLayout>
#include "Model/2048Engine.hpp"
#include "View/Components/Scoreboard.hpp"
#include "View/Components/NewGame.hpp"
#include "View/Components/GameBoard.hpp"
#include "View/Themes/Theme.hpp"


/**
 * @brief The UI managing class for the 2048 game.
 */
class _2048UI : public QWidget {

    Q_OBJECT

    public:
        /**
         * @brief Constructs a _2048UI object.
         *
         * @param parent The parent QWidget.
         */
        explicit _2048UI(QWidget *parent = nullptr);

        /**
         * @brief Reset the UI for a brand new game.
         */
        void reset();

        /**
         * @brief Commit a combine action.
         *
         * @param combineResult The result of a combine action to implement.
         */
        void updateBoard(_2048Engine::CombineResult combineResult);

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

    signals:
        /**
         * @brief Signal emitted when a new game is requested.
         */
        void newGameRequested();

    private:
        /**
         * @brief The gameboard UI component.
         */
        GameBoard* gameBoard;

        /**
         * @brief The scoreboard UI component.
         */
        Scoreboard* scoreboard;

        /**
         * @brief The new game UI component.
         */
        NewGame* newGame;
};
