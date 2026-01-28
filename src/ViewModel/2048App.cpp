/**
 * @file 2048App.cpp
 * @brief 2048 Application Implementation.
 * @date 2026-01-25
 * @version 1.0.0
 *
 * This file contains the implementation of the _2048App class, which
 * manages the overall application state and UI components.
 */


#include "ViewModel/2048App.hpp"


/**
 * @brief Handle logging for debugging throughout the application.
 *
 * @param type The type of message being logged (Debug, Warning, Critical, Fatal, Info).
 * @param context The context information including file, line, and function where the message originated.
 * @param message The message text to be logged.
 */
void debugLogHandler(QtMsgType type, const QMessageLogContext &context, const QString &message) {
    static QFile* logFile = nullptr;
    static QTextStream* logStream = nullptr;

    // Initialize on First Call //
    if (!logFile) {
        QString logPath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        QDir logDir(logPath);

        // Ensure the Directory Exists //
        if (!logDir.exists()) logDir.mkpath(".");

        QString logFilePath = logPath + "/debug.log";
        logFile = new QFile(logFilePath);
        if (logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            logStream = new QTextStream(logFile);
        }
    }

    // Write Message to Log //
    if (logStream) {
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        *logStream << "[" << timestamp << "] " << message << "\n";
        logStream->flush();
    }
}


/**
 * @brief Constructs a _2048App object.
 *
 * @param parent The parent QWidget.
 */
_2048App::_2048App(QObject *parent) : QObject(parent) {
    qApp->installEventFilter(this);
    qInstallMessageHandler(debugLogHandler);
    loadFonts();
    settingsManager.loadSettings();
    setupWindow();
    connect(uiManager, &_2048UI::newGameRequested, this, &_2048App::handleNewGameRequestInput);
    uiManager->setBestScore(settingsManager.getSettings().bestScore);
    if (settingsManager.getSettings().isWindowMaximized) uiManager->showMaximized();
    else uiManager->show();
    QTimer::singleShot(800, this, [this]() { handleNewGameRequestInput(); });
}


/**
 * @brief Event filter to capture application events
 */
bool _2048App::eventFilter(QObject* obj, QEvent* event) {
    // Handle Key Press Event Inputs //
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (acceptingGameInput && (
            keyEvent->key() == Qt::Key_Left ||
            keyEvent->key() == Qt::Key_Right ||
            keyEvent->key() == Qt::Key_Up ||
            keyEvent->key() == Qt::Key_Down)) {
            handleCombineInput(keyEvent);
        }
        return true;
    }

    // Handle Window Resize Events //
    else if (event->type() == QEvent::Resize) {
        if (obj == uiManager) {
            if (!uiManager->isMaximized()) {
                QResizeEvent* resizeEvent = static_cast<QResizeEvent*>(event);
                settingsManager.getSettings().windowSize.setWidth(resizeEvent->size().width());
                settingsManager.getSettings().windowSize.setHeight(resizeEvent->size().height());
            }
        }
    }

    // Handle Window Move Events //
    else if (event->type() == QEvent::Move) {
        if (obj == uiManager) {
            if (!uiManager->isMaximized()) {
                QMoveEvent* moveEvent = static_cast<QMoveEvent*>(event);
                settingsManager.getSettings().windowPosition.setX(moveEvent->pos().x());
                settingsManager.getSettings().windowPosition.setY(moveEvent->pos().y());
            }
        }
    }

    // Handle Window Close Events //
    else if (event->type() == QEvent::Close) {
        if (obj == uiManager) {
            QCloseEvent* closeEvent = static_cast<QCloseEvent*>(event);
            settingsManager.getSettings().isWindowMaximized = uiManager->isMaximized();
            settingsManager.saveSettings();
        }
    }

    // Pass On Base Implementation //
    return QObject::eventFilter(obj, event);
}


/**
 * @brief Load fonts for the application UI to use.
 */
