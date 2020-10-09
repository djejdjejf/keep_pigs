#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "keep_pig.h"
#include <QtCharts>
#include <QCloseEvent>

using namespace QtCharts;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    keep_pig kp;
    explicit MainWindow(QWidget *parent = 0);
    QMainWindow c1,c2,c3;
    ~MainWindow();

    void load_file();
    void change_chart1();
    void change_chart23();
    void plague(int,int);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
