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
    int result = avformat_open_input(&inputContext, urlPtr, nullptr, nullptr);
    if(result != 0){
        qDebug() << "Failed to open input file";
    }
//    avformat_find_stream_info是FFmpeg库中的一个函数，其作用是读取音频/视频文件的流信息，并将其存储在AVFormatContext对象中。
//    当打开一个音频/视频文件后，文件中可能包含多个音频流、视频流或其他类型的流。avformat_find_stream_info函数会遍历文件中的所有流，并从中提取流的相关信息，例如文件格式、编码器、流参数等。这些信息可以帮助后续的音频/视频处理过程。
//    调用avformat_find_stream_info函数可以确保AVFormatContext对象中的流信息已被完整地填充。此函数在解码之前被调用，以便准备好音频/视频数据的解码过程。可以通过访问AVFormatContext对象来获取有关流的详细信息，例如时长、帧率、分辨率等。
//    需要注意的是，该函数可能需要一段时间才能执行完毕，因为它需要遍历整个文件来获取流的信息。因此，在调用该函数时，可能需要进行适当的等待，直到所有流的信息都被获取和填充到AVFormatContext对象中。
    result = avformat_find_stream_info(inputContext, nullptr);
    if(result < 0){
        qDebug() << "Failed to find stream information";
        CloseContext();
    }
}

