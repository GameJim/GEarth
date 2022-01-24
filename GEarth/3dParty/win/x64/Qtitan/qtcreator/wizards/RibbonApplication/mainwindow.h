#ifndef %RibbonClassName:u%_%CppHeaderSuffix:u%
#define %RibbonClassName:u%_%CppHeaderSuffix:u%

#include <QtnRibbonMainWindow.h>

#include "ui_%RibbonClassName%.h"


/* %RibbonClassName% */
class %RibbonClassName% : public Qtitan::RibbonMainWindow
{
  Q_OBJECT
public:
    %RibbonClassName%(QWidget* parent = Q_NULL);
    virtual ~%RibbonClassName%();

private:
    Ui::%RibbonClassName% ui;
};

#endif // %RibbonClassName:u%_%CppHeaderSuffix:u%

