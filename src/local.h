#pragma once
#include "src/qyuvopenglwidget.h"
#include "ui_local.h"
#include <QMainWindow>
#include <cstdint>


extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
}


class local : public QMainWindow {
    Q_OBJECT
    

private slots:
    void on_openButton_clicked();
    void updateFrame(uint8_t* dataY, uint8_t* dataU, uint8_t* dataV, uint32_t linesizeY, uint32_t linesizeU, uint32_t linesizeV);
public:
    local(QWidget* parent = nullptr);
    ~local();
signals:
    void updateFrameSignal(uint8_t* dataY, uint8_t* dataU, uint8_t* dataV, uint32_t linesizeY, uint32_t linesizeU, uint32_t linesizeV);
private:
    Ui_local* ui;
    QYUVOpenGLWidget *widget;
};