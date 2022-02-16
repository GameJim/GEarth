#pragma execution_character_set("utf-8") 
#include "DataTreeWidget.h"

#include <QDirModel>
#include <QTreeView>
#include <QHeaderView>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStyleFactory>
#include <assert.h>

#include "EarthCore/map.h"


CDataTreeWidget::CDataTreeWidget(earth::CMapManager& pMapManager, QWidget * parent /*= Q_NULLPTR*/)
    :QWidget(parent), m_pMapManager(pMapManager)
{
    

    QDialogButtonBox* buttonBox = new QDialogButtonBox(Qt::Horizontal);
    QPushButton* diskButton = buttonBox->addButton(
        tr("折叠"), QDialogButtonBox::ActionRole);
    //connect(diskButton, SIGNAL(clicked(bool)), this, SLOT(ShowDiskTree()));

    QPushButton* dbButton = buttonBox->addButton(
        tr("展开"), QDialogButtonBox::ActionRole);
    //connect(dbButton, SIGNAL(clicked(bool)), this, SLOT(ShowDBTree()));

    QVBoxLayout *mainLayout = new QVBoxLayout();
    //目录
    m_pViewArea = new QStackedWidget();
    //增添窗口
    {
        QWidget* pWidget = new QWidget();
        QTreeView* treeView = new QTreeView;
        
        QVBoxLayout *mainLayout = new QVBoxLayout();
       
        pWidget->setLayout(mainLayout);
        m_pViewArea->addWidget(pWidget);
        mainLayout->addWidget(treeView);

        MapTreeModel* model = new MapTreeModel(m_pMapManager);
        treeView->setModel(model);
        treeView->resizeColumnToContents(0);
        treeView->setStyle(QStyleFactory::create("windows"));
        treeView->expand(QModelIndex());
        treeView->setHeaderHidden(true);

        //设置布局
    }

    //增添窗口
    {
        QWidget* pWidget = new QWidget();

        //设置布局
        QVBoxLayout *mainLayout = new QVBoxLayout;
        pWidget->setLayout(mainLayout);
        m_pViewArea->addWidget(pWidget);
    }

    mainLayout->addWidget(buttonBox);
    mainLayout->addWidget(m_pViewArea);

    setLayout(mainLayout);
    m_pViewArea->setCurrentIndex(0);

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

class DataTreeMapChanged : public earth::CMapManagerCallback
{
public:
    virtual void Callback(const earth::CMapManagerCallback::EnCallbackType& eType, earth::CRefPtr<earth::CMap> pMap, const unsigned& index) override
    {
        switch (eType)
        {
        case earth::CMapManagerCallback::EnCallbackType::MAP_ADDED:
        {
            m_pRoot->addChild(MapTreeItem::CreateItem(pMap));
        }
        break;
        case earth::CMapManagerCallback::EnCallbackType::MAP_REMOVE:
        {
            m_pRoot->removeChild(index);
        }
        default:
            break;
        }

        m_pModel->dataChanged();
    };

    DataTreeMapChanged(std::shared_ptr<AbstractTreeItem> pItme, MapTreeModel* pModel) :m_pRoot(pItme), m_pModel(pModel){};
public:
    std::shared_ptr<AbstractTreeItem> m_pRoot;
    MapTreeModel* m_pModel;
};



MapTreeModel::MapTreeModel(earth::CMapManager& pMapManager)
{
    m_pRoot = MapTreeItem::CreateRoot(&pMapManager);
   

   
    m_pCallBack = new DataTreeMapChanged(m_pRoot,this);
    pMapManager.AddCallBack(m_pCallBack);
}

MapTreeModel::~MapTreeModel()
{
    MapTreeItem* pMapTreeItem = dynamic_cast<MapTreeItem*>(m_pRoot.get());
    if (pMapTreeItem)
    {
        static_cast<earth::CMapManager*>(pMapTreeItem->GetObject())->RemoveCallBack(m_pCallBack);
    }
}


QVariant MapTreeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    //图标
    AbstractTreeItem* item = static_cast<AbstractTreeItem*>(index.internalPointer());
    return item->data(role);

    //图标
}

bool MapTreeModel::setData(const QModelIndex &index, const QVariant &value, int role /*= Qt::EditRole*/)
{
    if (!index.isValid())
        return false;

    AbstractTreeItem* item = static_cast<AbstractTreeItem*>(index.internalPointer());
    return item->setData(value, role);
}

QModelIndex MapTreeModel::index(int row, int column, const QModelIndex &parent /*= QModelIndex()*/) const
{
    AbstractTreeItem* item = nullptr;
    if (parent.isValid())
    {
       item = static_cast<AbstractTreeItem*>(parent.internalPointer());
     
    }
    else
    {
        item = m_pRoot.get();
    }

    auto child = item->child(row);
    if (child)
    {
        return createIndex(row, column, child);
    }
    else
        return QModelIndex();

}

QModelIndex MapTreeModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();
   
    AbstractTreeItem* item = static_cast<AbstractTreeItem*>(child.internalPointer());
   

    if (item->parent() == m_pRoot.get())
    {
        return QModelIndex();
    }
   
    return createIndex(item->parent()->rowCount(), 0, item->parent());
}

int MapTreeModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    if (!parent.isValid())
        return m_pRoot->rowCount();

    AbstractTreeItem* item = static_cast<AbstractTreeItem*>(parent.internalPointer());
    return item->rowCount();

}

int MapTreeModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    if (!parent.isValid())
        return m_pRoot->columnCount();

    AbstractTreeItem* item = static_cast<AbstractTreeItem*>(parent.internalPointer());
    return item->columnCount();
}



Qt::ItemFlags MapTreeModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags theFlags = QAbstractItemModel::flags(index);
    if (index.isValid()) {
        theFlags |= Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
    }
    return theFlags;
}

