#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QtnRibbonGallery.h>
#include "ribbonwindow.h"

/* GalleriesDialog */
class GalleriesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GalleriesDialog(QWidget* parent = Q_NULL);
    virtual ~GalleriesDialog();
protected:
    RibbonGalleryGroup* m_itemsEffects;
    RibbonGallery* m_galleryEffects;
protected:
    virtual void resizeEvent(QResizeEvent* event);
private:
    Q_DISABLE_COPY(GalleriesDialog)
};


/* MainWindow */
class MainWindow : public RibbonWindow
{
  Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = Q_NULL);
    virtual ~MainWindow();

private:
    void createRibbon();
    void createStatusBar();
    void createGalleryItems();

protected Q_SLOTS:
    void dialogGallery();
    void aboutToHideUndo();
    void itemColorPressed(RibbonGalleryItem* item);
    void itemStyleFontPressed(RibbonGalleryItem* item);
    void itemTablePressed(RibbonGalleryItem* item);
    void itemUndoPressed(RibbonGalleryItem* item);
private:
    virtual void updateActionsTheme();
    void updateImageStyles();

private:
    QAction* m_actionUndo;
    RibbonGallery* m_widgetFontTextColor;
    RibbonGallery* m_widgetStyleFont;
    RibbonGallery* m_popupStyleFont;
    RibbonGallery* m_popupTableGallery;
    RibbonGallery* m_popupUndoGallery;
    RibbonGallery* m_popupEffects;
    RibbonGallery* m_popupThemes;

    RibbonGalleryGroup* m_itemsFontTextColor;
    RibbonGalleryGroup* m_itemsStyles;
    RibbonGalleryGroup* m_itemsTable;
    RibbonGalleryGroup* m_itemsUndo;
    RibbonGalleryGroup* m_itemsShapes;
    RibbonGalleryGroup* m_itemsEffects;
    RibbonGalleryGroup* m_itemsThemes;
private:
    Q_DISABLE_COPY(MainWindow)
};

#endif // MAINWINDOW_H