void _2048App::loadFonts() {
    int fontID;
    std::vector<QString> fontPaths = {
        ":/fonts/assets/fonts/ClearSans-Regular-Normal.ttf",
        ":/fonts/assets/fonts/ClearSans-Regular-Medium.ttf",
        ":/fonts/assets/fonts/ClearSans-Regular-Bold.ttf"
    };

    for (const auto& fontPath : fontPaths) {
        fontID = QFontDatabase::addApplicationFont(fontPath);
        if (fontID == -1) {
            qWarning() << "Failed to Load Font:" << fontPath;
            continue;
        }

        QStringList families = QFontDatabase::applicationFontFamilies(fontID);
        if (!families.isEmpty()) {
            QString fontFamily = families.at(0);
            qDebug() << "Loaded Font Family:" << fontFamily;
        }
    }
}


/**
 * @brief Setup the application window.
 */
void _2048App::setupWindow() {
    // Get Display and Settings Information //
    QScreen* display = QGuiApplication::primaryScreen();
    QRect displayGeometry = display->geometry();
    _2048Settings::Settings& settings = settingsManager.getSettings();
    const _2048Settings::Settings& defaultSettings = settingsManager.getDefaultSettings();

    if (!settings.isWindowMaximized) {
        // Clamp the Window Size //
        settings.windowSize.setWidth(std::clamp(settings.windowSize.width(), defaultSettings.windowSize.width(), displayGeometry.width()));
        settings.windowSize.setHeight(std::clamp(settings.windowSize.height(), defaultSettings.windowSize.height(), displayGeometry.height()));

        // First check -- Is Window Completely Off-Screen //
        if (!displayGeometry.intersects(QRect(settings.windowPosition, settings.windowSize))) {
            settings.windowPosition.setX((displayGeometry.width() - settings.windowSize.width()) / 2);
            settings.windowPosition.setY((displayGeometry.height() - settings.windowSize.height()) / 2);
        }
        // Second Check -- Ensure Window is Fully Visible and Not Cut Off at Edges //
        else {
            // Window X-Position //
            if (settings.windowPosition.x() < 0 || settings.windowPosition.x() + settings.windowSize.width() > displayGeometry.width()) {
                settings.windowPosition.setX((displayGeometry.width() - settings.windowSize.width()) / 2);
            }
            // Window Y-Position //
            if (settings.windowPosition.y() < 0 || settings.windowPosition.y() + settings.windowSize.height() > displayGeometry.height()) {
                settings.windowPosition.setY((displayGeometry.height() - settings.windowSize.height()) / 2);
            }
        }
    }

    // Setup the Window and its UI //
    uiManager = new _2048UI();
    uiManager->setGeometry(settings.windowPosition.x(), settings.windowPosition.y(), settings.windowSize.width(), settings.windowSize.height());
    uiManager->setWindowTitle("2048");
}


/**
 * @brief Handles the arrow key or WASD gameplay combine attempt input.
 *
 * @param keyEvent The key event that triggered the input.
 */
void _2048App::handleCombineInput(QKeyEvent* keyEvent) {
    _2048Engine::CombineDirection combineDirection;
    if      (keyEvent->key() == Qt::Key_Left)   combineDirection = _2048Engine::CombineDirection::LEFT;
    else if (keyEvent->key() == Qt::Key_Right)  combineDirection = _2048Engine::CombineDirection::RIGHT;
    else if (keyEvent->key() == Qt::Key_Up)     combineDirection = _2048Engine::CombineDirection::UP;
    else if (keyEvent->key() == Qt::Key_Down)   combineDirection = _2048Engine::CombineDirection::DOWN;

    _2048Engine::CombineResult combineResult = engineManager.attemptCombine(combineDirection);
    if (!combineResult.tilesMoved) return;
    if (combineResult.gameOver) acceptingGameInput = false;
    playerScore += combineResult.combineScore;
    if (playerScore > settingsManager.getSettings().bestScore) {
        settingsManager.getSettings().bestScore = playerScore;
        uiManager->setBestScore(playerScore);
    }
    if (combineResult.combineScore > 0) uiManager->addToCurrentScore(combineResult.combineScore);
    uiManager->updateBoard(combineResult);
}


/**
 * @brief Handles play again or restart request input.
 */
void _2048App::handleNewGameRequestInput() {
    acceptingGameInput = true;
    uiManager->reset();
    _2048Engine::CombineResult combineResult = engineManager.startNewGame();
    uiManager->updateBoard(combineResult);
}
