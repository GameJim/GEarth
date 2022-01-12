#include "common/font_library.h"
#include "common/type_size.h"
#include "freetype/freetype.h"
#include "common/font_factory.h"
namespace COMMON_NAMESPACE
{
    CFontLibrary::CFontLibrary(const std::string& sPath)
        :m_sPath(sPath)
    {

    }


  

    void CFontLibrary::Serialize(CByte& data)
    {
        data << m_sPath;
    }

    void CFontLibrary::Deserialize(CByte& data)
    {
        data >> m_sPath;
    }

    size_t CFontLibrary::GetSize()
    {
        return CTypeSize::Instance().GetSize(m_sPath);
    }

  
 

    CFont::CFont(const std::string& context, std::unique_ptr<CFontLibrary> fontLibrary)
        : m_sContext(context), m_pFontLibrary(std::move(fontLibrary)), m_enableUnderline(false), m_fSize(11.0), m_fSpacing(0.5)
    {
        assert(m_pFontLibrary);
    }

    void CFont::Free()
    {
        CFontFactory::GetInstance().RemoveFont(*this);
    }

    void CFont::Serialize(CByte& data)
    { 
        data << m_sContext << m_enableUnderline << m_fSize << m_fSpacing;
        m_pFontLibrary->Serialize(data);
    }

    void CFont::Deserialize(CByte& data)
    {
        data >> m_sContext >> m_enableUnderline >> m_fSize >> m_fSpacing;
        m_pFontLibrary->Deserialize(data);
    }

    size_t CFont::GetSize()
    {
        size_t nSize = 0;
        nSize = CTypeSize::Instance().GetSize(m_sContext) + 1 + FLOAT_SIZE + FLOAT_SIZE + m_pFontLibrary->GetSize();
        return nSize;
    }

    CFontLibrary& CFont::GetFontLibrary()
    {
        return *m_pFontLibrary.get();
    }

    CFont::~CFont()
    {

    }
}

