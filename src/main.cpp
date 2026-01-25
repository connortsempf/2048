/**
 * @file main.cpp
 * @brief 2048 Application Program Entry Point.
 * @author Connor Sempf
 * @date 2026-01-25
 * @version 1.0.0
 *
 * This file contains the main entry point for the 2048 game application.
 * It creates the application object and runs it indefinitely.
 */


#include <QApplication>
#include "ViewModel/2048App.hpp"


/**
 * @brief The main function to start up and run the application.
 *
 * @return The exit status code as an int.
 */
int main(int argc, char *argv[]) {
    QApplication _2048(argc, argv);
    _2048App _2048App;
    return _2048.exec();
}
