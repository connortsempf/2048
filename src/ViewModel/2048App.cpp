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
    // Ensure the logs Directory Exists //
    QDir logDir("../logs");
    if (!logDir.exists()) logDir.mkpath(".");

    // Log the Message //
    QFile outFile("../logs/debug.log");
    if (outFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream ts(&outFile);
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        ts << "[" << timestamp << "] " << message << "\n";
        outFile.close();
    }
}


/**
 * @brief Constructs a _2048App object.
 *
 * @param parent The parent QWidget.
 */
_2048App::_2048App(QObject *parent) : QObject(parent) {
    // App Configurations //
    qApp->installEventFilter(this);
    qInstallMessageHandler(debugLogHandler);

    // Load Application Assets //
    loadFonts();
    loadSettings();

    // Construct Window //
    ui = new _2048UI();
    QScreen* display = QGuiApplication::primaryScreen();
    QRect displayGeometry = display->geometry();
    if (settings.windowSize.width() < 685) settings.windowSize.setWidth(685);
    else if (settings.windowSize.width() > displayGeometry.width()) settings.windowSize.setWidth(displayGeometry.width());
    if (settings.windowSize.height() < 850) settings.windowSize.setHeight(850);
    else if (settings.windowSize.height() > displayGeometry.height()) settings.windowSize.setWidth(displayGeometry.height());
    if (settings.windowPosition.x() < 0 || settings.windowPosition.x() > displayGeometry.width()) settings.windowPosition.setX((displayGeometry.width() - settings.windowSize.width()) / 2);
    if (settings.windowPosition.y() < 0 || settings.windowPosition.y() > displayGeometry.height()) settings.windowPosition.setY((displayGeometry.height() - settings.windowSize.height()) / 2);
    ui->setGeometry(settings.windowPosition.x(), settings.windowPosition.y(), settings.windowSize.width(), settings.windowSize.height());
    ui->setWindowTitle("2048");
    ui->setWindowIcon(QIcon("../assets/textures/window-icon.png"));

    // Connect UI Interaction Signals //
    connect(ui, &_2048UI::newGameRequested, this, &_2048App::handleNewGameRequestInput);

    // Configure Default Start State for Application //
    ui->setBestScore(settings.bestScore);
    ui->show();
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
        if (obj == ui) {
            QResizeEvent* resizeEvent = static_cast<QResizeEvent*>(event);
            settings.windowSize.setWidth(resizeEvent->size().width());
            settings.windowSize.setHeight(resizeEvent->size().height());
        }
    }

    // Handle Window Move Events //
    else if (event->type() == QEvent::Move) {
        if (obj == ui) {
            QMoveEvent* moveEvent = static_cast<QMoveEvent*>(event);
            settings.windowPosition.setX(moveEvent->pos().x());
            settings.windowPosition.setY(moveEvent->pos().y());
        }
    }

    // Handle Window Close Events //
    else if (event->type() == QEvent::Close) {
        if (obj == ui) {
            QCloseEvent* closeEvent = static_cast<QCloseEvent*>(event);
            saveSettings();
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
    QString fontFamily;
    std::vector<QString> fontPaths = {
        "../assets/fonts/ClearSans-Regular-Normal.ttf",
        "../assets/fonts/ClearSans-Regular-Medium.ttf",
        "../assets/fonts/ClearSans-Regular-Bold.ttf"
    };

    for (const auto& fontPath : fontPaths) {
        fontID = QFontDatabase::addApplicationFont(fontPath);
        if (fontID == -1) qWarning() << "Failed to Load Font" << fontPath;
        fontFamily = QFontDatabase::applicationFontFamilies(fontID).at(0);
    }
}


/**
 * @brief Load saved application settings.
 */
void _2048App::loadSettings() {
    QString userSettingsFilePath = "../assets/settings/settings.json";
    QFile userSettingsFile(userSettingsFilePath);

    if (!userSettingsFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open settings file for reading:" << userSettingsFilePath;
        return;
    }
    QByteArray data = userSettingsFile.readAll();
    userSettingsFile.close();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (!doc.isObject()) {
        qWarning() << "Invalid JSON format";
        return;
    }
    QJsonObject json = doc.object();

    // Window Size //
    if (json.contains("window-size") && json["window-size"].isArray()) {
        QJsonArray sizeArray = json["window-size"].toArray();
        if (sizeArray.size() == 2) {
            settings.windowSize = QSize(sizeArray[0].toInt(), sizeArray[1].toInt());
        }
    }

    // Window Position //
    if (json.contains("window-position") && !json["window-position"].isNull() && json["window-position"].isArray()) {
        QJsonArray posArray = json["window-position"].toArray();
        if (posArray.size() == 2) {
            settings.windowPosition = QPoint(posArray[0].toInt(), posArray[1].toInt());
        }
    }   else {
        settings.windowPosition = QPoint(-1, -1);
    }

    // Best Score Statistic //
    settings.bestScore = json["best-score"].toInt();
}


/**
 * @brief Save user application to disk.
 */
void _2048App::saveSettings() {
    QJsonObject json;
    QString userSettingsFilePath = "../assets/settings/settings.json";

    // Window Size //
    QJsonArray windowSizeArray;
    windowSizeArray.append(settings.windowSize.width());
    windowSizeArray.append(settings.windowSize.height());
    json["window-size"] = windowSizeArray;

    // Window Position (Null if Invalid) //
    if (settings.windowPosition.isNull()) {
        json["window-position"] = QJsonValue::Null;
    }   else {
        QJsonArray windowPosArray;
        windowPosArray.append(settings.windowPosition.x());
        windowPosArray.append(settings.windowPosition.y());
        json["window-position"] = windowPosArray;
    }

    // Best Score Statistic //
    json["best-score"] = static_cast<int>(settings.bestScore);

    // Create Directory if it Doesn't Exist //
    QFileInfo fileInfo(userSettingsFilePath);
    QDir dir = fileInfo.absoluteDir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    // Write to User Settings to File //
    QFile file(userSettingsFilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Could not open settings file for writing:" << userSettingsFilePath;
        return;
    }
    QJsonDocument doc(json);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
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

    _2048Engine::CombineResult combineResult = engine.attemptCombine(combineDirection);
    if (!combineResult.tilesMoved) return;
    if (combineResult.gameOver) acceptingGameInput = false;
    playerScore += combineResult.combineScore;
    if (playerScore > settings.bestScore) {
        settings.bestScore = playerScore;
        ui->setBestScore(playerScore);
    }
    if (combineResult.combineScore > 0) ui->addToCurrentScore(combineResult.combineScore);
    ui->updateBoard(combineResult);
}


/**
 * @brief Handles play again or restart request input.
 */
void _2048App::handleNewGameRequestInput() {
    acceptingGameInput = true;
    ui->reset();
    _2048Engine::CombineResult combineResult = engine.startNewGame();
    ui->updateBoard(combineResult);
}
