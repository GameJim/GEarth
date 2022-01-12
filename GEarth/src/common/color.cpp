#include "common/color.h"
#include "common/type_size.h"

namespace COMMON_NAMESPACE
{

    const CColor CColor::DIMGRAY = { 105.0f,105.0f,105.0f,0.0f};
    const CColor CColor::BACK = { 0.0f,0.0f,0.0f,0.0f };
    const CColor CColor::RED = { 255.0f,0.0f,0.0f,0.0f };
    const CColor CColor::GREEN = { 0.0f,255.0f,0.0f,0.0f };
    const CColor CColor::BULUE = { 0.0f,0.0f,255.0f,0.0f };
    const CColor CColor::WHITE = { 255.0f,255.0f,255.0f,0.0f };


    CColor::CColor(const float& _r, const float& _g, const float& _b, const float& _a /*= 1.0*/)
        :r(_r),g(_g),b(_b),a(_a)
    {

    }

    void CColor::Serialize(CByte& data)
    {
        data << r << g << b << a;
    }

    void CColor::Deserialize(CByte& data)
    {
        data >>r >> g >> b >> a;
    }

    size_t CColor::GetSize()
    {
        return 4 * FLOAT_SIZE;
    }

}