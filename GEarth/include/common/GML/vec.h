#pragma once
#include <array>
template<class T>
class CVec2
{
public:
	CVec2(const T& x, const T& y) { m_data[0] = x; m_data[1] = y; };

	 T& X() const { return &m_data[0]; };
	 T& Y() const { return &m_data[1]; };
	 T& Value(const unsigned& index) const { return &m_data[index]; };
	 T* XY() { return m_data.data(); };

	 unsigned GetSize() { return sizeof(Type) * 2; };
private:
	std::array<T, 2>  m_data;
};

using CVec2f = CVec2<float>;
using CVec2d = CVec2<double>;



template<class T>
class CVec3
{
public:
	CVec3(){};
	CVec3(const T& x, const T& y, const T& z) { m_data[0] = x; m_data[1] = y; m_data[2] = z; };
	T& X() const { return &m_data[0]; };
	T& Y() const { return &m_data[1]; };
	T& Z() const { return &m_data[2]; };

	T& Value(const unsigned& index) const { return &m_data[index]; };
	T* XYZ() { return m_data.data(); };
	unsigned GetSize() { return sizeof(Type) * 3; };
private:
	std::array<T, 3>  m_data;
	using Type = T;
};

using CVec3f = CVec3<float>;
using CVec3d = CVec3<double>;