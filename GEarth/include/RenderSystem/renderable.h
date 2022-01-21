/*!
*@File    renderable.h
*@Brief   渲染数据类
*@Date    2022/1/14
*/
#pragma once
#include "common/apiDescription.h"
namespace render
{
    //一份数据可以有多份状态机，与反射等。一个状态集合可以有多份数据
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

        //物体的材质e
       
    };
}