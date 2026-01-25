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
#include <QDateTime>
#include <QDebug>
#include <QTextStream>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
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
         * @brief 2048 settings struct for persistent app data.
         */
        struct _2048Settings {
            /**
             * @brief Window size setting.
             */
            QSize windowSize = {625, 800};

            /**
             * @brief Window position settings
             */
            QPoint windowPosition = {-1, -1};

            /**
             * @brief Best score statistic setting.
             */
            unsigned int bestScore = 0;
        };

    private:
        /**
         * @brief The current game score of the player.
         */
        unsigned int playerScore = 0;

        /**
         * @brief The 2048 game logic manager for the application.
         */
        _2048Engine engine;

        /**
         * @brief The 2048 UI for the application.
         */
        _2048UI* ui;

        /**
         * @brief Cache of 2048 settings to track during runtime and save for persistent settings tracking.
         */
        _2048Settings settings;

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
         * @brief Load saved application settings.
         */
        void loadSettings();

        /**
         * @brief Save user application to disk.
         */
        void saveSettings();

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
