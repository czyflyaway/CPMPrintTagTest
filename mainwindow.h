#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include "tagprinter.h"

#define CELLCOUNT   12
#define LEVELCOUNT  3
#define FACECOUNT   4

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void init();

signals:
    void printTag(QString *data);
    void printTags(QStringList tagNumbers, int face, int row);//打印一层条码
public slots:
    void updateView();
    void print();

private:
    Ui::MainWindow *ui;
    QButtonGroup *m_pButtonGroup;
    QString m_TagNumber, m_HolderType;
    QStringList m_TagNumbers;
    TagPrinter *m_pTagPrinter;
};

#endif // MAINWINDOW_H
