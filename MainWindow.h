#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include "GLWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    void setupUi();
    void setupOpenGL();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    GLWidget *glWidget;
    QVBoxLayout *mainlayout;
};
#endif // MAINWINDOW_H