void FFMpegDelegate::LogVideoResolution() {
    int streamIndex;
//        先使用formatContext->nb_streams获取流的数量，遍历其中的所有流，找到视频流，提取它的宽高
    for (streamIndex = 0; streamIndex < inputContext->nb_streams; ++streamIndex) {
//        AVStream 是 FFmpeg 中的一个结构体，它用于表示音视频文件或输入流中的一个音视频流。AVStream 包含了音视频流的各种信息，例如流的索引、时长、帧率、码率、解码器参数等。
//        在 AVFormatContext 中，每个音视频流对应着一个 AVStream 结构体。AVStream 结构体中包含了一个指向 AVCodecParameters 结构体的指针，该指针用于存储该音视频流的编解码器参数。
//        通过 AVStream 结构体，我们可以获取音视频流的各种信息，包括流的类型（是音频流还是视频流）、编码器的 ID、采样率、位率，以及其他和该流相关的详细信息。
//        AVStream 和 AVCodecParameters 之间的关系是，AVFormatContext 中的每个 AVStream 结构体都包含一个指向对应音视频流的 AVCodecParameters 结构体的指针。这种关系使得我们可以通过 AVFormatContext 对象访问音视频流的编解码器参数，或者将 AVCodecParameters 中的编解码器参数拷贝到 AVCodecContext 结构体中进行编解码操作。
//        理解 AVStream 结构体的作用和和它与 AVCodecParameters 的关系将有助于我们在使用 FFmpeg 时有效地检索和操作音视频流的编解码器参数。
        AVStream *stream = inputContext->streams[streamIndex];

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
    avformat_close_input(&inputContext);
}

int FFMpegDelegate::TransContainer() {
    const char *outputFile = "./file/output.m3u8";
//    avformat_alloc_output_context2 是 FFmpeg 提供的一个函数，用于创建输出格式上下文。在进行视频转封装时，我们需要创建一个输出文件和输出格式上下文来存储转封装后的视频流。avformat_alloc_output_context2 函数会根据输出文件的格式指定一个合适的输出格式上下文，并初始化该上下文的一些默认值。
//    AVFormatContext *avformat_alloc_output_context2(AVFormatContext **avctx, AVOutputFormat *oformat, const char *format_name, const char *filename);
//    参数说明：
//    avctx：输出格式上下文的指针的地址，创建成功后会将输出格式上下文的指针赋值给它。可以通过该指针来访问输出格式上下文的各种属性。
//    oformat：输出文件的格式。如果设置为 NULL，FFmpeg 会根据输出文件的扩展名猜测合适的格式。
//    format_name：输出文件的格式名称，可以是 NULL。
//    filename：输出文件的路径。
//    函数返回一个 AVFormatContext 结构体的指针，表示创建的输出格式上下文。返回值为 NULL 表示创建失败。
//
//    需要注意的是，avformat_alloc_output_context2 函数会为输出格式上下文自动分配内存，在使用完毕后需要使用 avformat_free_context 函数释放内存。

    if(avformat_alloc_output_context2(&outputContext, nullptr, "hls", outputFile) < 0){
        qDebug() << "无法创建输出格式化上下文";
        return -1;
    }

    for (int i = 0; i < inputContext->nb_streams; i++) {
        AVStream *inputStream = inputContext->streams[i];
//        avformat_new_stream的作用是在输出文件的封装格式中创建一个新的流，并将该流与输出文件的格式上下文（AVFormatContext）相关联。在音视频编码中，一个多媒体文件通常由多个音视频流组成，每个流包含一个音频或视频元素。avformat_new_stream可以用于创建新的音视频流，并将其添加到输出文件中。
//        使用avformat_new_stream，可以为新创建的流设置一些初始参数，如编码器ID、编码器上下文（AVCodecContext）、时间基等。然后可以通过avcodec_parameters_from_context函数将编码器上下文的参数复制到流的编码参数（AVCodecParameters）中。
        AVStream *outputStream = avformat_new_stream(outputContext, nullptr);
//        avcodec_parameters_copy是一个函数，其作用是复制一个AVCodecParameters结构体的内容。
//        AVCodecParameters是一个用于存储编解码器参数的结构体，包含了多个字段，如编解码器标识符、视频宽度和高度、比特率、帧率等。通过调用avcodec_parameters_copy函数，可以将一个AVCodecParameters结构体的内容复制到另一个AVCodecParameters结构体，从而实现参数的复制。
//        这个函数的具体语法如下：
//        int avcodec_parameters_copy (AVCodecParameters *dst, const AVCodecParameters *src)
//        参数解释：
//        dst：目标AVCodecParameters结构体，用于存储复制的数据。
//        src：源AVCodecParameters结构体，需要将其内容复制到目标结构体中。
//        返回值：
//        函数成功时返回0，否则返回一个负数，表示复制过程中发生了错误。
//        总之，avcodec_parameters_copy函数提供了一种方便的方式来复制AVCodecParameters结构体，以便在不同的编解码器之间传递参数。
        if(avcodec_parameters_copy(outputStream->codecpar, inputStream->codecpar) < 0) {
            qDebug() << "无法复制流信息";
            return -1;
        }
        qDebug() << outputStream->codecpar->codec_type << "输出流code_type";

    }
//    使用avio_open，可以打开不同的输入输出源，并得到对应的AVIOContext对象。AVIOContext是FFmpeg中用于管理输入输出的结构体，它封装了输入输出的一些参数和操作函数。
//    常见的使用场景包括：
//    打开本地媒体文件：可以通过设置参数url为本地媒体文件的路径来打开媒体文件，并得到对应的AVIOContext对象。
//    打开网络流：可以通过设置参数url为网络流的URL来打开网络流，并得到对应的AVIOContext对象。可以通过设置一些网络相关的选项来进行网络流传输的配置。
//    打开内存缓冲区：可以通过设置参数url为一个指向内存缓冲区的指针来打开内存缓冲区，并得到对应的AVIOContext对象。
    if(avio_open(&outputContext->pb, outputFile, AVIO_FLAG_WRITE)< 0) {
        qDebug() << "无法打开输出文件";
        return -1;
    }
    // 初始化outputContext的头部信息为默认值
    if(avformat_write_header(outputContext, nullptr) < 0){
        qDebug() << "无法写入输出文件的头部信息";
        return -1;
    }
    return 0;
}

FFMpegDelegate::~FFMpegDelegate() {
    delete inputContext;
}

FFMpegDelegate::FFMpegDelegate() = default;
