/*!
*@File    renderable.h
*@Brief   渲染数据类
*@Date    2022/1/14
*/
#pragma once
#include "common/apiDescription.h"
namespace render
{
    //vertex, normals, texcoords

    //纹理若绑定

    //颜色不一定有

  
    class CORE_PUBLIC CRenderable
    {
    public:
        CRenderable() = default;
        virtual ~CRenderable() = default;

        //顶点数组
        //纹理数据必定与顶点数据1对1

        //物体的材质
       
    };
}