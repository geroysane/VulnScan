#include "MainWindow.h"
#include <QMenuBar>
#include <QStatusBar>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUi() {
    setWindowTitle("VulnScan - Network Vulnerability Scanner");
    resize(800, 600);

    // Create menu bar
    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    // File menu
    QMenu* fileMenu = menuBar->addMenu("&File");
    fileMenu->addAction("E&xit", this, &QWidget::close);

    // Status bar
    QStatusBar* statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
    statusBar->showMessage("Ready");

    // Central widget placeholder
    QLabel* placeholder = new QLabel("VulnScan GUI\n\nFull interface will be implemented in FASE 3", this);
    placeholder->setAlignment(Qt::AlignCenter);
    setCentralWidget(placeholder);
}
