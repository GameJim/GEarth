#pragma once



namespace util
{

	template<typename T, typename R>
	T convertPointerType(R src)
	{
		T dest;
		memcpy(&dest, &src, sizeof(src));
		return dest;
	}
}