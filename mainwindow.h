#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void showEvent(QShowEvent *ev);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
        void showMessage();
        QSystemTrayIcon *trayIcon;
        QMenu *trayIconMenu;

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void on_exit();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
