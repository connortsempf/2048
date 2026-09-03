/**
 * @file GameConfig.hpp
 * @brief Game Config Constants for 2048.
 * @date 2026-01-25
 * @version 1.0.0
 *
 * This file contains compile-time constants that define the core
 * rules and constraints of the 2048 game.
 */


#pragma once


/**
 * @brief Core game configuration constants.
 */
namespace GameConfig {

    /**
     * @brief Amount of number tiles in each column (the height).
     */
    constexpr unsigned int BOARD_ROWS = 4;

    /**
     * @brief Amount of number tiles in each row (the width).
     */
    constexpr unsigned int BOARD_COLUMNS = 4;
}
