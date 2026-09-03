/**
 * @file NewGame.hpp
 * @brief New Game Button Component Definition.
 * @date 2026-01-25
 * @version 1.0.0
 *
 * This file contains the definition of the NewGame class, which
 * represents the new game button in the 2048 UI.
 */


#pragma once
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "View/Themes/Theme.hpp"


/**
 * @brief Represents a New Game button in the 2048 UI.
 */
class NewGame : public QWidget {

    Q_OBJECT

    public:
        /**
         * @brief Constructs a NewGame object.
         *
         * @param parent The parent QWidget.
         */
        explicit NewGame(QWidget *parent = nullptr);

    signals:
        /**
         * @brief Signal emitted when a new game is requested.
         */
        void newGameRequested();
};
