#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

/**
 * @class MainWindow
 * @brief Main application window for VulnScan GUI
 *
 * This is a placeholder for the main window.
 * Full implementation will be done in FASE 3.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void setupUi();
};

#endif // MAINWINDOW_H
