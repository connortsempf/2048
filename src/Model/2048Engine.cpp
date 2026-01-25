/**
 * @file 2048Engine.cpp
 * @brief 2048 Engine Implementation.
 * @date 2024-06
 * @version 1.0.0
 *
 * This file contains the implementation of the _2048Engine class, which
 * provides the core functionality for the 2048 game.
 */


#include "Model/2048Engine.hpp"


/**
 * @brief Start a new 2048 game and create a new starting board of tiles.
 *
 * @return Results of starting a new game (new tiles are spawned).
 */
_2048Engine::CombineResult _2048Engine::startNewGame() {
    // Reset Game Values //
    totalScore = 0;
    tiles.clear();
    spawnStartTiles();

    // Return the Combine Result Data //
    _2048Engine::CombineResult combineResult = {
        .combineDirection = _2048Engine::CombineDirection::NONE,
        .gameOver = false,
        .tilesMoved = false,
        .combineScore = 0,
        .totalScore = this->totalScore,
        .tiles = this->tiles
    };
    return combineResult;
}


/**
 * @brief Handle a combine action in any direction from the player.
 *
 * @param combineDirection The direction in which to shift and combine the tiles.
 *
 * @return Results of the combine action as a CombineResult.
 */
_2048Engine::CombineResult _2048Engine::attemptCombine(_2048Engine::CombineDirection combineDirection) {
    // Setup Combine Result Metadata //
    bool moved = false;
    unsigned int combineScore = 0;
    updateTilesForNewCombine();


    // Create an Empty Playing Board and Fill Taken Up Spaces with Tiles //
    std::array<std::array<_2048Engine::Tile, GameConfig::BOARD_ROWS>, GameConfig::BOARD_COLUMNS> tilesBoard;
    for (const auto& tile : tiles) tilesBoard[tile.startRowIndex][tile.startColumnIndex] = tile;


    // Combine to the Left //
    if (combineDirection == _2048Engine::CombineDirection::LEFT) {
        for (int i = 0; i < GameConfig::BOARD_ROWS; i++) {
            std::array<_2048Engine::Tile, GameConfig::BOARD_COLUMNS> tilesBoardRow;

            // Compress Tiles -- Remove Zeroes //
            int fillColumnIndex = 0;
            for (int j = 0; j < tilesBoard[i].size(); j++) {
                if (!tilesBoard[i][j].filled) continue;
                if (fillColumnIndex != j) {
                    moved = true;
                    tilesBoard[i][j].endColumnIndex = fillColumnIndex;
                    tilesBoard[i][j].action = _2048Engine::TileAction::MOVED;
                }
                tilesBoardRow[fillColumnIndex++] = tilesBoard[i][j];
            }

            // Merge Tiles -- Combine Equal Adjacent Values //
            bool merged = false;
            for (int j = 0; j < fillColumnIndex - 1; j++) {
                if (tilesBoardRow[j].filled && tilesBoardRow[j].action != _2048Engine::TileAction::ABSORBED && tilesBoardRow[j].startValue == tilesBoardRow[j + 1].startValue) {
                    // Update The Tiles After Finding Merge Sources //
                    unsigned int promotedStartColumnIndex = tilesBoardRow[j].startColumnIndex;
                    unsigned int absorbedStartColumnIndex = tilesBoardRow[j + 1].startColumnIndex;
                    tilesBoard[i][promotedStartColumnIndex].endValue = tilesBoard[i][promotedStartColumnIndex].startValue * 2;
                    tilesBoard[i][promotedStartColumnIndex].action = _2048Engine::TileAction::PROMOTED;
                    tilesBoard[i][absorbedStartColumnIndex].endValue = 0;
                    tilesBoard[i][absorbedStartColumnIndex].action = _2048Engine::TileAction::ABSORBED;
                    tilesBoard[i][absorbedStartColumnIndex].endColumnIndex = j;
                    tilesBoardRow[j] = tilesBoard[i][promotedStartColumnIndex];
                    tilesBoardRow[j + 1] = tilesBoard[i][absorbedStartColumnIndex];

                    // Update Combine Result Metadata //
                    merged = true;
                    moved = true;
                    combineScore += tilesBoard[i][promotedStartColumnIndex].endValue;
                    totalScore += combineScore;
                }
            }

            // Re-Compress Tiles After Merging //
            if (!merged) continue;
            fillColumnIndex = 0;
            for (int j = 0; j < GameConfig::BOARD_COLUMNS; j++) {
                if (!tilesBoard[i][j].filled || tilesBoard[i][j].action == _2048Engine::TileAction::ABSORBED) continue;
                if (fillColumnIndex != j) {
                    tilesBoard[i][j].endColumnIndex = fillColumnIndex;
                    if (tilesBoard[i][j].action != _2048Engine::TileAction::PROMOTED) {
                        tilesBoard[i][j].action = _2048Engine::TileAction::MOVED;
                    }
                }
                ++fillColumnIndex;
            }
        }
    }


    // Combine to the Right //
    else if (combineDirection == _2048Engine::CombineDirection::RIGHT) {
        for (int i = 0; i < GameConfig::BOARD_ROWS; i++) {
            std::array<_2048Engine::Tile, GameConfig::BOARD_COLUMNS> tilesBoardRow;

            // Compress Tiles -- Remove Zeroes //
            int fillColumnIndex = GameConfig::BOARD_COLUMNS - 1;
            for (int j = GameConfig::BOARD_COLUMNS - 1; j >= 0; j--) {
                if (!tilesBoard[i][j].filled) continue;
                if (fillColumnIndex != j) {
                    moved = true;
                    tilesBoard[i][j].endColumnIndex = fillColumnIndex;
                    tilesBoard[i][j].action = _2048Engine::TileAction::MOVED;
                }
                tilesBoardRow[fillColumnIndex--] = tilesBoard[i][j];
            }

            // Merge Tiles -- Combine Equal Adjacent Values //
            bool merged = false;
            for (int j = GameConfig::BOARD_COLUMNS - 1; j > 0; j--) {
                if (tilesBoardRow[j].filled && tilesBoardRow[j].action != _2048Engine::TileAction::ABSORBED && tilesBoardRow[j].startValue == tilesBoardRow[j - 1].startValue) {
                    // Update The Tiles After Finding Merge Sources //
                    unsigned int promotedStartColumnIndex = tilesBoardRow[j].startColumnIndex;
                    unsigned int absorbedStartColumnIndex = tilesBoardRow[j - 1].startColumnIndex;
                    tilesBoard[i][promotedStartColumnIndex].endValue = tilesBoard[i][promotedStartColumnIndex].startValue * 2;
                    tilesBoard[i][promotedStartColumnIndex].action = _2048Engine::TileAction::PROMOTED;
                    tilesBoard[i][absorbedStartColumnIndex].endValue = 0;
                    tilesBoard[i][absorbedStartColumnIndex].action = _2048Engine::TileAction::ABSORBED;
                    tilesBoard[i][absorbedStartColumnIndex].endColumnIndex = j;
                    tilesBoardRow[j] = tilesBoard[i][promotedStartColumnIndex];
                    tilesBoardRow[j - 1] = tilesBoard[i][absorbedStartColumnIndex];

                    // Update Combine Result Metadata //
                    merged = true;
                    moved = true;
                    combineScore += tilesBoard[i][promotedStartColumnIndex].endValue;
                    totalScore += combineScore;
                }
            }

            // Re-Compress Tiles After Merging //
            if (!merged) continue;
            fillColumnIndex = GameConfig::BOARD_COLUMNS - 1;
            for (int j = GameConfig::BOARD_COLUMNS - 1; j >= 0; j--) {
                if (!tilesBoard[i][j].filled || tilesBoard[i][j].action == _2048Engine::TileAction::ABSORBED) continue;
                if (fillColumnIndex != j) {
                    tilesBoard[i][j].endColumnIndex = fillColumnIndex;
                    if (tilesBoard[i][j].action != _2048Engine::TileAction::PROMOTED) {
                        tilesBoard[i][j].action = _2048Engine::TileAction::MOVED;
                    }
                }
                --fillColumnIndex;
            }
        }
    }


    // Combine to the Top //
    else if (combineDirection == _2048Engine::CombineDirection::UP) {
        for (int i = 0; i < GameConfig::BOARD_COLUMNS; i++) {
            std::array<_2048Engine::Tile, GameConfig::BOARD_ROWS> tilesBoardColumn;

            // Compress Tiles -- Remove Zeroes //
            int fillRowIndex = 0;
            for (int j = 0; j < GameConfig::BOARD_ROWS; j++) {
                if (!tilesBoard[j][i].filled) continue;
                if (fillRowIndex != j) {
                    moved = true;
                    tilesBoard[j][i].endRowIndex = fillRowIndex;
                    tilesBoard[j][i].action = _2048Engine::TileAction::MOVED;
                }
                tilesBoardColumn[fillRowIndex++] = tilesBoard[j][i];
            }

            // Merge Tiles -- Combine Equal Adjacent Values //
            bool merged = false;
            for (int j = 0; j < fillRowIndex - 1; j++) {
                if (tilesBoardColumn[j].filled && tilesBoardColumn[j].action != _2048Engine::TileAction::ABSORBED && tilesBoardColumn[j].startValue == tilesBoardColumn[j + 1].startValue) {
                    // Update The Tiles After Finding Merge Sources //
                    unsigned int promotedStartRowIndex = tilesBoardColumn[j].startRowIndex;
                    unsigned int absorbedStartRowIndex = tilesBoardColumn[j + 1].startRowIndex;
                    tilesBoard[promotedStartRowIndex][i].endValue = tilesBoard[promotedStartRowIndex][i].startValue * 2;
                    tilesBoard[promotedStartRowIndex][i].action = _2048Engine::TileAction::PROMOTED;
                    tilesBoard[absorbedStartRowIndex][i].endValue = 0;
                    tilesBoard[absorbedStartRowIndex][i].action = _2048Engine::TileAction::ABSORBED;
                    tilesBoard[absorbedStartRowIndex][i].endRowIndex = j;
                    tilesBoardColumn[j] = tilesBoard[promotedStartRowIndex][i];
                    tilesBoardColumn[j + 1] = tilesBoard[absorbedStartRowIndex][i];

                    // Update Combine Result Metadata //
                    merged = true;
                    moved = true;
                    combineScore += tilesBoard[promotedStartRowIndex][i].endValue;
                    totalScore += combineScore;
                }
            }

            // Re-Compress Tiles Again After Merging -- Remove Zeros //
            if (!merged) continue;
            fillRowIndex = 0;
            for (int j = 0; j < GameConfig::BOARD_ROWS; j++) {
                if (!tilesBoard[j][i].filled || tilesBoard[j][i].action == _2048Engine::TileAction::ABSORBED) continue;
                if (fillRowIndex != j) {
                    tilesBoard[j][i].endRowIndex = fillRowIndex;
                    if (tilesBoard[j][i].action != _2048Engine::TileAction::PROMOTED) {
                        tilesBoard[j][i].action = _2048Engine::TileAction::MOVED;
                    }
                }
                ++fillRowIndex;
            }
        }
    }


    // Combine to the Bottom //
    else if (combineDirection == _2048Engine::CombineDirection::DOWN) {
        for (int i = 0; i < GameConfig::BOARD_COLUMNS; i++) {
            std::array<_2048Engine::Tile, GameConfig::BOARD_ROWS> tilesBoardColumn;

            // Compress Tiles -- Remove Zeroes //
            int fillRowIndex = GameConfig::BOARD_ROWS - 1;
            for (int j = GameConfig::BOARD_ROWS - 1; j >= 0; j--) {
                if (!tilesBoard[j][i].filled) continue;
                if (fillRowIndex != j) {
                    moved = true;
                    tilesBoard[j][i].endRowIndex = fillRowIndex;
                    tilesBoard[j][i].action = _2048Engine::TileAction::MOVED;
                }
                tilesBoardColumn[fillRowIndex--] = tilesBoard[j][i];
            }

            // Merge Tiles -- Combine Equal Adjacent Values //
            bool merged = false;
            for (int j = GameConfig::BOARD_ROWS - 1; j > 0; j--) {
                if (tilesBoardColumn[j].filled && tilesBoardColumn[j].action != _2048Engine::TileAction::ABSORBED && tilesBoardColumn[j].startValue == tilesBoardColumn[j - 1].startValue) {
                    // Update The Tiles After Finding Merge Sources //
                    unsigned int promotedStartRowIndex = tilesBoardColumn[j].startRowIndex;
                    unsigned int absorbedStartRowIndex = tilesBoardColumn[j - 1].startRowIndex;
                    tilesBoard[promotedStartRowIndex][i].endValue = tilesBoard[promotedStartRowIndex][i].startValue * 2;
                    tilesBoard[promotedStartRowIndex][i].action = _2048Engine::TileAction::PROMOTED;
                    tilesBoard[absorbedStartRowIndex][i].endValue = 0;
                    tilesBoard[absorbedStartRowIndex][i].action = _2048Engine::TileAction::ABSORBED;
                    tilesBoard[absorbedStartRowIndex][i].endRowIndex = j;
                    tilesBoardColumn[j] = tilesBoard[promotedStartRowIndex][i];
                    tilesBoardColumn[j - 1] = tilesBoard[absorbedStartRowIndex][i];

                    // Update Combine Result Metadata //
                    merged = true;
                    moved = true;
                    combineScore += tilesBoard[promotedStartRowIndex][i].endValue;
                    totalScore += combineScore;
                }
            }

            // Re-Compress Tiles Again After Merging -- Remove Zeros //
            if (!merged) continue;
            fillRowIndex = GameConfig::BOARD_ROWS - 1;
            for (int j = GameConfig::BOARD_ROWS - 1; j >= 0; j--) {
                if (!tilesBoard[j][i].filled || tilesBoard[j][i].action == _2048Engine::TileAction::ABSORBED) continue;
                if (fillRowIndex != j) {
                    tilesBoard[j][i].endRowIndex = fillRowIndex;
                    if (tilesBoard[j][i].action != _2048Engine::TileAction::PROMOTED) {
                        tilesBoard[j][i].action = _2048Engine::TileAction::MOVED;
                    }
                }
                --fillRowIndex;
            }
        }
    }

    // Populate the Tiles List with its New Tile States //
    tiles.clear();
    for (const auto tileRow : tilesBoard) {
        for (const auto tile : tileRow) {
            if (tile.filled) tiles.push_back(tile);
        }
    }

    // Detect if Game is Over and Spawn a New Tile if Game Still Going //
    bool gameOver = false;
    if (moved) {
        spawnPostCombineTile();
        gameOver = detectGameOver();
    }

    // Compile the Combine Result and Return It //
    _2048Engine::CombineResult combineResult = {
        .combineDirection = combineDirection,
        .gameOver = gameOver,
        .tilesMoved = moved,
        .combineScore = combineScore,
        .totalScore = this->totalScore,
        .tiles = this->tiles,
    };
    return combineResult;
}


