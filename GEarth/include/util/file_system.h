#pragma once
//����Ӧ�ó���Ȼ��ײ����ɲ��ȥʵ��,������ɵ���ģʽ������������д
#include "util/util_export.h"
#include "util/define_helper.h"
#include "util/singleton_plugin.h"
#include <string>
namespace util
{
	class UTIL_API FileSystem : public SingletonPlugin
	{
		DEFINE_SINGLETON_PLUGIN(FileSystem)
	public:
		static std::string GetModulePath();
	public:
		virtual bool fileExists(const std::string& sPath) = 0;
		virtual std::string fileStem(const std::string& sPath) = 0;
		virtual std::string fileName(const std::string& sPath) = 0;
		virtual std::string fileExtension(const std::string& sPath) = 0;
		virtual std::string parentPath(const std::string& sPath) = 0;
		virtual bool isAbsPath(const std::string& sPath) = 0;
		virtual std::string searchFile(std::vector<std::string>& sRoots,const std::string& sRelatviePath) = 0;
	};

}


