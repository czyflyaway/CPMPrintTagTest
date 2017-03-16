#ifndef TAGPRINTER_H
#define TAGPRINTER_H

#include <QtPrintSupport/QPrinter>
#include <QObject>
#include <QFont>
#include <QRect>
#include <QImage>
#include <QPixmap>
#include "zint.h"

#define CELLCOUNT   12
#define LEVELCOUNT  3
#define FACECOUNT   4

const double InchScale      = 25.4;
const double TagWidth       = 50.00;
const double TagHeight      = 20.00;
const double OffsetWidth    = 1.00;
const double OffsetHeight   = 2.50;

const double oddFaceEdgeWidth = 12.50;//1,3面边缘空隙
const double evenFaceEdgeWidth = 15.00;//2,4面边缘空隙
const double oddFaceInterval = 25.00;//1,3面条码间隔
const double evenFaceInterval = 20.00;//2，4面条码间隔
const double oddFaceWidth = 900.00;//1,3面层宽度
const double evenFaceWidth = 870.00;//2,4面层宽度
const double pageWidth = 900.00;//打印纸张宽度
const double pageHeight = 200.00;//打印纸高度
const double tagIntervalInHeight = 5.00;//标签在垂直方向间隔

const double xScale = 10.00;//每mm像素数量
const double yScale = 10.00;

class TagPrinter : public QObject
{
    Q_OBJECT
public:
    TagPrinter();

protected:
    void run();

private:
    /*初始化*/
    void init();


    /*打印条形码*/
    void printBarcode(QPainter &painter, const QString &barcode, QRect &barcodeRect);

    /*打印图片*/
    void printPicture(QPainter &painter, const QString &PicFileName, QRect &PicRect);
    /*计算条形码(图形、编号)位置*/
    void calcBarCodeAndNumberRects(int face, int row);
    void printTagOneRow(QPainter &painter, QPixmap& pix, QStringList tagNumbers, int nFace = 0, int row = 0);

public slots:
    /*打印标签*/
    void printTag(QString *data);
    /*打印一层标签*/
    void printTags(QStringList tagNumbers, int nFace = 0, int row = 0);

private:
    QString m_Data;

    QRect   m_BarcodeRect;//条形码矩形
    QRect   m_NumberRect;//序号矩形

    QPrinter m_Printer;

    QFont   m_NumberFont;

    QSizeF  m_DocSize;
    double  m_XScale;
    double  m_YScale;
    QImage  m_Img;

    /*条形码图形与编号的位置存储*/
    QRect m_barCodeRects[CELLCOUNT];
    QRect m_numberRects[CELLCOUNT];
    /*单个标签的大小*/
    QSize m_tagSize;
};

#endif // TAGPRINTER_H
