/*!
*@File    EarthManipulator.h
*@Brief   文件描述:下次一定
*@Date    2022/1/26
*/
#pragma once




#include "common/apiDescription.h"
#include "osgGA/CameraManipulator"

namespace earth
{

    class CORE_PUBLIC CEarthManipulator : public osgGA::CameraManipulator
    {
    public:
        CEarthManipulator();
        virtual ~CEarthManipulator();

        virtual osg::Matrixd getMatrix() const;

        virtual osg::Matrixd getInverseMatrix() const;

        virtual void setByMatrix(const osg::Matrixd& matrix) ;

        virtual void setByInverseMatrix(const osg::Matrixd& matrix);
    };
}