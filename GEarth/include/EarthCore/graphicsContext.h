/*!
*@File    GraphicsContext.h
*@Brief   文件描述:下次一定
*@Date    2022/1/25
*/
#pragma once

#include "common/apiDescription.h"
#include "osg/GraphicsContext"
#include "EarthCore/type.h"
namespace earth
{
   

    class CORE_PUBLIC CGraphicsContext : public osg::GraphicsContext
    {
    public:
        using CWindowingSystemInterfaces = osg::GraphicsContext::WindowingSystemInterfaces;
        using CTraits = osg::GraphicsContext::Traits;

        CGraphicsContext() = default;
        ~CGraphicsContext() = default;

        static CRefPtr<CWindowingSystemInterfaces>& getWindowingSystemInterfaces();

       
        static CWindowingSystemInterfaces* getWindowingSystemInterface(const std::string& name = "");

       
        static CGraphicsContext* createGraphicsContext(CTraits* traits);

      
        static unsigned int createNewContextID();

        
        static unsigned int getMaxContextID();

    
        static void incrementContextIDUsageCount(unsigned int contextID);

      
        static void decrementContextIDUsageCount(unsigned int contextID);

        typedef std::vector<CGraphicsContext*> CGraphicsContexts;

       
   /*     static CGraphicsContexts getAllRegisteredGraphicsContexts();

      
        static CGraphicsContexts getRegisteredGraphicsContexts(unsigned int contextID);*/

       
        static void setCompileContext(unsigned int contextID, CGraphicsContext* gc);

     
        static  CGraphicsContext* getOrCreateCompileContext(unsigned int contextID);

      
        static CGraphicsContext* getCompileContext(unsigned int contextID);

    };
}



