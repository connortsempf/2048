/**
 * @file GameBoard.cpp
 * @brief Game Board Component Implementation.
 * @date 2026-01-25
 * @version 1.0.0
 *
 * This file contains the implementation of the GameBoard class,
 * which represents the tile board in the 2048 UI.
 */


#include "View/Components/GameBoard.hpp"


/**
 * @brief The spacing between UI tiles to eachother and the edge of the board.
 */
const unsigned int GameBoard::TILE_MARGINS = 14;

/**
 * @brief The height and width of the board in pixels.
 */
const unsigned int GameBoard::BOARD_SIZE = 500;


/**
 * @brief Constructs a GameBoard object.
 *
 * @param parent The parent QWidget.
 */
GameBoard::GameBoard(QWidget *parent) : QWidget(parent) {
    // Calculate Sizing //
    unsigned int TILE_WIDTH = (GameBoard::BOARD_SIZE - (GameBoard::TILE_MARGINS * (GameConfig::BOARD_ROWS + 1))) / GameConfig::BOARD_ROWS;
    unsigned int TILE_HEIGHT = (GameBoard::BOARD_SIZE - (GameBoard::TILE_MARGINS * (GameConfig::BOARD_COLUMNS + 1))) / GameConfig::BOARD_COLUMNS;


    // Get Theme //
    Theme::Theme theme = Theme::theme;
    QColor backgroundColor = theme.gameBoard.background;
    QColor tileBackgroundsColor = theme.gameBoard.tileBackgrounds;


    // Construct Tile Backgrounds Layout //
    tileBackgroundsLayout = new QGridLayout(this);
    tileBackgroundsLayout->setAlignment(Qt::AlignCenter);
    tileBackgroundsLayout->setSpacing(GameBoard::TILE_MARGINS);
    tileBackgroundsLayout->setContentsMargins(GameBoard::TILE_MARGINS, GameBoard::TILE_MARGINS, GameBoard::TILE_MARGINS, GameBoard::TILE_MARGINS);

    // Construct Tile Backgrounds //
    for (int i = 0; i < tileBackgrounds.size(); i++) {
        for (int j = 0; j < tileBackgrounds[i].size(); j++) {
            tileBackgrounds[i][j] = new QWidget(this);
            tileBackgrounds[i][j]->setFixedSize(TILE_WIDTH, TILE_HEIGHT);
            QString tileStyle = QString("border-radius: 4px; background-color: rgb(%1, %2, %3);")
                .arg(tileBackgroundsColor.red()).arg(tileBackgroundsColor.green()).arg(tileBackgroundsColor.blue());
            tileBackgrounds[i][j]->setStyleSheet(tileStyle);
            tileBackgroundsLayout->addWidget(tileBackgrounds[i][j], i, j);
        }
    }


    // Construct Background //
    QWidget* background = new QWidget(this);
    background->setFixedSize(GameBoard::BOARD_SIZE, GameBoard::BOARD_SIZE);
    background->setContentsMargins(0, 0, 0, 0);
    QString backgroundStyle = QString("border: none; border-radius: 6px; background-color: rgb(%1, %2, %3);")
        .arg(backgroundColor.red()).arg(backgroundColor.green()).arg(backgroundColor.blue());
    background->setStyleSheet(backgroundStyle);
    background->setLayout(tileBackgroundsLayout);


    // Game Over Transparent Overlay //
    gameOverOverlay = new QWidget(this);
    gameOverOverlay->setGeometry(0, 0, GameBoard::BOARD_SIZE, GameBoard::BOARD_SIZE);
    gameOverOverlay->setContentsMargins(0, 0, 0, 0);
    gameOverOverlay->setStyleSheet("background-color: rgba(255, 255, 255, 0.36);");

    // Game Over Header //
    gameOverHeader = new QLabel("Game Over!", gameOverOverlay);
    gameOverHeader->setContentsMargins(0, 0, 0, 0);
    gameOverHeader->setFont(QFont("Clear Sans", 50, QFont::Bold));
    QColor headerTextColor = theme.text.text;
    gameOverHeader->setStyleSheet(QString("background-color: none; border: none; color: rgb(%1, %2, %3);")
        .arg(headerTextColor.red()).arg(headerTextColor.green()).arg(headerTextColor.blue())
    );

    // Game Over Try Again Button //
    gameOverTryAgainButton = new QPushButton("Try Again", gameOverOverlay);
    gameOverTryAgainButton->setFixedSize(125, 40);
    gameOverTryAgainButton->setContentsMargins(0, 0, 0, 0);
    gameOverTryAgainButton->setCursor(Qt::PointingHandCursor);
    gameOverTryAgainButton->setFont(QFont("Clear Sans", 13, QFont::Medium));
    QColor buttonTextColor = theme.button.text;
    QColor buttonBackgroundColor = theme.button.background;
    gameOverTryAgainButton->setStyleSheet(QString("border: none; border-radius: 4px; color: rgb(%1, %2, %3); background-color: rgb(%4, %5, %6); ")
    .arg(buttonTextColor.red()).arg(buttonTextColor.green()).arg(buttonTextColor.blue())
    .arg(buttonBackgroundColor.red()).arg(buttonBackgroundColor.green()).arg(buttonBackgroundColor.blue())
    );
    connect(gameOverTryAgainButton, &QPushButton::clicked, this, &GameBoard::newGameRequested);
    QVBoxLayout* gameOverTryAgainButtonLayout = new QVBoxLayout();
    gameOverTryAgainButtonLayout->setContentsMargins(0, 0, 0, 0);
    gameOverTryAgainButtonLayout->setAlignment(Qt::AlignCenter);
    gameOverTryAgainButtonLayout->addWidget(gameOverTryAgainButton);

    // Game Over Overlay Layout //
    QVBoxLayout* gameOverOverlayLayout = new QVBoxLayout(gameOverOverlay);
    gameOverOverlayLayout->setContentsMargins(0, 0, 0, 0);
    gameOverOverlayLayout->setAlignment(Qt::AlignCenter);
    gameOverOverlayLayout->addStretch();
    gameOverOverlayLayout->addWidget(gameOverHeader);
    gameOverOverlayLayout->addSpacing(50);
    gameOverOverlayLayout->addLayout(gameOverTryAgainButtonLayout);
    gameOverOverlayLayout->addStretch();
    gameOverOverlay->setLayout(gameOverOverlayLayout);
    gameOverOverlay->hide();


    // Construct Main Layout //
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(background);


    // Set the UI //
    setContentsMargins(0, 0, 0, 0);
    setFixedSize(GameBoard::BOARD_SIZE, GameBoard::BOARD_SIZE);
    setLayout(layout);
}


