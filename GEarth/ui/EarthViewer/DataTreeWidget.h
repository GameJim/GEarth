/*!
*@File    DataTreeWidget.h
*@Brief   文件描述:下次一定
*@Date    2022/2/9
*/
#pragma once

#include <QWidget>
#include <QTreeView>
#include <QStackedWidget>
#include <QAbstractItemModel>

#include "EarthCore/mapManager.h"


//文件目录类
class CDirTreeWidget : public QWidget {
    Q_OBJECT

public:
    CDirTreeWidget(QWidget * parent = Q_NULLPTR);
    ~CDirTreeWidget() = default;

    protected slots:
    void ShowDiskTree();
    void ShowDBTree();
private:
    QStackedWidget* m_pViewArea;   //用于显示的区域

};



class AbstractTreeItem 
{
protected:
    AbstractTreeItem(AbstractTreeItem* pParent, QAbstractItemModel* pModel = nullptr) :m_pParent(pParent), m_pModel(pModel){};
public:
    virtual ~AbstractTreeItem() = default; 
    virtual AbstractTreeItem* parent() { return m_pParent; };
    virtual AbstractTreeItem* child(const unsigned& index) { 
        if (index < m_pChildren.size())
            return m_pChildren[index].get();
        return nullptr;
        ; 
    };
    virtual void addChild(std::shared_ptr<AbstractTreeItem> child) {
        m_pChildren.push_back(child);
        child->m_pParent = this;
        child->m_pModel = this->m_pModel;
        if (m_pModel)
        {
            m_pModel->layoutChanged();
            child->SetModel(m_pModel);
        }

        
       
    };

    virtual void removeChild(const unsigned& index)
    {
        m_pChildren.erase(m_pChildren.begin() + index);
        if (m_pModel)
        {
            m_pModel->layoutChanged();
        }
    };

    virtual int columnCount() { return 1; };
    virtual int rowCount() { return m_pChildren.size(); };
    //virtual void removeChild(const unsigned & index) { m_pChildren.erase(m_pChildren.begin() + index); }


    virtual QVariant data(const unsigned& index) = 0;
    virtual bool setData(const QVariant &value, int role /*= Qt::EditRole*/) { return true; };
    QAbstractItemModel* getModel() { return m_pModel; };
protected:
    void SetModel(QAbstractItemModel* pModel)
    {
        this->m_pModel = pModel;
        for (auto pChild: m_pChildren)
        {
            pChild->SetModel(pModel);
        }
    }

    std::vector<std::shared_ptr<AbstractTreeItem>> m_pChildren;
    AbstractTreeItem* m_pParent;

    //位于那一个model
    QAbstractItemModel* m_pModel;
 
};


class MapTreeItem : public AbstractTreeItem
{
public:
    enum EnObjectType
    {
        OBJECT_TYPE_ROOT_NODE = 0,
        OBJECT_TYPE_MAP_NODE,
        OBJECT_TYPE_LAYER
    };

    MapTreeItem(earth::CMapManager*  pMapManager, QAbstractItemModel* pModel);
    MapTreeItem(earth::CMap*  pMapManager);
    MapTreeItem(const EnObjectType& enType, void* pObject, AbstractTreeItem* pParent = nullptr);

    virtual ~MapTreeItem() = default;
  
    virtual QVariant data(const unsigned& index);
    virtual bool setData(const QVariant &value, int role /*= Qt::EditRole*/);
    void* GetObject() { return m_pObject; };

protected:
    EnObjectType m_enObjectType;      //数据类型
    void* m_pObject;   //数据对象

};


class MapTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    MapTreeModel(earth::CMapManager& pMapManager);
    virtual ~MapTreeModel();
    QVariant data(const QModelIndex& index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

    QModelIndex index(int row, int column,
        const QModelIndex &parent = QModelIndex())  const Q_DECL_OVERRIDE;
     QModelIndex parent(const QModelIndex &child) const Q_DECL_OVERRIDE;

    int rowCount(const QModelIndex &parent = QModelIndex())  const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex())  const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

    void dataChanged();

protected:
    std::shared_ptr<AbstractTreeItem> m_pRoot;
    earth::CRefPtr<earth::CMapManagerCallback> m_pCallBack; //回调函数
};


class CDataTreeWidget : public QWidget {
    Q_OBJECT

public:
    CDataTreeWidget(earth::CMapManager& pMapManager,QWidget * parent = Q_NULLPTR);
    ~CDataTreeWidget();
private:
   

    earth::CMapManager& m_pMapManager;   //所有数据的根节点
    QStackedWidget* m_pViewArea;   //用于显示的区域
};



