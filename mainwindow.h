#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavdevice/avdevice.h"
}

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QByteArray *image_byte;

private:
    Ui::MainWindow *ui;
    QTcpSocket *tcpSocket;

    AVFrame *pFrame ;
    AVFrame *pFrameRGB;
    uint8_t * rgbBuffer;
    SwsContext *img_convert_ctx;
    int image_width,image_height;

    void showImage();

private slots:
    void on_connectCar_clicked();
    void read_image();
    void on_videoMode_clicked();



    void on_pushButton_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
};
#endif // MAINWINDOW_H