/**
 * @brief Spawn the random tiles for the start of a new game.
 */
void _2048Engine::spawnStartTiles() {
    // Get Two Random Tile Placements and Values //
    static std::random_device random;
    static std::mt19937 generateRandom(random());
    std::uniform_int_distribution<> distribution(0, GameConfig::BOARD_ROWS * GameConfig::BOARD_COLUMNS - 1);
    std::uniform_int_distribution<> valueGenerator(0, 1);
    unsigned int randomIndex1 = distribution(generateRandom);
    unsigned int randomIndex2 = distribution(generateRandom);
    while (randomIndex2 == randomIndex1) randomIndex2 = distribution(generateRandom);
    unsigned int randomRowIndex1 = randomIndex1 / GameConfig::BOARD_COLUMNS;
    unsigned int randomColumnIndex1 = randomIndex1 % GameConfig::BOARD_COLUMNS;
    unsigned int randomRowIndex2 = randomIndex2 / GameConfig::BOARD_COLUMNS;
    unsigned int randomColumnIndex2 = randomIndex2 % GameConfig::BOARD_COLUMNS;
    unsigned int randomValue1 = (valueGenerator(generateRandom) == 0) ? 2 : 4;
    unsigned int randomValue2 = (valueGenerator(generateRandom) == 0) ? 2 : 4;

    // Spawn the Tiles with the Random Placements and Values //
    _2048Engine::Tile spawnedTile1 = {
        .filled = true,
        .action = _2048Engine::TileAction::SPAWNED,
        .startValue = randomValue1,
        .startRowIndex = randomRowIndex1,
        .startColumnIndex = randomColumnIndex1,
        .endValue = randomValue1,
        .endRowIndex = randomRowIndex1,
        .endColumnIndex = randomColumnIndex1
    };
    _2048Engine::Tile spawnedTile2 = {
        .filled = true,
        .action = _2048Engine::TileAction::SPAWNED,
        .startValue = randomValue2,
        .startRowIndex = randomRowIndex2,
        .startColumnIndex = randomColumnIndex2,
        .endValue = randomValue2,
        .endRowIndex = randomRowIndex2,
        .endColumnIndex = randomColumnIndex2
    };

    // Add the Tiles to the Tile List //
    tiles.push_back(spawnedTile1);
    tiles.push_back(spawnedTile2);
}


