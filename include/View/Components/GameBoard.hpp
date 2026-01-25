/**
 * @file GameBoard.hpp
 * @brief Game Board Component Definition.
 * @date 2026-01-25
 * @version 1.0.0
 *
 * This file contains the definition of the GameBoard class,
 * which represents the tile board in the 2048 UI.
 */


#pragma once
#include <array>
#include <cmath>
#include <tuple>
#include <QString>
#include <QPointer>
#include <QTimer>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QPauseAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>
#include "Model/GameConfig.hpp"
#include "Model/2048Engine.hpp"
#include "View/Themes/Theme.hpp"


/**
 * @brief Represents the playing board of tiles in the 2048 UI.
 */
class GameBoard : public QWidget {

    Q_OBJECT

    public:
        /**
         * @brief Constructs a GameBoard object.
         *
         * @param parent The parent QWidget.
         */
        explicit GameBoard(QWidget *parent = nullptr);

        /**
         * @brief Reset the logical and visual state forof the board.
         */
        void reset();

        /**
         * @brief Updates the board with new tile positions and performs animations.
         *
          * @param combineResult The result of a combine action to implement.
         */
        void updateBoard(_2048Engine::CombineResult combineResult);

    signals:
        /**
         * @brief Sigmal emitted when a new game is requested.
         */
        void newGameRequested();

        /**
         * @brief Signal emitted when the tile movement animations are complete.
         */
        void moveAnimationsFinished();

        /**
         * @brief Signal emitted when the tile spawn animations are complete.
         */
        void spawnAnimationsFinished();

    protected:
        /**
         * @brief Move event override.
         *
         * @param event The move event.
         */
        void moveEvent(QMoveEvent *event) override;

    private:
        /**
         * @brief The spacing between UI tiles to eachother and the edge of the board.
         */
        static const unsigned int TILE_MARGINS;

        /**
         * @brief The height and width of the board in pixels.
         */
        static const unsigned int BOARD_SIZE;

        /**
         * @brief The tile element container storing the UI and the data.
         */
        struct Tile {
            /**
             * @brief The UI label portion of the tile.
             */
            QLabel* label;

            /**
             * @brief The logical data portion of the tile.
             */
            _2048Engine::Tile data;
        };

    private:
        /**
         * @brief The tile board.
         */
        std::vector<Tile> tiles;

        /**
         * @brief The tile backgrounds.
         */
        std::array<std::array<QWidget*, GameConfig::BOARD_ROWS>, GameConfig::BOARD_COLUMNS> tileBackgrounds;

        /**
         * @brief Tile backgrounds layout.
         */
        QGridLayout* tileBackgroundsLayout;

        QWidget* gameOverOverlay;
        QLabel* gameOverHeader;
        QPushButton* gameOverTryAgainButton;

        QPointer<QParallelAnimationGroup> moveAnimationGroup;
        QPointer<QParallelAnimationGroup> spawnAnimationGroup;

    private:
        /**
         * @brief Animate the tile movements in the game board.
         */
        void animateTileMoves();

        /**
         * @brief Animate the tile spawns in the game board.
         */
        void animateTileSpawns();

        /**
         * @brief Animate the entry of the game over state UI elements.
         */
        void animateGameOverElementsEntry();
};
