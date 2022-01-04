#include "common/byte.h"
#include "Common/type_size.h"


CByte::CByte(unsigned char* src, unsigned nSize)
	:m_pData(src),m_nSize(nSize)
{
	this->SeekgPos(0);
}


CByte::CByte(unsigned nSize)
	:m_nSize(nSize)
{
	m_pData = new unsigned char[nSize];
	this->SeekgPos(0);
}

CByte::~CByte()
{
	if (m_pData)
	{
		delete[] m_pData;
	}
	m_pData = nullptr;
}

const unsigned char* CByte::Data()
{
	return m_pData;
}

void CByte::SeekgPos(const unsigned& nsize /*= 0*/)
{
	itr = &m_pData[nsize];
}

void CByte::Seekg(const unsigned& nsize)
{
	itr = &itr[nsize];
}

CByte& CByte::operator >> (bool& b)
{
	memcpy(&b, itr, BOOL_SIZE);
	itr+= BOOL_SIZE;

	return *this;
}

CByte& CByte::operator >> (char& c)
{
	memcpy(&c, itr,CHAR_SIZE);
	itr += CHAR_SIZE;

	return *this;
}

CByte& CByte::operator >> (uint8_t& ui8)
{
	memcpy(&ui8, itr, UINT8_SIZE);
	itr += UINT8_SIZE;

	return *this;
}


CByte& CByte::operator >> (short& s)
{
	memcpy(&s, itr, SHORT_SIZE);
	itr += SHORT_SIZE;

	return *this;
}

CByte& CByte::operator >> (int& i)
{
	memcpy(&i, itr, INT_SIZE);
	itr += INT_SIZE;

	return *this;
}

CByte& CByte::operator >> (unsigned& ui)
{
	memcpy(&ui, itr, UINT_SIZE);
	itr += UINT_SIZE;

	return *this;
}


//CByte& CByte::operator >> (unsigned int& ui)
//{
//    memcpy(&ui, itr, UINT_SIZE);
//    itr += UINT_SIZE;
//
//    return *this;
//}

CByte& CByte::operator >> (float& f)
{
	memcpy(&f, itr, FLOAT_SIZE);
	itr += FLOAT_SIZE;

	return *this;
}

CByte& CByte::operator >> (double& d)
{
	memcpy(&d, itr, DOUBLE_SIZE);
	itr += DOUBLE_SIZE;

	return *this;
}

CByte& CByte::operator >> (std::string& str)
{
	unsigned size;
	*this >> size;

	str.resize(size);
	if (size > 0)
	{
		memcpy(&str[0], itr, size);
		itr += size;
	}
	return *this;
}



void CByte::ReadCharArry(unsigned char* data, unsigned nSize)
{
    memcpy(data, itr, nSize);
    itr += nSize;
}

void CByte::WriteCharArry(unsigned char* data, unsigned nSize)
{
    memcpy(itr, data, nSize);
    itr += nSize;
}

CByte& CByte::operator<<(const std::string& str)
{
	unsigned size = str.size();
	*this << size;

	if (size > 0)
	{
		memcpy(itr, &str[0], size);
		itr += size;
	}
	return *this;
}

CByte& CByte::operator<<(const double& d)
{
	memcpy(itr, &d, DOUBLE_SIZE);
	itr += DOUBLE_SIZE;

	return *this;
}

CByte& CByte::operator<<(const float& f)
{
	memcpy(itr, &f, FLOAT_SIZE);
	itr += FLOAT_SIZE;

	return *this;
}

CByte& CByte::operator<<(const unsigned& ui)
{
	memcpy(itr, &ui, UINT_SIZE);
	itr += UINT_SIZE;

	return *this;
}

//CByte& CByte::operator<<(unsigned int& ui)
//{
//    memcpy(itr, &ui, UINT_SIZE);
//    itr += UINT_SIZE;
//
//    return *this;
//}


CByte& CByte::operator<<(const int& i)
{
	memcpy(itr, &i, INT_SIZE);
	itr += INT_SIZE;

	return *this;
}

CByte& CByte::operator<<(const short& s)
{
	memcpy(itr, &s, SHORT_SIZE);
	itr += SHORT_SIZE;

	return *this;
}

CByte& CByte::operator<<(const char& c)
{
	memcpy(itr, &c, CHAR_SIZE);
	itr += CHAR_SIZE;

	return *this;
}

CByte& CByte::operator<<(const bool& b)
{
	memcpy(itr, &b, BOOL_SIZE);
	itr += BOOL_SIZE;

	return *this;
}

CByte& CByte::operator<<(const uint8_t& ui8)
{
	memcpy(itr, &ui8, UINT8_SIZE);
	itr += UINT8_SIZE;

	return *this;
}