/**
 * @brief Spawn the random tile after a combine has occurred.
 */
void _2048Engine::spawnPostCombineTile() {
    // Fill with all Possible Linear Indices //
    std::vector<unsigned int> spawnIndices;
    for (int i = 0; i < GameConfig::BOARD_ROWS; i++) {
        for (int j = 0; j < GameConfig::BOARD_COLUMNS; j++) {
            spawnIndices.emplace_back((i * GameConfig::BOARD_COLUMNS) + j);
        }
    }

    // Eliminate Spawn Indices where Tiles have their End Indices Located //
    for (const auto& tile : tiles) {
        if (tile.action != _2048Engine::TileAction::ABSORBED) {
            unsigned int tileIndex = (tile.endRowIndex * GameConfig::BOARD_COLUMNS) + tile.endColumnIndex;
            spawnIndices.erase(std::remove_if(spawnIndices.begin(), spawnIndices.end(), [tileIndex](const unsigned int& spawnIndex) {
                return (spawnIndex == tileIndex);
            }), spawnIndices.end());
        }
    }

    // Generate the Random Spawn Index and its Random (2 or 4) Value //
    static std::random_device random;
    static std::mt19937 generator(random());
    std::uniform_int_distribution<> spawnDistribution(0, spawnIndices.size() - 1);
    std::uniform_int_distribution<> valueDistribution(0, 1);
    unsigned int randomSpawnValue = (valueDistribution(generator) == 0) ? 2 : 4;
    unsigned int randomSpawnIndex = spawnIndices[spawnDistribution(generator)];
    unsigned int randomSpawnRowIndex = randomSpawnIndex / GameConfig::BOARD_COLUMNS;
    unsigned int randomSpawnColumnIndex = randomSpawnIndex % GameConfig::BOARD_COLUMNS;

    // Spawn the Tile //
    _2048Engine::Tile spawnedTile = {
        .filled = true,
        .action = _2048Engine::TileAction::SPAWNED,
        .startValue = randomSpawnValue,
        .startRowIndex = randomSpawnRowIndex,
        .startColumnIndex = randomSpawnColumnIndex,
        .endValue = randomSpawnValue,
        .endRowIndex = randomSpawnRowIndex,
        .endColumnIndex = randomSpawnColumnIndex
    };
    tiles.push_back(spawnedTile);
}


