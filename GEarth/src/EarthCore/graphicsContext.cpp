#include "EarthCore/graphicsContext.h"

namespace earth
{



    earth::CRefPtr<earth::CGraphicsContext::CWindowingSystemInterfaces>& CGraphicsContext::getWindowingSystemInterfaces()
    {
        return GraphicsContext::getWindowingSystemInterfaces();
    }

    earth::CGraphicsContext::CWindowingSystemInterfaces* CGraphicsContext::getWindowingSystemInterface(const std::string& name /*= ""*/)
    {
        return (CWindowingSystemInterfaces*)GraphicsContext::getWindowingSystemInterface(name);
    }

    earth::CGraphicsContext* CGraphicsContext::createGraphicsContext(CTraits* traits)
    {
        return (CGraphicsContext*)GraphicsContext::createGraphicsContext(traits);
    }

    unsigned int CGraphicsContext::createNewContextID()
    {
        return GraphicsContext::createNewContextID();
    }

    unsigned int CGraphicsContext::getMaxContextID()
    {
        return GraphicsContext::getMaxContextID();
    }

    void CGraphicsContext::incrementContextIDUsageCount(unsigned int contextID)
    {
        GraphicsContext::incrementContextIDUsageCount(contextID);
    }

    void CGraphicsContext::decrementContextIDUsageCount(unsigned int contextID)
    {
        GraphicsContext::decrementContextIDUsageCount(contextID);
    }

    /*earth::CGraphicsContext::CGraphicsContexts CGraphicsContext::getAllRegisteredGraphicsContexts()
    {
        return (CGraphicsContexts)GraphicsContext::getAllRegisteredGraphicsContexts();
    }

    earth::CGraphicsContext::CGraphicsContexts CGraphicsContext::getRegisteredGraphicsContexts(unsigned int contextID)
    {
        return GraphicsContext::getRegisteredGraphicsContexts(contextID);
    }*/

    void CGraphicsContext::setCompileContext(unsigned int contextID, CGraphicsContext* gc)
    {
        GraphicsContext::setCompileContext(contextID,gc);
    }

    earth::CGraphicsContext* CGraphicsContext::getOrCreateCompileContext(unsigned int contextID)
    {
        return (CGraphicsContext*)GraphicsContext::getOrCreateCompileContext(contextID);
    }

    earth::CGraphicsContext* CGraphicsContext::getCompileContext(unsigned int contextID)
    {
        return (CGraphicsContext*)GraphicsContext::getCompileContext(contextID);
    }

}