void MapTreeModel::dataChanged()
{
    //emit updateCount(QModelIndex());
}

std::shared_ptr<AbstractTreeItem> MapTreeItem::CreateRoot(earth::CMapManager* pGroupNode)
{
    std::shared_ptr<AbstractTreeItem> pRootItem = std::make_shared<MapTreeItem>(OBJECT_TYPE_ROOT_NODE, pGroupNode);
    unsigned nMap = pGroupNode->GetMapNum();
    for (unsigned i = 0;i < nMap;++i)
    {
        earth::CMap* pMap = dynamic_cast<earth::CMap*>(pGroupNode->GetMap(i).get());
        std::shared_ptr<AbstractTreeItem> pMapItem = std::make_shared<MapTreeItem>(OBJECT_TYPE_MAP_NODE, pMap, pRootItem.get());
        pRootItem->addChild(pMapItem);

    
        unsigned nLayer = pMap->getNumLayers();
        for (unsigned j = 0; j < nLayer; ++j)
        {
            std::shared_ptr<AbstractTreeItem> pLayerItem = std::make_shared<MapTreeItem>(OBJECT_TYPE_LAYER, pMap->getLayerAt(j), pMapItem.get());
            pMapItem->addChild(pLayerItem);
        }
    }


    return pRootItem;
}

std::shared_ptr<AbstractTreeItem> MapTreeItem::CreateItem(earth::CMap* pMap)
{
    std::shared_ptr<AbstractTreeItem> pMapItem = std::make_shared<MapTreeItem>(OBJECT_TYPE_MAP_NODE, pMap, nullptr);

    unsigned nLayer = pMap->getNumLayers();
    for (unsigned j = 0; j < nLayer; ++j)
    {
        std::shared_ptr<AbstractTreeItem> pLayerItem = std::make_shared<MapTreeItem>(OBJECT_TYPE_LAYER, pMap->getLayerAt(j), pMapItem.get());
        pMapItem->addChild(pLayerItem);
    }

    return pMapItem;
}

MapTreeItem::MapTreeItem(const EnObjectType& enType, void* pObject, AbstractTreeItem* pParent)
    :AbstractTreeItem(pParent),m_pObject(pObject),m_enObjectType(enType)
{
  
}


QVariant MapTreeItem::data(const unsigned& role)
{
    switch (role)
    {
    case Qt::DecorationRole:
    {
        //图标
        switch (m_enObjectType)
        {
        case MapTreeItem::OBJECT_TYPE_ROOT_NODE:
            break;
        case MapTreeItem::OBJECT_TYPE_MAP_NODE:
            break;
        case MapTreeItem::OBJECT_TYPE_LAYER:
            break;
        default:
            break;
        }
        return QIcon("./res/ui/EarthViewer/OpenFile.png");
    };
    break;
    case Qt::DisplayRole:
    {
        //图标
        switch (m_enObjectType)
        {
        case MapTreeItem::OBJECT_TYPE_ROOT_NODE:
        {
            return "";
        }
        break;
        case MapTreeItem::OBJECT_TYPE_MAP_NODE:
        {
            earth::CMap* pMap = static_cast<earth::CMap*>(m_pObject);
            return QString::fromStdString(pMap->getMapName());
        }
        break;
        case MapTreeItem::OBJECT_TYPE_LAYER:
        {
            earth::CLayer* pLayer = static_cast<earth::CLayer*>(m_pObject);
            return QString::fromStdString(pLayer->getName());
        }
        break;
        default:
            break;
        }
    }
    break;
    case Qt::CheckStateRole:
    {
        //图标
        switch (m_enObjectType)
        {
        case MapTreeItem::OBJECT_TYPE_ROOT_NODE:
        {
            return true;
        }
        break;
        case MapTreeItem::OBJECT_TYPE_MAP_NODE:
        {
            return true;
        }
        break;
        case MapTreeItem::OBJECT_TYPE_LAYER:
        {
            earth::CLayer* pLayer = static_cast<earth::CLayer*>(m_pObject);
            return pLayer->getEnabled();
        }
        break;
        default:
            break;
        }
    }
    default:
        break;
    }

    return QVariant();
}



bool MapTreeItem::setData(const QVariant &value, int role /*= Qt::EditRole*/)
{
    switch (role)
    {
    break;
    case Qt::DisplayRole:
    {
        //图标
        switch (m_enObjectType)
        {
        case MapTreeItem::OBJECT_TYPE_ROOT_NODE:
        {
            return "";
        }
        break;
        case MapTreeItem::OBJECT_TYPE_MAP_NODE:
        {
            earth::CMap* pMap = static_cast<earth::CMap*>(m_pObject);
            pMap->setMapName(value.toString().toStdString());
        }
        break;
        case MapTreeItem::OBJECT_TYPE_LAYER:
        {
            earth::CLayer* pLayer = static_cast<earth::CLayer*>(m_pObject);
            pLayer->setName(value.toString().toStdString());
        }
        break;
        default:
            break;
        }

     
    }
    break;
    case Qt::CheckStateRole:
    {
        //图标
        switch (m_enObjectType)
        {
        case MapTreeItem::OBJECT_TYPE_ROOT_NODE:
        {
            return false;
        }
        break;
        case MapTreeItem::OBJECT_TYPE_MAP_NODE:
        {
            return false;
        }
        break;
        case MapTreeItem::OBJECT_TYPE_LAYER:
        {
            earth::CLayer* pLayer = static_cast<earth::CLayer*>(m_pObject);
            bool isEnable = !pLayer->getEnabled();
            pLayer->setEnabled(!pLayer->getEnabled());
        }
        break;
        default:
            break;
        }
    }
    default:
        break;
    }

    return true;
}

