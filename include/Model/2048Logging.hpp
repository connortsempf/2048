/**
 * @file 2048Logging.hpp
 * @brief Logging Management for 2048.
 * @date 2026-01-25
 * @version 1.0.0
 *
 * This file contains definition for the logging management
 * of the 2048 Application.
 */


#pragma once
#include <QString>
#include <QDir>
#include <QStandardPaths>
#include <QCoreApplication>


/**
 * @brief The logging managing class for the 2048 game.
 */
class _2048Logging {

    public:
        /**
         * @brief Explicit constructor of a Log Manager object.
         */
        explicit _2048Logging();

        /**
         * @brief Deleted Copy Constructor.
         */
        _2048Logging(const _2048Logging&) = delete;

        /**
         * @brief Deleted Copy Assignment Constructor.
         */
        _2048Logging& operator=(const _2048Logging&) = delete;

        /**
         * @brief Deleted Move Constructor.
         */
        _2048Logging(_2048Logging&&) = delete;

        /**
         * @brief Deleted Move Assignment Constructor.
         */
        _2048Logging& operator=(_2048Logging&&) = delete;

        /**
         * @brief The debug mode configuration logging file path.
         */
        const QString& getDebugModeLoggingFilePath() const;

        /**
         * @brief The releasedebug mode configuration logging file path.
         */
        const QString& getReleaseModeLoggingFilePath() const;

        /**
         * @brief Handle application logs.
         *
         * @param type The type of message being logged (Debug, Warning, Critical, Fatal, Info).
         * @param context The context information including file, line, and function where the message originated.
         * @param message The message text to be logged.
         */
        static void messageLoggingHandler(QtMsgType type, const QMessageLogContext& context, const QString& message);

    private:
        /**
         * @brief The title for the logging directory.
         */
        const QString loggingDirectoryTitle;

        /**
         * @brief The logging path for debug mode configuration logging location.
         */
        const QString debugModeLoggingDirectoryPath;

        /**
         * @brief The logging path for release mode configuration logging location.
         */
        const QString releaseModeLoggingDirectoryPath;

        /**
         * @brief The file name of logging file.
         */
        const QString loggingFileName;

        /**
         * @brief The full logging file path for debug mode configuration logging file.
         */
        const QString debugModeLoggingFilePath;

        /**
         * @brief The full logging file path for debug mode configuration logging file.
         */
        const QString releaseModeLoggingFilePath;

        /**
         * @brief The active configuration's logging file path.
         */
        static QString activeConfigurationModeLoggingFilePath;
};
