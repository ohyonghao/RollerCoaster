#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();

    this->setCentralWidget(new QWidget);
    this->centralWidget()->setLayout(mainlayout);
    this->setWindowTitle("Roller Coaster");
}

MainWindow::~MainWindow()
{

}

void MainWindow::setupUi(){
    mainlayout = new QVBoxLayout;

    glWidget = new GLWidget;
    mainlayout->addWidget(glWidget);
}
