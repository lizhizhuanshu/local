#include "local.h"

#include <QImage>
#include <QDebug>
#include <qtypes.h>
#include <QTimer>
#include "qyuvopenglwidget.h"


QImage loadImage(const QString &imagePath) {
    QImage image(imagePath);
    if (image.isNull()) {
        qDebug() << "Failed to load image!";
        return QImage();
    }
    return image;
}

void rgbToYUV420P(const QImage &rgbImage, uchar *yuvData) {
    int width = rgbImage.width();
    int height = rgbImage.height();

    // Y 分量：图像宽*高
    int ySize = width * height;
    uchar *yData = yuvData;

    // UV 分量：图像宽/2 * 高/2
    int uvSize = (width / 2) * (height / 2);
    uchar *uData = yData + ySize;
    uchar *vData = uData + uvSize;

    // 遍历图像，进行 RGB 到 YUV 转换
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            // 获取 RGB 值
            QColor color(rgbImage.pixel(i, j));
            int r = color.red();
            int g = color.green();
            int b = color.blue();

            // 计算 YUV 值
            int y = (0.299 * r + 0.587 * g + 0.114 * b);
            int u = (-0.147 * r - 0.289 * g + 0.436 * b) + 128;
            int v = (0.615 * r - 0.515 * g - 0.100 * b) + 128;

            // 限制 Y, U, V 范围
            y = std::max(0, std::min(255, y));
            u = std::max(0, std::min(255, u));
            v = std::max(0, std::min(255, v));

            // 填充 Y 分量
            yData[j * width + i] = static_cast<uchar>(y);

            // UV 分量需要按照 2x2 分块方式处理，取样 (down-sampling)
            if (i % 2 == 0 && j % 2 == 0) {
                uData[(j / 2) * (width / 2) + (i / 2)] = static_cast<uchar>(u);
                vData[(j / 2) * (width / 2) + (i / 2)] = static_cast<uchar>(v);
            }
        }
    }
}


local::local(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_local)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &local::on_openButton_clicked);
    connect(this, &local::updateFrameSignal, this, &local::updateFrame);
}

local::~local()
{
    delete ui; 
}

void local::on_openButton_clicked(){
    auto path = "/home/lizhi/Downloads/background.png";
    QImage image = loadImage(path);
    if (image.isNull()) {
        qDebug()<< "load image error";
        return;
    }
    uchar *yuvData = new uchar[image.width() * image.height() * 3 / 2];
    rgbToYUV420P(image, yuvData);

    int ySize = image.width() * image.height();
    int uvSize = image.width() * image.height() / 4;
    uchar *yData = yuvData;
    uchar *uData = yData + ySize;
    uchar *vData = uData + uvSize;

    uint32_t yStride = image.width();
    uint32_t uvStride = image.width() / 2;

    widget = new QYUVOpenGLWidget();
    widget->show();
    widget->setFixedSize(image.width(), image.height());
    widget->setFrameSize(QSize(image.width(), image.height()));

    auto timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, [this,yData, uData, vData, yStride,uvStride](){
        
        emit updateFrameSignal(yData, uData, vData, yStride, uvStride, uvStride);
    });
    timer->start();
}

void local::updateFrame(uint8_t* dataY, uint8_t* dataU, uint8_t* dataV, uint32_t linesizeY, uint32_t linesizeU, uint32_t linesizeV){
    widget->updateTextures(dataY, dataU, dataV, linesizeY, linesizeU, linesizeV);
}