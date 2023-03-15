#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpSocket = new QTcpSocket(this);
    image_byte = new QByteArray();

    connect(tcpSocket, &QTcpSocket::connected,[=](){ui->console->append("connect success!");});
    connect(tcpSocket,&QTcpSocket::readyRead,this,&MainWindow::read_image);

    image_width = 224;
    image_height = 224;

    int yuvSize = image_width * image_height * 3 /2;
    pFrame = av_frame_alloc();
    pFrameRGB = av_frame_alloc();
    int numBytes = avpicture_get_size(AV_PIX_FMT_RGB32, image_width,image_height);
    rgbBuffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    avpicture_fill((AVPicture *) pFrameRGB, rgbBuffer, AV_PIX_FMT_RGB32,image_width, image_height);
    img_convert_ctx = sws_getContext(image_width, image_height, AV_PIX_FMT_YUV420P, image_width, image_height, AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);

}

MainWindow::~MainWindow()
{
    sws_freeContext(img_convert_ctx);
    delete ui;
}





void MainWindow::on_connectCar_clicked()
{
    QString ip = ui->textEditIp->toPlainText();
    if(ip.isEmpty())
        ui->console->append("please enter ip address! ");
    else{
        tcpSocket->connectToHost(ip, 3823);
        ui->console->append("connecting to "+ip);
    }
}

void MainWindow::read_image()
{
    QByteArray bytearray = tcpSocket->readAll();
    image_byte->append(bytearray);
    if(image_byte->size()>=image_width * image_height * 3 /2){
        showImage();
    }

}




void MainWindow::on_videoMode_clicked()
{
    char cmd[6];
    cmd[0] = 0xfa;
    cmd[1] = 0xaa;
    cmd[2] = 0x07;//cmd

    if(ui->videoMode->text()=="Video: Turn On"){
        ui->videoMode->setText("Vedio: Turn Off");
        cmd[3] = 0x01;//param1
        cmd[4] = 0x01;//param2
        cmd[5] = 0x01;//param3
        ui->console->append("turn on the video! ");
    }
    else{
        ui->videoMode->setText("Video: Turn On");
        cmd[3] = 0x00;
        cmd[4] = 0x00;
        cmd[5] = 0x00;
        ui->console->append("turn off the video! ");
    }
    tcpSocket->write(cmd,6);
}


void MainWindow::showImage()
{
    //ui->console->append(QString("all:%1byte").arg(image_byte->size()));
    char* str = image_byte->data();
    avpicture_fill((AVPicture *) pFrame, (uint8_t *)str, AV_PIX_FMT_YUV420P, image_width, image_height);//这里的长度和高度跟之前保持一致
    //转换图像格式，将解压出来的YUV420P的图像转换为RGB的图像
    sws_scale(img_convert_ctx,
            (uint8_t const * const *) pFrame->data,
            pFrame->linesize, 0, image_height, pFrameRGB->data,
            pFrameRGB->linesize);
    //把这个RGB数据 用QImage加载
    QImage tmpImg((uchar *)rgbBuffer,image_width,image_height,QImage::Format_RGB32);
    QImage image = tmpImg.copy();
    image_byte->remove(0,image_width * image_height * 3 /2);
    ui->label->setPixmap(QPixmap::fromImage(image));

}




void MainWindow::on_pushButton_clicked()
{
    //right
    char cmd[6];
    cmd[0] = 0xfa;
    cmd[1] = 0xaa;
    cmd[2] = 0x05;//cmd

    cmd[3] = 0x00;//param1
    cmd[4] = 0x6E;//param2 110
    cmd[5] = 0x6E;//param3 110

    tcpSocket->write(cmd,6);
}


void MainWindow::on_pushButton_4_clicked()
{
    //Down
    char cmd[6];
    cmd[0] = 0xfa;
    cmd[1] = 0xaa;
    cmd[2] = 0x05;//cmd

    cmd[3] = 0x00;//param1
    cmd[4] = 0x00;//param2
    cmd[5] = 0x5A;//param3 90

    tcpSocket->write(cmd,6);
}


void MainWindow::on_pushButton_3_clicked()
{
    //left
    char cmd[6];
    cmd[0] = 0xfa;
    cmd[1] = 0xaa;
    cmd[2] = 0x05;//cmd

    cmd[3] = 0x00;//param1
    cmd[4] = 0x5A;//param2 90
    cmd[5] = 0x6E;//param3 110

    tcpSocket->write(cmd,6);
}


void MainWindow::on_pushButton_2_clicked()
{
    //up
    char cmd[6];
    cmd[0] = 0xfa;
    cmd[1] = 0xaa;
    cmd[2] = 0x05;//cmd

    cmd[3] = 0x00;//param1
    cmd[4] = 0x64;//param2 100
    cmd[5] = 0x6E;//param3 110

    tcpSocket->write(cmd,6);
}

