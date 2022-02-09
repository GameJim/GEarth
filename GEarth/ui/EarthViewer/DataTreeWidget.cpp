#pragma execution_character_set("utf-8") 
#include "DataTreeWidget.h"

#include <QDirModel>
#include <QTreeView>
#include <QHeaderView>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QPushButton>

CDataTreeWidget::CDataTreeWidget(QWidget * parent /*= Q_NULLPTR*/)
    :QWidget(parent)
{

}


CDataTreeWidget::~CDataTreeWidget()
{

}



CDirTreeWidget::CDirTreeWidget(QWidget * parent /*= Q_NULLPTR*/)
{

    //工具蓝
    QDialogButtonBox* buttonBox = new QDialogButtonBox(Qt::Horizontal);
    QPushButton* diskButton = buttonBox->addButton(
        tr("磁盘"), QDialogButtonBox::ActionRole);
    connect(diskButton, SIGNAL(clicked(bool)), this, SLOT(ShowDiskTree()));

    QPushButton* dbButton = buttonBox->addButton(
        tr("数据库"), QDialogButtonBox::ActionRole);
    connect(dbButton, SIGNAL(clicked(bool)), this, SLOT(ShowDBTree()));

    QVBoxLayout *mainLayout = new QVBoxLayout();
    //目录
    m_pViewArea = new QStackedWidget();
    //增添窗口
    {
        QWidget* pWidget = new QWidget();
        QDirModel* model = new QDirModel;
        model->setReadOnly(false);
        model->setSorting(QDir::DirsFirst | QDir::IgnoreCase | QDir::Name);

        QTreeView* treeView = new QTreeView;
        treeView->setModel(model);
        treeView->header()->setStretchLastSection(true);
        treeView->header()->setSortIndicator(0, Qt::AscendingOrder);
        treeView->header()->setSortIndicatorShown(true);

        QModelIndex index = model->index(QDir::currentPath());
        treeView->expand(index);
        treeView->scrollTo(index);
        treeView->resizeColumnToContents(0);

        //设置布局
        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addWidget(treeView);
        pWidget->setLayout(mainLayout);

        m_pViewArea->addWidget(pWidget);
    }


    //增添窗口
    {
        QWidget* pWidget = new QWidget();
        
        //设置布局
        QVBoxLayout *mainLayout = new QVBoxLayout;
        pWidget->setLayout(mainLayout);
        m_pViewArea->addWidget(pWidget);
    }


    this->ShowDiskTree();


    mainLayout->addWidget(buttonBox);
    mainLayout->addWidget(m_pViewArea);

    setLayout(mainLayout);
}

void CDirTreeWidget::ShowDiskTree()
{

    m_pViewArea->setCurrentIndex(0);
}

void CDirTreeWidget::ShowDBTree() 
{
    m_pViewArea->setCurrentIndex(1);
}
