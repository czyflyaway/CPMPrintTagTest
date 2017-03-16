#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_pTagPrinter;
}

void MainWindow::init()
{
    m_HolderType = QString("");
    m_TagNumber = QString("");
    m_pButtonGroup = new QButtonGroup;

    m_pTagPrinter = new TagPrinter;

    m_pButtonGroup->addButton(ui->SingleRadioButton, 0);
    m_pButtonGroup->addButton(ui->LevelRadioButton, 1);
    m_pButtonGroup->addButton(ui->FaceRadioButton, 2);
    m_pButtonGroup->addButton(ui->HolderRadioButton, 3);

    ui->SingleRadioButton->setChecked(true);

    connect(ui->SingleRadioButton, SIGNAL(clicked()), this, SLOT(updateView()));
    connect(ui->LevelRadioButton, SIGNAL(clicked()), this, SLOT(updateView()));
    connect(ui->FaceRadioButton, SIGNAL(clicked()), this, SLOT(updateView()));
    connect(ui->HolderRadioButton, SIGNAL(clicked()), this, SLOT(updateView()));

    connect(ui->PrintButton, SIGNAL(clicked()), this, SLOT(print()));

    connect(this, SIGNAL(printTag(QString*)), m_pTagPrinter, SLOT(printTag(QString*)));
    connect(this, SIGNAL(printTags(QStringList,int,int)), m_pTagPrinter, SLOT(printTags(QStringList,int,int)));
}

void MainWindow::updateView()
{
    switch (m_pButtonGroup->checkedId()) {
    case 0:
        ui->CellNumSpinBox->setVisible(true);
        ui->CellNumLabel->setVisible(true);
        ui->LevelNumSpinBox->setVisible(true);
        ui->LevelNumLabel->setVisible(true);
        ui->FaceNumSpinBox->setVisible(true);
        ui->FaceNumLabel->setVisible(true);
        break;
    case 1:
        ui->CellNumSpinBox->setVisible(false);
        ui->CellNumLabel->setVisible(false);
        ui->LevelNumSpinBox->setVisible(true);
        ui->LevelNumLabel->setVisible(true);
        ui->FaceNumSpinBox->setVisible(true);
        ui->FaceNumLabel->setVisible(true);
        break;
    case 2:
        ui->CellNumSpinBox->setVisible(false);
        ui->CellNumLabel->setVisible(false);
        ui->LevelNumSpinBox->setVisible(false);
        ui->LevelNumLabel->setVisible(false);
        ui->FaceNumSpinBox->setVisible(true);
        ui->FaceNumLabel->setVisible(true);
        break;
    case 3:
        ui->CellNumSpinBox->setVisible(false);
        ui->CellNumLabel->setVisible(false);
        ui->LevelNumSpinBox->setVisible(false);
        ui->LevelNumLabel->setVisible(false);
        ui->FaceNumSpinBox->setVisible(false);
        ui->FaceNumLabel->setVisible(false);
        break;
    default:
        break;
    }
}

void MainWindow::print()
{
    switch (ui->HolderStyleComboBox->currentIndex())
    {
    case 0:
        m_HolderType = QString("SH");
        break;
    case 1:
        m_HolderType = QString("HL");
        break;
    default:
        break;
    }

    switch (m_pButtonGroup->checkedId()) {
    case 0:
        m_TagNumber = m_HolderType + QString("%1%2%3%4")
                .arg(ui->HolderNumSpinBox->value(), 7, 10, QChar('0'))//7位货架号
                .arg(ui->FaceNumSpinBox->value())//1位面号
                .arg(ui->LevelNumSpinBox->value(), 2, 10, QChar('0'))//2位层号
                .arg(ui->CellNumSpinBox->value(), 2, 10, QChar('0'));//2位单元号

        emit printTag(&m_TagNumber);
        break;
    case 1://打印一层
    {
        for (int i = 0; i < CELLCOUNT ; ++i)
        {
            m_TagNumber = m_HolderType + QString("%1%2%3%4")
                    .arg(ui->HolderNumSpinBox->value(), 7, 10, QChar('0'))
                    .arg(ui->FaceNumSpinBox->value())
                    .arg(ui->LevelNumSpinBox->value(), 2, 10, QChar('0'))
                    .arg(i, 2, 10, QChar('0'));
            m_TagNumbers.append(m_TagNumber);

        }
         emit printTags(m_TagNumbers, ui->FaceNumSpinBox->value(), ui->LevelNumSpinBox->value());
    }
        break;
    case 2:
    {
        for (int j = 1; j <= LEVELCOUNT; ++ j)
        {
            for (int i = 1; i <= CELLCOUNT ; ++i)
            {
                m_TagNumber = m_HolderType + QString("%1%2%3%4")
                        .arg(ui->HolderNumSpinBox->value(), 7, 10, QChar('0'))
                        .arg(ui->FaceNumSpinBox->value())
                        .arg(j, 2, 10, QChar('0'))
                        .arg(i, 2, 10, QChar('0'));
                m_TagNumbers.append(m_TagNumber);
            }
        }
        emit printTags(m_TagNumbers, ui->FaceNumSpinBox->value(), 0);
    }
        break;
    case 3:
    {
        for(int k = 1; k <= FACECOUNT; ++ k)
        {
            for (int j = 1; j <= LEVELCOUNT; ++ j)
            {
                for (int i = 1; i <= CELLCOUNT ; ++i)
                {
                    m_TagNumber = m_HolderType + QString("%1%2%3%4")
                            .arg(ui->HolderNumSpinBox->value(), 7, 10, QChar('0'))
                            .arg(k)
                            .arg(j, 2, 10, QChar('0'))
                            .arg(i, 2, 10, QChar('0'));

                    m_TagNumbers.append(m_TagNumber);
                }
            }
        }
        emit printTags(m_TagNumbers, 0, 0);
    }
        break;
    default:
        break;
    }
}
