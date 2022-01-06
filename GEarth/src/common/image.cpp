#include "common/image.h"

namespace COMMON_NAMESPACE
{
    CImage::CImage()
        :m_pData(nullptr)
        , m_nWidth(0)
        , m_nHeigth(0)
        , m_nChannel(0)
        , m_eImageFormateType(IFT_UNKNOWN)
    {

    }

    CImage::CImage(unsigned char* data, unsigned nWidth, unsigned nHegith, unsigned nChannel, const EnImageFormateType& eImageFormateType /*= IFT_DEFAULT*/)
        :m_pData(data)
        , m_nWidth(nWidth)
        , m_nHeigth(nChannel)
        , m_eImageFormateType(eImageFormateType)
    {
        if (eImageFormateType == IFT_DEFAULT)
        {
            if (nChannel == 1)
            {
                m_eImageFormateType = EnImageFormateType::IFT_GRAY;
            }
            else if (nChannel == 3)
            {
                m_eImageFormateType = EnImageFormateType::IFT_RGB;
            }
            else if (nChannel == 4)
            {
                m_eImageFormateType = EnImageFormateType::IFT_RGBA;
            }
            else
            {
                m_eImageFormateType = EnImageFormateType::IFT_BINARY; //二进制
            }
        }
    }

    CImage::~CImage()
    {
        if (m_pData)
        {
            delete m_pData;
        }
    }

    unsigned CImage::GetImageSize() const
    {
        if (m_eImageFormateType == EnImageFormateType::IFT_BINARY)
        {
            return m_nWidth * m_nHeigth / 8;  //一个字节8位
        }
        else
            return m_nWidth * m_nHeigth * m_nChannel;

    }

    CImage& CImage::operator=(const CImage& image)
    {
        if (m_pData)
        {
            delete[] m_pData;
        }

        unsigned nSize = GetImageSize();
        m_pData = new unsigned char[nSize];
        memcpy(m_pData, image.m_pData, nSize);

        m_nHeigth = image.m_nHeigth;
        m_nWidth = image.m_nWidth;
        m_nChannel = image.m_nChannel;
        m_eImageFormateType = image.m_eImageFormateType;

        return *this;
    }

    void CImage::Serialize(CByte& data)
    {
        data << m_nWidth << m_nHeigth << m_nChannel << m_eImageFormateType;
        data.WriteCharArry(m_pData, GetImageSize());
    }

    void CImage::Deserialize(CByte& data)
    {
        data.ReadCharArry(m_pData, GetImageSize());
    }

    size_t CImage::GetSize()
    {
        return 4 + 4 + 2 + GetImageSize() + 1;
    }

    bool CImage::IsEnable() const
    {
        if (m_pData == nullptr || m_nWidth == 0 || m_nHeigth == 0)
        {
            return false;
        }

        switch (m_eImageFormateType)
        {
        case CImage::IFT_UNKNOWN:
            return false;
            break;
        case CImage::IFT_BINARY:
        {
            if (m_nChannel != 0)
            {
                return false;
            }
        }
        break;
        case CImage::IFT_GRAY:
        {
            if (m_nChannel != 1)
            {
                return false;
            }
        }
        break;
        case CImage::IFT_RGB:
        case CImage::IFT_BGR:
            if (m_nChannel != 3)
            {
                return false;
            }
            break;
        case CImage::IFT_RGBA:
        case CImage::IFT_BGRA:
            if (m_nChannel != 4)
            {
                return false;
            }
            break;
        default:
            break;
        }

        return true;
    }

    void CImage::Resize() const
    {

    }

    void CImage::UpdateSubArea(const unsigned char* data, const unsigned& nWidth, const unsigned& nHegith, const unsigned& x /*= 0*/, const unsigned& y/*=0*/)
    {

    }
}


