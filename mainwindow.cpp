#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QSound>
QString mediadir = "./media/"; //change mediadir2 in main.cpp aswell if needed

extern "C" {
int mains(int argc, char **argv);
}

void test(){
     char *argv1[]={"appname","-h","test"};
  //   char *argv1[]={"appname","-l",,ui->deviceCMB->currentText().toLocal8Bit().data(),"test"};
     int argc1 = sizeof(argv1) / sizeof(char*) - 1;



     mains(argc1,argv1);
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    char *argv1[]={"appname","-l",QString::number(ui->threshold->value()).toLocal8Bit().data(),"-d",ui->deviceCMB->currentText().toLocal8Bit().data(),"test"};
    int argc1 = sizeof(argv1) / sizeof(char*) - 1;



 //   mains(argc1,argv1);  to run the command





    QPixmap oPixmap(32,32);
    oPixmap.load ( mediadir + "trayicon.png");

    QIcon oIcon( oPixmap );

    trayIcon = new QSystemTrayIcon(oIcon);

    QAction *quit_action = new QAction( "Exit", trayIcon );
    connect( quit_action, SIGNAL(triggered()), this, SLOT(on_exit()) );

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction( quit_action );

    trayIcon->setContextMenu( trayIconMenu);
    trayIcon->setVisible(true);
    //trayIcon->showMessage("Test Message", "Text", QSystemTrayIcon::Information, 1000);
    //trayIcon->show();


    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));




}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
     case QSystemTrayIcon::Trigger:
         this->show();
         break;
     case QSystemTrayIcon::DoubleClick:
         this->show();
         break;
     case QSystemTrayIcon::MiddleClick:
        showMessage();
        break;

     default:
         ;
    }
}

void MainWindow::showMessage()
{
    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon();
    trayIcon->showMessage(tr("QSatisfy"), tr("Will you smoke now..."), icon, 100);
}
void MainWindow::on_exit()
{
    this->close();
    QApplication::quit();
}

void MainWindow::showEvent( QShowEvent* event ) {
    QWidget::showEvent( event );
}
