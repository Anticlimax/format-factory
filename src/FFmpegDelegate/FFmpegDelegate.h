//
// Created by 李杨 on 2023/9/22.
//

#ifndef FORMAT_FACTORY_FFMPEGDELEGATE_H
#define FORMAT_FACTORY_FFMPEGDELEGATE_H

#include <QString>
#include <QDebug>
#include <iostream>


extern "C"{
#include <libavformat/avformat.h>
}
class FFMpegDelegate {
public:
    FFMpegDelegate();
    ~FFMpegDelegate();
    // const QString& url 表示常量引用，避免值传递产生内存拷贝的开销
    void openFile(const QString& url);
    void LogVideoResolution ();
    void CloseContext ();
    int TransContainer();
private:
//    AVFormatContext是FFmpeg中的一个重要数据结构，用于表示音视频封装格式的上下文信息。
//    AVFormatContext类的作用是存储音视频文件或流的封装格式相关的信息，例如文件名、文件格式、音视频流等。它是在打开一个音视频文件或流时由FFmpeg库自动创建和填充的，包含了许多关于这个音视频文件或流的元数据和参数。
//    AVFormatContext中包含了以下一些重要的成员变量：
//        AVInputFormat/AVOutputFormat：表示音视频文件的输入或输出格式。
//        AVIOContext：表示文件的输入输出上下文。
//        AVStream：表示一个音频或视频流。
//        AVCodecContext：表示一个编解码器的上下文，包含编解码器的参数。
//        AVDictionary：存储了一些额外的封装格式相关的信息。
//    通过操作AVFormatContext和其中的成员变量，可以获取音视频文件的各种信息，如时长、格式、码率等，并进行相关的音视频解码和编码操作。
    AVFormatContext* inputContext = nullptr;
    AVFormatContext* outputContext = nullptr;
};



#endif //FORMAT_FACTORY_FFMPEGDELEGATE_H
