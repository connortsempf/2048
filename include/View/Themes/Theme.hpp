/**
 * @file Theme.hpp
 * @brief 2048 Color Theme Class Definition.
 * @date 2026-01-25
 * @version 1.0.0
 *
 * This file contains the definition of the application Theme, which
 * devises the outline for the application color themes for the UI and its components.
 */


#pragma once
#include <QColor>
#include <QObject>
#include <unordered_map>


/**
 * @brief Core game configuration constants.
 */
namespace Theme {

    static const QColor neutral1 = QColor(255, 255, 255);

    static const QColor neutral2 = QColor(250, 248, 239);

    static const QColor neutral3 = QColor(205, 193, 180);

    static const QColor neutral4 = QColor(187, 173, 160);

    static const QColor neutral5 = QColor(143, 122, 102);

    static const QColor neutral6 = QColor(120, 110, 101);

    static const QColor highlight1 = QColor(238, 228, 218);

    static const QColor highlight2 = QColor(237, 224, 200);

    static const QColor highlight3 = QColor(242, 177, 121);

    static const QColor highlight4 = QColor(245, 149, 99);

    static const QColor highlight5 = QColor(246, 124, 95);

    static const QColor highlight6 = QColor(246, 94, 59);

    static const QColor highlight7 = QColor(237, 207, 114);

    static const QColor highlight8 = QColor(237, 204, 97);

    static const QColor highlight9 = QColor(237, 200, 80);

    static const QColor highlight10 = QColor(237, 197, 63);

    static const QColor highlight11 = QColor(237, 194, 46);

    static const QColor highlight12 = QColor(62, 57, 51);

    struct Theme {
        struct Background { QColor background = neutral2; };
        struct Text { QColor text = neutral6; };
        struct Scoreboard { QColor background = neutral4; QColor headerText = highlight1; QColor scoreText = neutral1; };
        struct Button { QColor background = neutral5; QColor text = neutral2; };
        struct GameBoard { QColor tileBackgrounds = neutral3; QColor background = neutral4; };
        struct Tile { QColor background; QColor text; unsigned int fontSize; };

        const Background background;
        const Text text;
        const Scoreboard scoreboard;
        const Button button;
        const GameBoard gameBoard;
        const std::unordered_map<unsigned int, Tile> tiles = {
            {      2, {  highlight1, neutral6, 40 } },
            {      4, {  highlight2, neutral6, 40 } },
            {      8, {  highlight3, neutral2, 40 } },
            {     16, {  highlight4, neutral2, 40 } },
            {     32, {  highlight5, neutral2, 40 } },
            {     64, {  highlight6, neutral2, 40 } },
            {    128, {  highlight7, neutral2, 34 } },
            {    256, {  highlight8, neutral2, 34 } },
            {    512, {  highlight9, neutral2, 34 } },
            {   1024, { highlight10, neutral2, 26 } },
            {   2048, { highlight11, neutral2, 26 } },
            {   4096, { highlight12, neutral2, 26 } },
            {   8192, { highlight12, neutral2, 26 } },
            {  16384, { highlight12, neutral2, 23 } },
            {  32768, { highlight12, neutral2, 23 } },
            {  65536, { highlight12, neutral2, 23 } },
            { 131072, { highlight12, neutral2, 18 } },
        };
    };

    const Theme theme;
}
