//
// Created by 李杨 on 2023/9/19.
//


#ifndef FORMAT_FACTORY_MAINWINDOW_H
#define FORMAT_FACTORY_MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QDebug>
#include "./FFmpegDelegate/FFmpegDelegate.h"




class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    //在C++中，当一个类继承自另一个类并且基类的析构函数被声明为虚函数时，派生类应该使用override关键字来重写基类的析构函数。
    //这是因为当使用基类指针或引用指向派生类的对象时，通过调用基类的虚析构函数，可以确保在销毁对象时，先调用派生类的析构函数，然后再调用基类的析构函数。这种行为被称为"多态析构"。
    //如果派生类没有使用override关键字重写基类的析构函数，那么在销毁派生类对象时，如果使用基类指针来删除派生类对象，只会调用基类的析构函数，而不会调用派生类的析构函数。这可能导致派生类中分配的资源无法被正确地释放，从而引发内存泄漏或其他问题。
    //通过使用override关键字，可以确保在派生类中正确地重写基类的析构函数，并确保在销毁对象时调用正确的析构函数，从而实现正确的对象销毁和资源释放。
    ~MainWindow() override;

    void bindSignals();

    int OnButtonClicked();

private:
    QWidget *centralWidget;
    QVBoxLayout *layout;
    QHBoxLayout *buttonLayout;
    QPushButton *button;
    QLabel *label;
    QString fileName;

};


#endif
//FORMAT_FACTORY_MAINWINDOW_H

