#include "EarthCore/View.h"
#if WIN32
#include "osgViewer/api/Win32/GraphicsWindowWin32"
#endif
#include "EarthCore/camera.h"
#include "EarthCore/graphicsContext.h"
#include "EarthCore/EarthManipulator.h"
namespace earth
{
   
    CView::CView() {};
   

    CView::CView(void* hwnd)
    {
        InitWindow(hwnd);
    };

    void CView::InitWindow(void* hwnd)
    {
#if WIN32
        RECT rect;
        ::GetWindowRect((HWND)hwnd, &rect);

        CRefPtr<CReferenced> windata = new osgViewer::GraphicsWindowWin32::WindowData((HWND)hwnd);
        CRefPtr<CGraphicsContext::CTraits> traits = new CGraphicsContext::CTraits();
        traits->x = rect.left;
        traits->y = rect.top;
        traits->width = rect.right - rect.left;
        traits->height = rect.bottom - rect.top;
        traits->windowDecoration = false;
        traits->doubleBuffer = true;
        traits->sharedContext = 0;
        traits->inheritedWindowData = windata;

        //显示设置
        CDisplaySettings* ds = CDisplaySettings::instance().get();
        traits->alpha = ds->getMinimumNumAlphaBits();
        traits->stencil = ds->getMinimumNumStencilBits();
        traits->sampleBuffers = ds->getMultiSamples();
        traits->samples = ds->getNumMultiSamples();
        traits->vsync = true;

        CGraphicsContext* gc = CGraphicsContext::createGraphicsContext(traits);

        CRefPtr<CCamera> camera = new CCamera();
        camera->setGraphicsContext(gc);
        camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));


        //设置相机
        this->setCamera(camera);
#endif
    }

}

