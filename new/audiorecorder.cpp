#include "audiorecorder.h"
#include "audiolevel.h"

#include "ui_audiorecorder.h"

#include <QAudioProbe>
#include <QAudioRecorder>
#include <QDir>
#include <QFileDialog>
#include <QMediaRecorder>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QStandardPaths>
#include <QDateTime>
#include <QMessageBox>
#include <QTimer>

static qreal getPeakValue(const QAudioFormat &format);
static QVector<qreal> getBufferLevels(const QAudioBuffer &buffer);

template <class T>
static QVector<qreal> getBufferLevels(const T *buffer, int frames, int channels);
QTimer *newtimer =nullptr;

AudioRecorder::AudioRecorder()
    : ui(new Ui::AudioRecorder)
{
    ui->setupUi(this);

    m_audioRecorder = new QAudioRecorder(this);
    m_probe = new QAudioProbe(this);
    connect(m_probe, &QAudioProbe::audioBufferProbed,
            this, &AudioRecorder::processBuffer);
    m_probe->setSource(m_audioRecorder);

    //audio devices
    ui->audioDeviceBox->addItem(tr("Default"), QVariant(QString()));
    for (auto &device: m_audioRecorder->audioInputs()) {
        ui->audioDeviceBox->addItem(device, QVariant(device));
    }

    //audio codecs
    ui->audioCodecBox->addItem(tr("Default"), QVariant(QString()));
    for (auto &codecName: m_audioRecorder->supportedAudioCodecs()) {
        ui->audioCodecBox->addItem(codecName, QVariant(codecName));
    }

    //containers
    ui->containerBox->addItem(tr("Default"), QVariant(QString()));
    for (auto &containerName: m_audioRecorder->supportedContainers()) {
        ui->containerBox->addItem(containerName, QVariant(containerName));
    }

    //sample rate
    ui->sampleRateBox->addItem(tr("Default"), QVariant(0));
    for (int sampleRate: m_audioRecorder->supportedAudioSampleRates()) {
        ui->sampleRateBox->addItem(QString::number(sampleRate), QVariant(
                                       sampleRate));
    }

    //channels
    ui->channelsBox->addItem(tr("Default"), QVariant(-1));
    ui->channelsBox->addItem(QStringLiteral("1"), QVariant(1));
    ui->channelsBox->addItem(QStringLiteral("2"), QVariant(2));
    ui->channelsBox->addItem(QStringLiteral("4"), QVariant(4));

    //quality
    ui->qualitySlider->setRange(0, int(QMultimedia::VeryHighQuality));
    ui->qualitySlider->setValue(int(QMultimedia::NormalQuality));

    //bitrates:
    ui->bitrateBox->addItem(tr("Default"), QVariant(0));
    ui->bitrateBox->addItem(QStringLiteral("32000"), QVariant(32000));
    ui->bitrateBox->addItem(QStringLiteral("64000"), QVariant(64000));
    ui->bitrateBox->addItem(QStringLiteral("96000"), QVariant(96000));
    ui->bitrateBox->addItem(QStringLiteral("128000"), QVariant(128000));

    connect(m_audioRecorder, &QAudioRecorder::durationChanged, this, &AudioRecorder::updateProgress);
    connect(m_audioRecorder, &QAudioRecorder::statusChanged, this, &AudioRecorder::updateStatus);
    connect(m_audioRecorder, &QAudioRecorder::stateChanged, this, &AudioRecorder::onStateChanged);
    connect(m_audioRecorder, QOverload<QMediaRecorder::Error>::of(&QAudioRecorder::error), this,
            &AudioRecorder::displayErrorMessage);
    this->setWindowTitle("qFriend");



    /* Initialize the tray icon, set the icon of a set of system icons,
        * As well as set a tooltip
        * */
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/trayicon.png"));

    /* After that create a context menu of two items */
    QMenu * menu = new QMenu(this);
    QAction * viewWindow = new QAction(trUtf8("View window"), this);
    QAction * quitAction = new QAction(trUtf8("Quit"), this);

    /* connect the signals clicks on menu items to by appropriate slots.
        * The first menu item expands the application from the tray,
        * And the second menu item terminates the application
        * */
    connect(viewWindow, SIGNAL(triggered()), this, SLOT(show()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

    menu->addAction(viewWindow);
    menu->addAction(quitAction);

    /* Set the context menu on the icon
        * And show the application icon in the system tray
        * */
    trayIcon->setContextMenu(menu);
    trayIcon->show();

    /* Also connect clicking on the icon to the signal processor of this press
        * */
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    ui->horizontalSlider->setValue(50);
    m_audioRecorder->setVolume(ui->horizontalSlider->value());
    ui->recordButton->setEnabled(false);
}

void AudioRecorder::updateProgress(qint64 duration)
{
    if (m_audioRecorder->error() != QMediaRecorder::NoError || duration < 2000)
        return;

    ui->statusbar->showMessage(tr("Recorded %1 sec").arg(duration / 1000));
}

void AudioRecorder::updateStatus(QMediaRecorder::Status status)
{
    QString statusMessage;

    switch (status) {
    case QMediaRecorder::RecordingStatus:
        statusMessage = tr("Recording to %1").arg(m_audioRecorder->actualLocation().toString());
        break;
    case QMediaRecorder::PausedStatus:
        clearAudioLevels();
        statusMessage = tr("Paused");
        break;
    case QMediaRecorder::UnloadedStatus:
    case QMediaRecorder::LoadedStatus:
        clearAudioLevels();
        statusMessage = tr("Stopped");
    default:
        break;
    }

    if (m_audioRecorder->error() == QMediaRecorder::NoError)
        ui->statusbar->showMessage(statusMessage);
}

void AudioRecorder::onStateChanged(QMediaRecorder::State state)
{
    switch (state) {
    case QMediaRecorder::RecordingState:
        ui->recordButton->setText(tr("Stop"));
        ui->pauseButton->setText(tr("Pause"));
        break;
    case QMediaRecorder::PausedState:
        ui->recordButton->setText(tr("Stop"));
        ui->pauseButton->setText(tr("Resume"));
        break;
    case QMediaRecorder::StoppedState:
        ui->recordButton->setText(tr("Record"));
        ui->pauseButton->setText(tr("Pause"));
        break;
    }

    ui->pauseButton->setEnabled(m_audioRecorder->state() != QMediaRecorder::StoppedState);
}

static QVariant boxValue(const QComboBox *box)
{
    int idx = box->currentIndex();
    if (idx == -1)
        return QVariant();

    return box->itemData(idx);
}

void AudioRecorder::toggleRecord()
{
    if (m_audioRecorder->state() == QMediaRecorder::StoppedState) {
        m_audioRecorder->setAudioInput(boxValue(ui->audioDeviceBox).toString());

        QAudioEncoderSettings settings;
        settings.setCodec(boxValue(ui->audioCodecBox).toString());
        settings.setSampleRate(boxValue(ui->sampleRateBox).toInt());
        settings.setBitRate(boxValue(ui->bitrateBox).toInt());
        settings.setChannelCount(boxValue(ui->channelsBox).toInt());
        settings.setQuality(QMultimedia::EncodingQuality(ui->qualitySlider->value()));
        settings.setEncodingMode(ui->constantQualityRadioButton->isChecked() ?
                                     QMultimedia::ConstantQualityEncoding :
                                     QMultimedia::ConstantBitRateEncoding);

        QString container = boxValue(ui->containerBox).toString();

        m_audioRecorder->setEncodingSettings(settings,
                                             QVideoEncoderSettings(), container);
        m_audioRecorder->record();
    }
    else {
        m_audioRecorder->stop();
    }
}

void AudioRecorder::togglePause()
{
    if (m_audioRecorder->state() != QMediaRecorder::PausedState)
        m_audioRecorder->pause();
    else
        m_audioRecorder->record();
}

void AudioRecorder::setOutputLocation()
{
#ifdef Q_OS_WINRT
    // UWP does not allow to store outside the sandbox
    const QString cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    if (!QDir().mkpath(cacheDir)) {
        qWarning() << "Failed to create cache directory";
        return;
    }
   fileName = cacheDir + QLatin1String("/output.wav");
#else
    fileName = QFileDialog::getExistingDirectory();
    fileName =fileName+"/"+QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
#endif
    m_audioRecorder->setOutputLocation(QUrl::fromLocalFile(fileName));
    m_outputLocationSet = true;
    ui->recordButton->setEnabled(true);
}

void AudioRecorder::displayErrorMessage()
{
    ui->statusbar->showMessage(m_audioRecorder->errorString());
}

void AudioRecorder::clearAudioLevels()
{
    for (int i = 0; i < m_audioLevels.size(); ++i)
        m_audioLevels.at(i)->setLevel(0);
}

// This function returns the maximum possible sample value for a given audio format
qreal getPeakValue(const QAudioFormat& format)
{
    // Note: Only the most common sample formats are supported
    if (!format.isValid())
        return qreal(0);

    if (format.codec() != "audio/pcm")
        return qreal(0);

    switch (format.sampleType()) {
    case QAudioFormat::Unknown:
        break;
    case QAudioFormat::Float:
        if (format.sampleSize() != 32) // other sample formats are not supported
            return qreal(0);
        return qreal(1.00003);
    case QAudioFormat::SignedInt:
        if (format.sampleSize() == 32)
            return qreal(INT_MAX);
        if (format.sampleSize() == 16)
            return qreal(SHRT_MAX);
        if (format.sampleSize() == 8)
            return qreal(CHAR_MAX);
        break;
    case QAudioFormat::UnSignedInt:
        if (format.sampleSize() == 32)
            return qreal(UINT_MAX);
        if (format.sampleSize() == 16)
            return qreal(USHRT_MAX);
        if (format.sampleSize() == 8)
            return qreal(UCHAR_MAX);
        break;
    }

    return qreal(0);
}

// returns the audio level for each channel
QVector<qreal> getBufferLevels(const QAudioBuffer& buffer)
{
    QVector<qreal> values;

    if (!buffer.format().isValid() || buffer.format().byteOrder() != QAudioFormat::LittleEndian)
        return values;

    if (buffer.format().codec() != "audio/pcm")
        return values;

    int channelCount = buffer.format().channelCount();
    values.fill(0, channelCount);
    qreal peak_value = getPeakValue(buffer.format());
    if (qFuzzyCompare(peak_value, qreal(0)))
        return values;

    switch (buffer.format().sampleType()) {
    case QAudioFormat::Unknown:
    case QAudioFormat::UnSignedInt:
        if (buffer.format().sampleSize() == 32)
            values = getBufferLevels(buffer.constData<quint32>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 16)
            values = getBufferLevels(buffer.constData<quint16>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 8)
            values = getBufferLevels(buffer.constData<quint8>(), buffer.frameCount(), channelCount);
        for (int i = 0; i < values.size(); ++i)
            values[i] = qAbs(values.at(i) - peak_value / 2) / (peak_value / 2);
        break;
    case QAudioFormat::Float:
        if (buffer.format().sampleSize() == 32) {
            values = getBufferLevels(buffer.constData<float>(), buffer.frameCount(), channelCount);
            for (int i = 0; i < values.size(); ++i)
                values[i] /= peak_value;
        }
        break;
    case QAudioFormat::SignedInt:
        if (buffer.format().sampleSize() == 32)
            values = getBufferLevels(buffer.constData<qint32>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 16)
            values = getBufferLevels(buffer.constData<qint16>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 8)
            values = getBufferLevels(buffer.constData<qint8>(), buffer.frameCount(), channelCount);
        for (int i = 0; i < values.size(); ++i)
            values[i] /= peak_value;
        break;
    }

    return values;
}

template <class T>
QVector<qreal> getBufferLevels(const T *buffer, int frames, int channels)
{
    QVector<qreal> max_values;
    max_values.fill(0, channels);

    for (int i = 0; i < frames; ++i) {
        for (int j = 0; j < channels; ++j) {
            qreal value = qAbs(qreal(buffer[i * channels + j]));
            if (value > max_values.at(j))
                max_values.replace(j, value);
        }
    }

    return max_values;
}
static bool staticrecordingstarted=false;
static bool isLowVolumeRecording =false;
void AudioRecorder::processBuffer(const QAudioBuffer& buffer)
{

    if (m_audioLevels.count() != buffer.format().channelCount()) {
        qDeleteAll(m_audioLevels);
        m_audioLevels.clear();
        for (int i = 0; i < buffer.format().channelCount(); ++i) {
            AudioLevel *level = new AudioLevel(ui->centralwidget);
            m_audioLevels.append(level);
            qDebug() <<" "<<level;
            ui->levelsLayout->addWidget(level);
        }
    }

    QVector<qreal> levels = getBufferLevels(buffer);

    for (int i = 0; i < levels.count(); ++i)
    {
        m_audioLevels.at(i)->setLevel(levels.at(i));
        int volume =levels.at(i)*CHAR_MAX;
       // qDebug() << "Muted"<<volume;
        if(volume<m_iVolume)
        {
            //qDebug() << "Muted"<<levels.at(i)*100;
            if(!staticrecordingstarted)
            {
               m_audioRecorder->setMuted(true);
            }
            if(!isLowVolumeRecording)
            {
                isLowVolumeRecording =true;
                newtimer =new QTimer();
                QObject::connect(newtimer, &QTimer::timeout,
                                 this,&AudioRecorder::AudioRestart);
                newtimer->start(1000*60*20);

            }
        }else
        {
            if(!staticrecordingstarted)
            {
                staticrecordingstarted  =true;
                  QTimer::singleShot(1000*60*m_prtime, this,
                                     &AudioRecorder::AudioRestart);
                m_audioRecorder->setMuted(false);
            }

            isLowVolumeRecording =false;
            if(newtimer !=nullptr)
            {
                newtimer->stop();
                delete newtimer;
                newtimer =nullptr;
            }

              qDebug() << "un muted Muted";
        }
    }
  //  qDebug() <<" "<<levels.at(i);

}

void AudioRecorder::StopRecording()
{
m_audioRecorder->stop();
}
void AudioRecorder::AudioRestart()
{
    staticrecordingstarted  =false;
}

/* The method that handles the closing event of the application window
 * */
void AudioRecorder::closeEvent(QCloseEvent * event)
{
   m_audioRecorder->stop();
}

/* The method that handles click on the application icon in the system tray
 * */
void AudioRecorder::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason){
    case QSystemTrayIcon::Trigger:
        /* The event is ignored if the checkbox is not checked
         * */
        //if(ui->trayCheckBox->isChecked())
    {
        /* otherwise, if the window is visible, it is hidden,
             * Conversely, if hidden, it unfolds on the screen
             * */
        if(!this->isVisible()){
            this->show();
        } else {
            this->hide();
        }
    }
        break;
    default:
        break;
    }
}

void AudioRecorder::on_horizontalSlider_thresoldlevel_sliderMoved(int position)
{
    qDebug()<< "Volume position "<<position;
   // m_audioRecorder->setVolume(position);
    m_iVolume =position;

}

//void AudioRecorder::closeEvent (QCloseEvent *event)
//{
//    QMessageBox::StandardButton resBtn = QMessageBox::question( this, APP_NAME,
//                                                                tr("Are you sure?\n"),
//                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
//                                                                QMessageBox::Yes);
//    if (resBtn != QMessageBox::Yes) {
//        event->ignore();
//    } else {
//        event->accept();
//    }
//}

void AudioRecorder::on_horizontalSlider_sliderMoved(int position)
{

    qDebug()<< "Volume position "<<position;
    m_audioRecorder->setVolume(position);
}

void AudioRecorder::on_horizontalSlider_2_sliderMoved(int position)
{
    m_prtime =position;
}
