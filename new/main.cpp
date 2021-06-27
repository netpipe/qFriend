#include "audiorecorder.h"

#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


    AudioRecorder recorder;
    recorder.show();
    recorder.hide();
    recorder.setWindowIcon(QIcon(":/trayicon.png"));


    return app.exec();
}
