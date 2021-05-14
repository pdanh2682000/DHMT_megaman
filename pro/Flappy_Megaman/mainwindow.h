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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void timerEvent(QTimerEvent *);
    int timerId;
    
private slots:
    void on_btnJump_clicked();

    void on_btnShiled_clicked();

    void on_btnBullet_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