/**
 * @brief Reset the logical and visual state forof the board.
 */
void GameBoard::reset() {
    gameOverOverlay->hide();

    // Delete the Animations if Interrupted //
    if (moveAnimationGroup) {
        moveAnimationGroup->stop();
        delete moveAnimationGroup;
        moveAnimationGroup = nullptr;
    }
    if (spawnAnimationGroup) {
        spawnAnimationGroup->stop();
        delete spawnAnimationGroup;
        spawnAnimationGroup = nullptr;
    }

    // Delete the Tiles //
    for (auto& tile : tiles) {
        tile.label->setParent(nullptr);
        delete tile.label;
    }
    tiles.clear();
}


/**
 * @brief Updates the board with new tile positions and performs animations.
 *
     * @param combineResult The result of a combine action to implement.
 */
void GameBoard::updateBoard(_2048Engine::CombineResult combineResult) {
    reset();

    // Get the Theme //
    Theme::Theme theme = Theme::theme;

    // Create The Tiles at their Starting Positions in the Board //
    for (const auto& tileData : combineResult.tiles) {
        // Create the Tile Entry //
        GameBoard::Tile tile = {
            .label = new QLabel(QString::number(tileData.startValue), this),
            .data = tileData
        };

        // Find the Tile's Starting Position //
        QWidget* tileBackground = qobject_cast<QWidget*>(tileBackgroundsLayout->itemAtPosition(tileData.startRowIndex, tileData.startColumnIndex)->widget());

        // Construct the Tile Label //
        unsigned int tileFontSize = theme.tiles.at(tileData.startValue).fontSize;
        QColor tileTextColor = theme.tiles.at(tileData.startValue).text;
        QColor tileBackgroundColor = theme.tiles.at(tileData.startValue).background;
        tile.label->setFixedSize(tileBackground->width(), tileBackground->height());
        tile.label->move(tileBackground->pos());
        tile.label->setAlignment(Qt::AlignCenter);
        tile.label->setFont(QFont("Clear Sans", tileFontSize, QFont::Bold));
        QString tileStyle = QString("border-radius: 4px; color: rgb(%1, %2, %3); background-color: rgb(%4, %5, %6);")
            .arg(tileTextColor.red()).arg(tileTextColor.green()).arg(tileTextColor.blue())
            .arg(tileBackgroundColor.red()).arg(tileBackgroundColor.green()).arg(tileBackgroundColor.blue());
        tile.label->setStyleSheet(tileStyle);

        // Add the Tile to the Tiles Container //
        tiles.push_back(std::move(tile));
    }

    // Create Newly Spawned Tiles //
    connect(this, &GameBoard::moveAnimationsFinished, this, [this]() {
        animateTileSpawns();
    }, Qt::SingleShotConnection);

    // Check for and Handle Game Over Scenario //
    if (combineResult.gameOver) {
        connect(this, &GameBoard::spawnAnimationsFinished, this, [this]() {
            QTimer::singleShot(1000, this, [this]() { animateGameOverElementsEntry(); });
        }, Qt::SingleShotConnection);
    }

    animateTileMoves();
}


