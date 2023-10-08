// Created by 李杨 on 2023/9/19.
//


#include "MainWindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(nullptr)
{
    //创建居中组件
    centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    //创建布局
    layout = new QVBoxLayout(centralWidget);
    buttonLayout = new QHBoxLayout();

    //创建按钮
    //传入的centralWidget用于指定widget的父对象，方便centralWidget销毁时自动销毁 label 和 button
    button = new QPushButton("点击我", centralWidget);
    label = new QLabel("这是一个标签", centralWidget);

    //将按钮和标签添加到布局中
    layout->addLayout(buttonLayout);
    layout->addWidget(label);

    buttonLayout->addStretch();
    buttonLayout->addWidget(button);
    buttonLayout->addStretch();

    bindSignals();

}

void MainWindow::bindSignals(){
    connect(button, &QPushButton::clicked, this, &MainWindow::OnButtonClicked);
}

int MainWindow::OnButtonClicked() {
    fileName = QFileDialog::getOpenFileName(this, "选择文件", "", "音视频(*.mp3 *.mp4 *.m3u8 *.flv)");
    button->setText(fileName);

    auto* delegate = new FFMpegDelegate();

    delegate->openFile(fileName);

    delegate->LogVideoResolution();

    delegate->TransContainer();

    delegate->CloseContext();

    delete delegate;
    return 0;
}

MainWindow::~MainWindow()= default;

