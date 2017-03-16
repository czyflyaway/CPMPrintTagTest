#include <QDate>
#include <QPainter>
#include <QPageSize>
#include <QDebug>
#include "TagPrinter.h"

TagPrinter::TagPrinter()
{
    init();
}

void TagPrinter::init()
{
    m_Data = "";

    m_Printer.setPageSize(QPageSize(QSize(TagWidth, TagHeight), QPageSize::Millimeter));
    m_Printer.setOrientation(QPrinter::Portrait);

    /*设置字体*/
    m_NumberFont.setFamily(QString("Courier New"));
    m_NumberFont.setBold(true);
    m_NumberFont.setPointSize(12);

    m_tagSize = QSize(TagWidth * xScale, TagHeight * yScale);
}


/*打印条形码*/
void TagPrinter::printBarcode(QPainter &painter, const QString &barcode, QRect &barcodeRect)
{
    struct zint_symbol * my_symbol = ZBarcode_Create();

    my_symbol->symbology=BARCODE_CODE128;
    my_symbol->show_hrt = 0;

    ZBarcode_Encode(my_symbol, (unsigned char*)(barcode.toStdString().c_str()),0);

    ZBarcode_Print(my_symbol,0);
    ZBarcode_Delete(my_symbol);


    m_Img.load(my_symbol->outfile);

    painter.save();
    painter.drawPixmap(barcodeRect,
                       QPixmap::fromImage(m_Img),
                       m_Img.rect());
    painter.restore();
}

void TagPrinter::printPicture(QPainter &painter, const QString &PicFileName, QRect &PicRect)
{
    m_Img.load(PicFileName);

    painter.save();
    painter.drawImage(PicRect.x()+0.1*PicRect.width(), PicRect.y()+0.1*PicRect.height(),
                      m_Img.scaled(PicRect.width()*0.8, PicRect.height()*0.8, Qt::KeepAspectRatio));
    painter.restore();
}

void TagPrinter::calcBarCodeAndNumberRects(int face, int row)
{
    double fisrtTagLeft = oddFaceEdgeWidth * xScale;//第一个标签最左处
    switch (face % 2) {//可根据不同面打印不同间隔
    case 0://偶面
    {
        for(int i = 0; i < CELLCOUNT; ++i)
        {
            double x = fisrtTagLeft + (oddFaceInterval * xScale + m_tagSize.width()) * i  + m_tagSize.width() * 0.05;
            double yBarcode = (m_tagSize.height() + tagIntervalInHeight * yScale) * row + m_tagSize.height() * 0.05;
            m_barCodeRects[i] = QRect(x , yBarcode, m_tagSize.width() * 0.9, m_tagSize.height() * 0.6);
            double yNumber = (m_tagSize.height() + tagIntervalInHeight * yScale) * row + m_tagSize.height() * 0.65;
            m_numberRects[i] = QRect(x , yNumber, m_tagSize.width() * 0.9, m_tagSize.height() * 0.3);
        }
    }
        break;
    case 1://奇面
    {
        for(int i = 0; i < CELLCOUNT; ++i)
        {
            double x = fisrtTagLeft + (oddFaceInterval * xScale + m_tagSize.width()) * i  + m_tagSize.width() * 0.05;
            double yBarcode = (m_tagSize.height() + tagIntervalInHeight * yScale) * row + m_tagSize.height() * 0.05;
            m_barCodeRects[i] = QRect(x , yBarcode, m_tagSize.width() * 0.9, m_tagSize.height() * 0.6);
            double yNumber = (m_tagSize.height() + tagIntervalInHeight * yScale) * row + m_tagSize.height() * 0.65;
            m_numberRects[i] = QRect(x , yNumber, m_tagSize.width() * 0.9, m_tagSize.height() * 0.3);
        }
    }
        break;
    default:
        break;
    }

}

void TagPrinter::printTagOneRow(QPainter &painter, QPixmap& pix, QStringList tagNumbers, int nFace, int row)
{
     calcBarCodeAndNumberRects(nFace, row);
     for ( int i = 0 ; i < CELLCOUNT; ++i)
     {
         printBarcode(painter, tagNumbers.at(i), m_barCodeRects[i]);
         painter.setFont(m_NumberFont);
         painter.drawText(m_numberRects[i], Qt::AlignHCenter|Qt::AlignTop, tagNumbers.at(i));
     }
}

/*打印标签*/
void TagPrinter::printTag(QString *data)
{
    m_Data = *data;
    QPainter painter(&m_Printer);

    //打印货架条形码
    printBarcode(painter, m_Data, m_BarcodeRect);
    painter.save();
    painter.setFont(m_NumberFont);
    painter.drawText(m_NumberRect, Qt::AlignHCenter|Qt::AlignTop, m_Data);
    painter.restore();

}

void TagPrinter::printTags(QStringList tagNumbers, int nFace, int row)
{
    QPixmap pix(pageWidth * 10,pageHeight * 10);
    pix.fill(QColor(0,0,0,0));
    QPainter painter(&pix);
    if(nFace == 0 && row == 0)
    {
        /*打印一个货架*/
        for(int k = 0; k < FACECOUNT; ++k)
        {
            QStringList faceList;
            for(int n = 0; n < LEVELCOUNT * CELLCOUNT; ++n)//取出层*单元数Number
            {
                static int index = k * LEVELCOUNT * CELLCOUNT;
                faceList.append(tagNumbers.at(index));
                index ++;
            }
            for(int j = 0; j < LEVELCOUNT; ++j)
            {
                static int curRow = 0;
                QStringList rowList;
                static int index = j * CELLCOUNT;
                for(int  n= 0; n < CELLCOUNT; ++n)
                {
                    if(LEVELCOUNT * CELLCOUNT % index == 0)
                        index = 0;
                    rowList.append(faceList.at(index));
                    index ++;
                }
                printTagOneRow(painter, pix, rowList, k, curRow);//bug!!!...
                curRow++;
            }
        }

    }
    else if(nFace != 0 && row == 0)
    {
        /*打印一面*/
        for(int j = 0; j < LEVELCOUNT; ++j)
        {
            QStringList rowList;
            for(int  n= 0; n < CELLCOUNT; ++n)
            {
                static int index = j * CELLCOUNT;
                rowList.append(tagNumbers.at(index));
                index ++;

            }
            printTagOneRow(painter, pix, rowList, nFace, j);
        }
    }
    else if(nFace != 0 && row != 0)
    {
        /*打印一层*/
        printTagOneRow(painter, pix, tagNumbers, nFace, row);
    }

//    QPixmap pix(pageWidth * 10,pageHeight * 10);
//    pix.fill(QColor(0,0,0,0));
//    QPainter painter(&pix);
//    for ( int i = 0 ; i < CELLCOUNT; ++i)
//    {
//        printBarcode(painter, tagNumbers.at(i), m_barCodeRects[i]);
//        painter.setFont(m_NumberFont);
//        painter.drawText(m_numberRects[i], Qt::AlignHCenter|Qt::AlignTop, tagNumbers.at(i));
//    }
    painter.end();
    QString picName = QString("Holder") + tagNumbers.at(0).mid(2, 7) + QString("-Face") + QString::number(nFace) + QString("-Level") + QString::number(row) + QString(".png");
    pix.save(picName);

}