/**
 * @brief Move event override.
 *
 * @param event The move event.
 */
void GameBoard::moveEvent(QMoveEvent *event) {
    gameOverOverlay->setGeometry(0, 0, GameBoard::BOARD_SIZE, GameBoard::BOARD_SIZE);
}


/**
 * @brief Animate the tiles in the game board.
 */
void GameBoard::animateTileMoves() {
    // Delete the Animation if Previous is Interrupted //
    if (moveAnimationGroup) {
        moveAnimationGroup->stop();
        delete moveAnimationGroup;
    }

    // Get the Theme //
    Theme::Theme theme = Theme::theme;


    // Construct the All-Moves Parallel Animation Group //
    moveAnimationGroup = new QParallelAnimationGroup();


    // Animate Existing Tiles to their Final Spots //
    for (auto& tile : tiles) {
        if (tile.data.action != _2048Engine::TileAction::SPAWNED) {
            tile.label->raise();
            tile.label->show();
        }
        if (tile.data.action == _2048Engine::TileAction::HELD || tile.data.action == _2048Engine::TileAction::SPAWNED) continue;

        // Get Necessary Values //
        bool deleteTile = false;
        unsigned int endValue = tile.data.endValue;
        if (tile.data.action == _2048Engine::TileAction::ABSORBED) deleteTile = true;

        // Create the Tile's Animation Group //
        QSequentialAnimationGroup* tileAnimationGroup = new QSequentialAnimationGroup();

        // Calculate the Move Animation Positions and Timing //
        QWidget* endTileBackground = qobject_cast<QWidget*>(tileBackgroundsLayout->itemAtPosition(tile.data.endRowIndex, tile.data.endColumnIndex)->widget());
        QRect startGeometry = tile.label->geometry();
        QRect endGeometry(endTileBackground->pos(), tile.label->size());
        int moveDistance = std::abs((endGeometry.x() - startGeometry.x()) + (endGeometry.y() - startGeometry.y())); // Always 1D movement, so Just Add Together //

        // Create the Move Animation //
        if (moveDistance > 0) {
            QPropertyAnimation* moveAnimation = new QPropertyAnimation(tile.label, "geometry");
            moveAnimation->setDuration(100); // 1 px per millisecond pixel velocity //
            moveAnimation->setEasingCurve(QEasingCurve::Linear);
            moveAnimation->setStartValue(startGeometry);
            moveAnimation->setEndValue(endGeometry);
            tileAnimationGroup->addAnimation(moveAnimation);
            if (deleteTile) {
                QPointer<QLabel> tileLabel = tile.label;
                connect(moveAnimation, &QPropertyAnimation::finished, this, [tileLabel]() { if (tileLabel) tileLabel->hide(); }, Qt::SingleShotConnection);
            }
        }

        // Merge Animation is Necessary //
        if (tile.data.action == _2048Engine::TileAction::PROMOTED) {
            QPointer<QLabel> tileLabel = tile.label;

            // Calculate the Expand Animation Positions and Timing //
            QRect startGeometry = endTileBackground->geometry();
            QRect contractGeometry = startGeometry;
            int expandWidth = startGeometry.width() + (TILE_MARGINS * 1.75);
            int expandHeight = startGeometry.height() + (TILE_MARGINS * 1.75);
            tile.label->setMaximumSize(expandWidth, expandHeight);
            int expandX = startGeometry.x() - (expandWidth - startGeometry.width()) / 2;
            int expandY = startGeometry.y() - (expandHeight - startGeometry.height()) / 2;
            QRect expandGeometry(expandX, expandY, expandWidth, expandHeight);

            // Create the Expand Animation //
            QPropertyAnimation* expandAnimation = new QPropertyAnimation(tile.label, "geometry");
            expandAnimation->setDuration(100);
            expandAnimation->setEasingCurve(QEasingCurve::OutQuad);
            expandAnimation->setStartValue(startGeometry);
            expandAnimation->setEndValue(expandGeometry);
            tileAnimationGroup->addAnimation(expandAnimation);

            connect(expandAnimation, &QPropertyAnimation::finished, this, [tileLabel, endValue, theme]() {
                if (!tileLabel) return;
                unsigned int tileFontSize = theme.tiles.at(endValue).fontSize;
                QColor textColor = theme.tiles.at(endValue).text;
                QColor backgroundColor = theme.tiles.at(endValue).background;
                QString tileStyle = QString("border-radius: 4px; color: rgb(%1, %2, %3); background-color: rgb(%4, %5, %6);")
                .arg(textColor.red()).arg(textColor.green()).arg(textColor.blue())
                .arg(backgroundColor.red()).arg(backgroundColor.green()).arg(backgroundColor.blue());
                tileLabel->setFont(QFont("Clear Sans", tileFontSize, QFont::Bold));
                tileLabel->setText(QString::number(endValue));
                tileLabel->setStyleSheet(tileStyle);
            }, Qt::SingleShotConnection);

            // Create the Contract Animation //
            QPropertyAnimation* contractAnimation = new QPropertyAnimation(tile.label, "geometry");
            contractAnimation->setDuration(100);
            contractAnimation->setEasingCurve(QEasingCurve::InQuad);
            contractAnimation->setStartValue(expandGeometry);
            contractAnimation->setEndValue(contractGeometry);
            tileAnimationGroup->addAnimation(contractAnimation);
            connect(contractAnimation, &QPropertyAnimation::finished, this, [tileLabel, startGeometry]() {
                if (tileLabel) tileLabel->setFixedSize(startGeometry.width(), startGeometry.height());
            }, Qt::SingleShotConnection);
        }

        if (tileAnimationGroup->animationCount() > 0) moveAnimationGroup->addAnimation(tileAnimationGroup);
        else delete tileAnimationGroup;
    }

    connect(moveAnimationGroup, &QParallelAnimationGroup::finished, this, &GameBoard::moveAnimationsFinished, Qt::SingleShotConnection);
    moveAnimationGroup->start();
}


