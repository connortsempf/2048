/**
 * @file 2048Logging.cpp
 * @brief Logging Management for 2048.
 * @date 2026-01-25
 * @version 1.0.0
 *
 * This file contains implementation for the logging
 * management of the 2048 Application.
 */


#include "Model/2048Logging.hpp"


/**
 * @brief The active configuration's logging file path.
 */
QString _2048Logging::activeConfigurationModeLoggingFilePath = "";


/**
 * @brief Explicit constructor of a Log Manager object.
 */
_2048Logging::_2048Logging() :
    loggingDirectoryTitle("/logs"),
    debugModeLoggingDirectoryPath(QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(".." + loggingDirectoryTitle)),
    releaseModeLoggingDirectoryPath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + _2048Logging::loggingDirectoryTitle),
    loggingFileName("2048.log"),
    debugModeLoggingFilePath(debugModeLoggingDirectoryPath + "/" + loggingFileName),
    releaseModeLoggingFilePath(releaseModeLoggingDirectoryPath + "/" + loggingFileName) {

    #ifdef DEBUG
        _2048Logging::activeConfigurationModeLoggingFilePath = debugModeLoggingFilePath;
    #elif RELEASE
        _2048Logging::activeConfigurationModeLoggingFilePath = releaseModeLoggingFilePath;
    #else
        #error "No build configuration defined: expected DEBUG or RELEASE"
    #endif

    QDir loggingDir(QFileInfo(activeConfigurationModeLoggingFilePath).absolutePath());
    if (!loggingDir.exists()) {
        loggingDir.mkpath(".");
    }
}


/**
 * @brief The debug mode configuration logging file path.
 */
const QString& _2048Logging::getDebugModeLoggingFilePath() const {
    return debugModeLoggingFilePath;
}


/**
 * @brief The releasedebug mode configuration logging file path.
 */
const QString& _2048Logging::getReleaseModeLoggingFilePath() const {
    return releaseModeLoggingFilePath;
}


/**
 * @brief Handle application logs.
 *
 * @param type The type of message being logged (Debug, Warning, Critical, Fatal, Info).
 * @param context The context information including file, line, and function where the message originated.
 * @param message The message text to be logged.
 */
void _2048Logging::messageLoggingHandler(QtMsgType type, const QMessageLogContext& context, const QString& message) {
    QFile outFile(activeConfigurationModeLoggingFilePath);

    QString logType;
    switch (type) {
        case QtDebugMsg:
            logType = "DEBUG";
            break;
        case QtInfoMsg:
            logType = "INFO";
            break;
        case QtWarningMsg:
            logType = "WARNING";
            break;
        case QtCriticalMsg:
            logType = "CRITICAL";
            break;
        case QtFatalMsg:
            logType = "FATAL";
            break;
        default:
            logType = "UNKNOWN";
    }

    if (outFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream outfileTextStream(&outFile);
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        if (type == QtDebugMsg) {
            outfileTextStream << "[" << timestamp << "] | " << context.file << ":" << context.line << " | " << logType << " | " << message << "\n";
        }
        else {
            outfileTextStream << "[" << timestamp << "] | " << logType << " | " << message << "\n";
        }
        outFile.close();
    }
}
