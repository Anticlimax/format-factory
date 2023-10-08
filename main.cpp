#include <QApplication>
#include "./src/MainWindow.h"
extern "C" {
#include <libavutil/log.h>
#include <libavcodec/avcodec.h>
}
void initFFmpeg() {
    av_log_set_level(AV_LOG_DEBUG);
    av_log(nullptr, AV_LOG_DEBUG, "hello ffmpeg\n");
}


 int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    //创建窗口
    MainWindow window;

    //显示窗口
    window.show();

    initFFmpeg();

    return QApplication::exec();
}