/**
 * @brief Animate the tile spawns in the game board.
 */
void GameBoard::animateTileSpawns() {
    // Delete the Animation if Previous is Interrupted //
    if (spawnAnimationGroup) {
        spawnAnimationGroup->stop();
        delete spawnAnimationGroup;
    }

    // Create the Parallel Spawn Animation Group //
    spawnAnimationGroup = new QParallelAnimationGroup();

    for (auto& tile : tiles) {
        if (tile.data.action == _2048Engine::TileAction::SPAWNED) {
            QPointer<QLabel> tileLabel = tile.label;
            if (!tileLabel) continue;

            tileLabel->setMinimumSize(0, 0);
            tileLabel->setMaximumSize(tileLabel->size());

            // Spawn Animation  //
            QPropertyAnimation* spawnAnimation = new QPropertyAnimation(tileLabel, "geometry");
            spawnAnimation->setDuration(185);
            spawnAnimation->setEasingCurve(QEasingCurve::OutQuint);
            QRect finalGeometry = tileLabel->geometry();
            int centerX = finalGeometry.x() + finalGeometry.width() / 2;
            int centerY = finalGeometry.y() + finalGeometry.height() / 2;
            QRect startGeometry(centerX, centerY, 0, 0);
            spawnAnimation->setStartValue(startGeometry);
            spawnAnimation->setEndValue(finalGeometry);

            // Run the Animations in Parallel //
            connect(spawnAnimation, &QPropertyAnimation::finished, this, [tile, tileLabel]() {
                tileLabel->setFixedSize(tileLabel->size());
            }, Qt::SingleShotConnection);

            spawnAnimationGroup->addAnimation(spawnAnimation);
            tileLabel->raise();
            tileLabel->show();
        }
    }

    connect(spawnAnimationGroup, &QParallelAnimationGroup::finished, this, &GameBoard::spawnAnimationsFinished);
    spawnAnimationGroup->start();
}


/**
 * @brief Animate the entry of the game over state UI elements.
 */
void GameBoard::animateGameOverElementsEntry() {
    gameOverOverlay->raise();
    gameOverOverlay->show();

    // Fade In Animation //
    QGraphicsOpacityEffect* transparencyEffect = new QGraphicsOpacityEffect();
    gameOverOverlay->setGraphicsEffect(transparencyEffect);
    QPropertyAnimation* fadeInAnimation = new QPropertyAnimation(transparencyEffect, "opacity");
    fadeInAnimation->setDuration(750);
    fadeInAnimation->setStartValue(0.0);
    fadeInAnimation->setEndValue(1.0);
    fadeInAnimation->setEasingCurve(QEasingCurve::InOutQuad);
    fadeInAnimation->start();
}
