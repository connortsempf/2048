/**
 * @file 2048Engine.hpp
 * @brief 2048 Engine Definition.
 * @date 2024-06
 * @version 1.0.0
 *
 * This file contains the definition of the _2048Engine class, which
 * provides the core functionality for the 2048 game.
 */


#pragma once
#include <array>
#include <vector>
#include <random>
#include <cstdlib>
#include <algorithm>
#include "Model/GameConfig.hpp"


/**
 * @brief Provides the core functionality for the 2048 game.
 */
class _2048Engine {

    public:
        /**
         * @brief Direction of a user swipe for playing the game.
         */
        enum class CombineDirection { NONE, LEFT, RIGHT, UP, DOWN };

        /**
         * @brief The action associated with a tile after a combine attempt.
         */
        enum class TileAction { HELD, MOVED, ABSORBED, PROMOTED, SPAWNED };

        /**
         * @brief A tile and its associated metadata for a given combine.
         */
        struct Tile {
            bool filled = false;
            TileAction action = TileAction::HELD;
            unsigned int startValue = 0;
            unsigned int startRowIndex = 0;
            unsigned int startColumnIndex = 0;
            unsigned int endValue = 0;
            unsigned int endRowIndex = 0;
            unsigned int endColumnIndex = 0;
        };

        /**
         * @brief Output of information describing the reuslts of a combine attempt.
         */
        struct CombineResult {
            /**
             * @brief The direction of the combine attempt.
             */
            CombineDirection combineDirection;

            /**
             * @brief Flag indicating if the game is over (True if over, false if not).
             */
            bool gameOver = false;

            /**
             * @brief Flag indicating if any tiles were moved in the combine attempted.
             */
            bool tilesMoved = false;

            /**
             * @brief The score associated with the combine attempt.
             */
            unsigned int combineScore = 0;

            /**
             * @brief The total score associated with the current game.
             */
            unsigned int totalScore = 0;

            /**
             * @brief The tiles for the given combine action.
             */
            const std::vector<Tile>& tiles;
        };

    public:
        /**
         * @brief The default constructor for the 204 Engine class to create an instance of the engine.
         */
        _2048Engine() = default;

        /**
         * @brief Start a new 2048 game and create a new starting board of tiles.
         *
         * @return Results of starting a new game (new tiles are spawned).
         */
        CombineResult startNewGame();

        /**
         * @brief Handle a combine action in any direction from the player.
         *
         * @param combineDirection The direction in which to shift and combine the tiles.
         *
         * @return Results of the combine action as a CombineResult.
         */
        CombineResult attemptCombine(CombineDirection combineDirection);

    private:
        /**
         * @brief The total score of the current game for the player.
         */
        unsigned int totalScore = 0;

        /**
         * @brief The game board of tiles.
         */
        std::vector<Tile> tiles;

    private:
        /**
         * @brief Spawn the random tiles for the start of a new game.
         */
        void spawnStartTiles();

        /**
         * @brief Spawn the random tile after a combine has occurred.
         */
        void spawnPostCombineTile();

        /**
         * @brief Detect if the game is over (no mergeable tiles).
         *
         * @return True if the game is over, false if otherwise.
         */
        bool detectGameOver();

        /**
         * @brief Update the tiles in preparation for a new combine attempt.
         */
        void updateTilesForNewCombine();
};
