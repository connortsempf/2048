/**
 * @file 2048Settings.cpp
 * @brief 2048 Settings Management Definition.
 * @date 2026-01-25
 * @version 1.0.0
 *
 * This file contains the definition of the _2048Settings class, which
 * manages the settings state and persistence for the application.
 */


#pragma once
#include <QSize>
#include <QPoint>
#include <QString>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>


/**
 * @brief The blueprint for the 2048 application settings manager class.
 */
class _2048Settings {

    public:
        /**
         * @brief Settings data structure for persistent app configuration.
         */
        struct Settings {
            /**
             * @brief Window is maximized setting.
             */
            bool isWindowMaximized = false;

            /**
             * @brief Window size setting.
             */
            QSize windowSize = {685, 850};

            /**
             * @brief Window position setting.
             */
            QPoint windowPosition = {-1, -1};

            /**
             * @brief Best score statistic.
             */
            unsigned int bestScore = 0;
        };

    public:
        /**
         * @brief Default constructor - initializes settings with defaults.
         */
        _2048Settings() = default;

        /**
         * @brief Load settings from disk (or create with defaults if doesn't exist).
         */
        void loadSettings();

        /**
         * @brief Save current settings to disk.
         */
        void saveSettings();

        /**
         * @brief Get current settings.
         *
         * @return Reference to the settings struct so it can be updated.
         */
        Settings& getSettings();

        /**
         * @brief Get default settings.
         *
         * @return Reference to the defualt settings struct.
         */
        const Settings& getDefaultSettings();

        /**
         * @brief The settings file path.
         */
        QString getSettingsFilePath() const;

    private:
        /**
         * @brief The resource file path for the default settings file.
         */
        static const QString defaultSettingsPath;

        /**
         * @brief The file name suffix for the config settings file.
         */
        static const QString configSettingsFile;

    private:
        /**
         * @brief The settings container.
         */
        Settings settings;

        /**
         * @brief The default settings container.
         */
        const Settings defaultSettings;
};
