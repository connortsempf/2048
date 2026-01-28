/**
 * @file 2048App.hpp
 * @brief 2048 Application Definition.
 * @date 2026-01-25
 * @version 1.0.0
 *
 * This file contains the definition of the _2048App class, which
 * manages the overall application state and UI components.
 */


#pragma once
#include <string>
#include <algorithm>
#include <QDateTime>
#include <QDebug>
#include <QTextStream>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QFontDatabase>
#include <QTimer>
#include <QEvent>
#include <QKeyEvent>
#include <QWidget>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QGuiApplication>
#include <QScreen>
#include "Model/GameConfig.hpp"
#include "Model/2048Engine.hpp"
#include "Model/2048Settings.hpp"
#include "View/2048UI.hpp"
#include "View/Themes/Theme.hpp"


/**
 * @brief Handle logging for debugging throughout the application.
 *
 * @param type The type of message being logged (Debug, Warning, Critical, Fatal, Info).
 * @param context The context information including file, line, and function where the message originated.
 * @param message The message text to be logged.
 */
void debugLogHandler(QtMsgType type, const QMessageLogContext &context, const QString &message);


/**
 * @brief The application managing class for the 2048 game.
 */
class _2048App : public QObject {

    Q_OBJECT

    public:
        /**
         * @brief Constructs a _2048App object.
         *
         * @param parent The parent QWidget.
         */
        explicit _2048App(QObject *parent = nullptr);

    protected:
        /**
         * @brief Event filter to capture application events
         */
        bool eventFilter(QObject* obj, QEvent* event) override;

    private:
        /**
         * @brief The current game score of the player.
         */
        unsigned int playerScore = 0;

        /**
         * @brief The 2048 application game logic manager.
         */
        _2048Engine engineManager;

        /**
         * @brief The 2048 application settings manager.
         */
        _2048Settings settingsManager;

        /**
         * @brief The 2048 application UI manager.
         */
        _2048UI* uiManager;

        /**
         * @brief Flag for if any game input will be acted upon.
         */
        bool acceptingGameInput = false;

    private:
        /**
         * @brief Load fonts for the application UI to use.
         */
        void loadFonts();

        /**
         * @brief Setup the application window.
         */
        void setupWindow();

        /**
         * @brief Handles the arrow key or WASD gameplay combine attempt input.
         *
         * @param keyEvent The key event that triggered the input.
         */
        void handleCombineInput(QKeyEvent* keyEvent);

        /**
         * @brief Handles play again or restart request input.
         */
        void handleNewGameRequestInput();
};
