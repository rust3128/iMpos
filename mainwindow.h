#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionSettings_triggered();

    void on_actionUsers_triggered();

    void on_actionFuelName_triggered();

private:
    void createUI();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