/**
 * @brief Detect if the game is over (no mergeable tiles).
 *
 * @return True if the game is over, false if otherwise.
 */
bool _2048Engine::detectGameOver() {
    // Reconstruct the Tiles Board from End Indices //
    std::array<std::array<_2048Engine::Tile, GameConfig::BOARD_ROWS>, GameConfig::BOARD_COLUMNS> tilesBoard;
    for (const auto& tile : tiles) {
        if (tile.action != _2048Engine::TileAction::ABSORBED) {
            tilesBoard[tile.endRowIndex][tile.endColumnIndex] = tile;
        }
    }

    // Check if Still Empty Tiles Slots //
    for (const auto& tileRow : tilesBoard) {
        for (const auto& tile : tileRow) {
            if (!tile.filled) return false;
        }
    }

    // Check if Any Mergeable Tiles //
    for (int i = 0; i < tilesBoard.size() - 1; i++) {
        for (int j = 0; j < tilesBoard[i].size() - 1; j++) {
            if (j + 1 < tilesBoard[i].size() && tilesBoard[i][j].endValue == tilesBoard[i][j + 1].endValue) return false;
            if (i + 1 < tilesBoard.size() && tilesBoard[i][j].endValue == tilesBoard[i + 1][j].endValue) return false;
        }
    }

    return true;
}


/**
 * @brief Update the tiles in preparation for a new combine attempt.
 */
void _2048Engine::updateTilesForNewCombine() {
    // Remove All Tiles that aren't Filled or Have no Value or Were Absorbed //
    tiles.erase(std::remove_if(tiles.begin(), tiles.end(), [](const Tile& tile) {
        return (!tile.filled || tile.action == _2048Engine::TileAction::ABSORBED || tile.endValue == 0);
    }), tiles.end());

    // Reset State of Remaining Tiles for New Combine Attempt //
    for (auto& tile : tiles) {
        tile.action = _2048Engine::TileAction::HELD;
        tile.startValue = tile.endValue;
        tile.startRowIndex = tile.endRowIndex;
        tile.startColumnIndex = tile.endColumnIndex;
    }
}
