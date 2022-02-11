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

#include "EarthCore/type.h"


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
    AbstractTreeItem(AbstractTreeItem* pParent) :m_pParent(pParent){};
public:
    virtual ~AbstractTreeItem() = default; 
    virtual AbstractTreeItem* parent() { return m_pParent; };
    virtual AbstractTreeItem* child(const unsigned& index) { 
        if (index < m_pChildren.size())
            return m_pChildren[index].get();
        return nullptr;
        ; 
    };
    virtual void addChild(std::shared_ptr<AbstractTreeItem> child) {m_pChildren.push_back(child);};
    virtual int columnCount() { return 1; };
    virtual int rowCount() { return m_pChildren.size(); };
  
    virtual QVariant data(const unsigned& index) = 0;
    virtual bool setData(const QVariant &value, int role /*= Qt::EditRole*/) { return true; };

protected:
    std::vector<std::shared_ptr<AbstractTreeItem>> m_pChildren;
    AbstractTreeItem* m_pParent;
 
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

    static std::shared_ptr<AbstractTreeItem> CreateRoot(earth::CGroupNode* pGroupNode);


    MapTreeItem(const EnObjectType& enType, earth::CObject* pObject, AbstractTreeItem* pParent = nullptr);
    virtual ~MapTreeItem() = default;
  
    virtual QVariant data(const unsigned& index);
    virtual bool setData(const QVariant &value, int role /*= Qt::EditRole*/);
protected:
    EnObjectType m_enObjectType;      //数据类型
    earth::CObject* m_pObject;   //数据对象
};


class MapTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    MapTreeModel(earth::CRefPtr<earth::CGroupNode> pGroup);
    QVariant data(const QModelIndex& index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

    QModelIndex index(int row, int column,
        const QModelIndex &parent = QModelIndex())  const Q_DECL_OVERRIDE;
     QModelIndex parent(const QModelIndex &child) const Q_DECL_OVERRIDE;

    int rowCount(const QModelIndex &parent = QModelIndex())  const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex())  const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
protected:
    std::shared_ptr<AbstractTreeItem> m_pRoot;
};


class CDataTreeWidget : public QWidget {
    Q_OBJECT

public:
    CDataTreeWidget(earth::CRefPtr<earth::CGroupNode>,QWidget * parent = Q_NULLPTR);
    ~CDataTreeWidget();
private:
    earth::CRefPtr<earth::CGroupNode>  m_pGroupNode;   //所有数据的根节点
    QStackedWidget* m_pViewArea;   //用于显示的区域
};



