#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QMainWindow>
#include <QMediaRecorder>
#include <QUrl>
#include <QSystemTrayIcon>
#include <QAudioDeviceInfo>
#include <QAudioInput>
#include <QAudioOutput>

QT_BEGIN_NAMESPACE
namespace Ui { class AudioRecorder; }
class QAudioRecorder;
class QAudioProbe;
class QAudioBuffer;
QT_END_NAMESPACE

class AudioLevel;

class AudioRecorder : public QMainWindow
{
    Q_OBJECT

public:
    AudioRecorder();

public slots:
    void processBuffer(const QAudioBuffer&);

private slots:
    void setOutputLocation();
    void togglePause();
    void toggleRecord();

    void updateStatus(QMediaRecorder::Status);
    void onStateChanged(QMediaRecorder::State);
    void updateProgress(qint64 pos);
    void displayErrorMessage();

protected:
    /* Virtual function of the parent class in our class
     * Overridden to change the behavior of the application,
     * That it is minimized to tray when we want
     */
    void closeEvent(QCloseEvent * event);

private slots:
    /* The slot that will accept the signal from the event
     * Click on the application icon in the system tray
     */
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

    void on_horizontalSlider_thresoldlevel_sliderMoved(int position);

    void on_horizontalSlider_sliderMoved(int position);

    void AudioRestart();

    void StopRecording();

    void on_horizontalSlider_2_sliderMoved(int position);

private:
    void clearAudioLevels();

    Ui::AudioRecorder *ui = nullptr;

    QAudioRecorder *m_audioRecorder = nullptr;
    QAudioProbe *m_probe = nullptr;
    QList<AudioLevel*> m_audioLevels;
    bool m_outputLocationSet = false;
    /* Declare the object of future applications for the tray icon */
    QSystemTrayIcon         * trayIcon;

    QAudioDeviceInfo m_Inputdevice;
    QAudioDeviceInfo m_Outputdevice;
    QAudioFormat m_format;
    QAudioInput *m_audioInput;
    QAudioOutput *m_audioOutput;
    QIODevice *m_input;
    QIODevice *m_output;
    QByteArray m_buffer;
    int m_iVolume;
    QString fileName;
    int m_prtime  =10;

};

#endif // AUDIORECORDER_H
