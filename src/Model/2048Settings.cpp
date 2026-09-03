/**
 * @file 2048Settings.hpp
 * @brief 2048 Settings Management Implementation.
 * @date 2026-01-25
 * @version 1.0.0
 *
 * This file contains the implementation of the _2048Settings class, which
 * manages the settings state and persistence for the application.
 */


#include "Model/2048Settings.hpp"


/**
 * @brief The resource file path for the default settings file.
 */
const QString _2048Settings::defaultSettingsPath = ":/config/default-settings.json";

/**
 * @brief The file name suffix for the config settings file.
 */
const QString _2048Settings::configSettingsFile = "/settings.json";


/**
 * @brief Load settings from disk (or create with defaults if doesn't exist).
 */
void _2048Settings::loadSettings() {
    QString configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir configDir(configPath);

    // Ensure Directory Exists //
    if (!configDir.exists()) {
        configDir.mkpath(".");
    }

    QString settingsFilePath = getSettingsFilePath();
    QFile settingsFile(settingsFilePath);

    // If Settings Don't Exist, Just use the Struct Default Settings //
    if (!settingsFile.exists()) {
        saveSettings();
        return;
    }

    // Open and Read Settings //
    if (!settingsFile.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray data = settingsFile.readAll();
    settingsFile.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        return;
    }

    if (!doc.isObject()) {
        return;
    }


    QJsonObject json = doc.object();

    // Window Maximized Setting //
    if (json.contains("is-window-maximized")) {
        settings.isWindowMaximized = json["is-window-maximized"].toBool();
    }

    // Window Size Setting //
    if (json.contains("window-size") && json["window-size"].isArray()) {
        QJsonArray sizeArray = json["window-size"].toArray();
        if (sizeArray.size() == 2) {
            settings.windowSize = QSize(sizeArray[0].toInt(), sizeArray[1].toInt());
        }
    }

    // Window Position Setting //
    if (json.contains("window-position") && json["window-position"].isArray()) {
        QJsonArray posArray = json["window-position"].toArray();
        if (posArray.size() == 2) {
            settings.windowPosition = QPoint(posArray[0].toInt(), posArray[1].toInt());
        }
    }   else {
        settings.windowPosition = QPoint(-1, -1);
    }

    // Best Score Setting //
    if (json.contains("best-score")) {
        settings.bestScore = json["best-score"].toInt();
    }
}


/**
 * @brief Save current settings to disk.
 */
void _2048Settings::saveSettings() {
    QString settingsFilePath = getSettingsFilePath();

    // Construct Settings //
    QJsonObject json;
    json["is-window-maximized"] = settings.isWindowMaximized;
    json["window-size"] = QJsonArray{settings.windowSize.width(), settings.windowSize.height()};
    json["window-position"] = QJsonArray{settings.windowPosition.x(), settings.windowPosition.y()};
    json["best-score"] = static_cast<int>(settings.bestScore);

    QJsonDocument doc(json);
    QFile file(settingsFilePath);

    // Write to Disk //
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
    }
}


/**
 * @brief Get current settings.
 *
 * @return Reference to the settings struct so it can be updated.
 */
_2048Settings::Settings& _2048Settings::getSettings() {
    return settings;
}


/**
 * @brief Get default settings.
 *
 * @return Reference to the defualt settings struct.
 */
const _2048Settings::Settings& _2048Settings::getDefaultSettings() {
    return defaultSettings;
}


/**
 * @brief The settings file path.
 */
QString _2048Settings::getSettingsFilePath() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + _2048Settings::configSettingsFile;
}
