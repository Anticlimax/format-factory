//
// Created by 李杨 on 2023/9/22.
//

#include "FFmpegDelegate.h"

void FFMpegDelegate::openFile(const QString& url) {
// 为什么要拆成 2 行写
// 这种行为是基于 C++ 的自动变量（automatic variables）的生命周期原则。在函数的局部作用域中定义的自动变量，例如 QByteArray 对象，会在离开该作用域时自动被销毁。这是为了确保内存资源的正确释放和增加程序的效率。
// 因此，在第一个代码片段中，url.toUtf8().constData() 返回的临时 QByteArray 对象在当前语句执行结束后就被销毁了，导致返回的指针 urlPtr 变得无效。
    QByteArray byteArray = url.toUtf8();
    const char* urlPtr = byteArray.constData();
//    avformat_open_input是FFmpeg库中的一个函数，其作用是打开一个音频/视频文件或输入流，并将其解析为AVFormatContext对象。
    int result = avformat_open_input(&context, urlPtr, nullptr, nullptr);
    if(result != 0){
        qDebug() << "Failed to open input file";
    }
//    avformat_find_stream_info是FFmpeg库中的一个函数，其作用是读取音频/视频文件的流信息，并将其存储在AVFormatContext对象中。
//    当打开一个音频/视频文件后，文件中可能包含多个音频流、视频流或其他类型的流。avformat_find_stream_info函数会遍历文件中的所有流，并从中提取流的相关信息，例如文件格式、编码器、流参数等。这些信息可以帮助后续的音频/视频处理过程。
//    调用avformat_find_stream_info函数可以确保AVFormatContext对象中的流信息已被完整地填充。此函数在解码之前被调用，以便准备好音频/视频数据的解码过程。可以通过访问AVFormatContext对象来获取有关流的详细信息，例如时长、帧率、分辨率等。
//    需要注意的是，该函数可能需要一段时间才能执行完毕，因为它需要遍历整个文件来获取流的信息。因此，在调用该函数时，可能需要进行适当的等待，直到所有流的信息都被获取和填充到AVFormatContext对象中。
    result = avformat_find_stream_info(context, nullptr);
    if(result < 0){
        qDebug() << "Failed to find stream information";
        avformat_close_input(&context);
    }
}

void FFMpegDelegate::LogVideoResolution() {
    int streamIndex;
//        先使用formatContext->nb_streams获取流的数量，遍历其中的所有流，找到视频流，提取它的宽高
    for (streamIndex = 0; streamIndex < context->nb_streams; ++streamIndex) {
//        AVStream 是 FFmpeg 中的一个结构体，它用于表示音视频文件或输入流中的一个音视频流。AVStream 包含了音视频流的各种信息，例如流的索引、时长、帧率、码率、解码器参数等。
//        在 AVFormatContext 中，每个音视频流对应着一个 AVStream 结构体。AVStream 结构体中包含了一个指向 AVCodecParameters 结构体的指针，该指针用于存储该音视频流的编解码器参数。
//        通过 AVStream 结构体，我们可以获取音视频流的各种信息，包括流的类型（是音频流还是视频流）、编码器的 ID、采样率、位率，以及其他和该流相关的详细信息。
//        AVStream 和 AVCodecParameters 之间的关系是，AVFormatContext 中的每个 AVStream 结构体都包含一个指向对应音视频流的 AVCodecParameters 结构体的指针。这种关系使得我们可以通过 AVFormatContext 对象访问音视频流的编解码器参数，或者将 AVCodecParameters 中的编解码器参数拷贝到 AVCodecContext 结构体中进行编解码操作。
//        理解 AVStream 结构体的作用和和它与 AVCodecParameters 的关系将有助于我们在使用 FFmpeg 时有效地检索和操作音视频流的编解码器参数。
        AVStream *stream = context->streams[streamIndex];

        AVCodecParameters* codecParameters =  stream->codecpar;
//        AVMEDIA_TYPE_VIDEO：视频流类型。
//        AVMEDIA_TYPE_AUDIO：音频流类型。
//        AVMEDIA_TYPE_SUBTITLE：字幕流类型。
//        AVMEDIA_TYPE_DATA：数据流类型。
//        AVMEDIA_TYPE_ATTACHMENT：附件流类型。
//        AVMEDIA_TYPE_NB：流类型的总数。
        enum AVMediaType streamType = codecParameters->codec_type;
        qDebug() << "Video codec_type" << streamType;

        if(streamType == AVMEDIA_TYPE_VIDEO) {
            int width = codecParameters->width;
            int height = codecParameters->height;
            qDebug() << "Video Resolution" << width << "x" << height;
        }
    }
};

void FFMpegDelegate::CloseContext() {
    avformat_close_input(&context);
}

FFMpegDelegate::~FFMpegDelegate() {
    delete context;
}

FFMpegDelegate::FFMpegDelegate() = default;
