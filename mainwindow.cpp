#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "zlib.h"
#include <QtDebug>
QByteArray GzipCompress(QByteArray postBody)
{
    QByteArray outBuf;
    z_stream c_stream;
    int err = 0;
    int windowBits = 15;
    int GZIP_ENCODING = 16;
    if (!postBody.isEmpty())
    {
        c_stream.zalloc = (alloc_func)0;
        c_stream.zfree = (free_func)0;
        c_stream.opaque = (voidpf)0;
        c_stream.next_in = (Bytef *)postBody.data();
        c_stream.avail_in = postBody.size();
        if (deflateInit2(&c_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
            MAX_WBITS + GZIP_ENCODING, 8, Z_DEFAULT_STRATEGY) != Z_OK) return QByteArray();
        for (;;) {
            char destBuf[4096] = { 0 };
            c_stream.next_out = (Bytef *)destBuf;
            c_stream.avail_out = 4096;
            int err = deflate(&c_stream, Z_FINISH);
            outBuf.append(destBuf, 4096 - c_stream.avail_out);
            if (err == Z_STREAM_END || err != Z_OK)
            {
                break;
            }
        }
        auto total = c_stream.total_out;
        deflateEnd(&c_stream);
        total = c_stream.total_out;
    }
    return outBuf;
}


QByteArray GZipUnCompress(QByteArray src)
{
    QByteArray outBuffer;
    z_stream strm;
    strm.zalloc = NULL;
    strm.zfree = NULL;
    strm.opaque = NULL;

    strm.avail_in = src.size();
    strm.next_in = (Bytef *)src.data();

    int err = -1, ret = -1;
    err = inflateInit2(&strm, MAX_WBITS + 16);
    if (err == Z_OK) {
        while (true)
        {
            char buffer[4096] = { 0 };
            strm.avail_out = 4096;
            strm.next_out = (Bytef *)buffer;
            int code = inflate(&strm, Z_FINISH);
            outBuffer.append(buffer, 4096 - strm.avail_out);
           // qDebug()<<"ddd"<<code<<Z_OK;
            if (Z_STREAM_END == code )
            {
                break;
            }
        }
    }
    inflateEnd(&strm);
    return outBuffer;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString k = "1111111111111111111111111111111111111111111111111111111111";
    QByteArray a = GzipCompress(k.toUtf8());
    QByteArray b = GZipUnCompress(a);
    qDebug()<<"原大小"<<k.size()<<"压缩后大小"<<a.size()<<"解压大小"<<b.size();
    qDebug()<<k<<a<<b;

}

MainWindow::~MainWindow()
{
    delete ui;
}

