// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len < 0) {
      throw "the length must be greater than 0 or equal to 0";
    }
    this->BitLen = len;
    this->MemLen = (BitLen+(sizeof(TELEM)*8)-1) / (sizeof(TELEM)*8);
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) 
    {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    this->BitLen = bf.BitLen;
    this->MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; ++i) {
        this->pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return (n/(sizeof(TELEM)*8)); // sizeof возвращает размер в байтах
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
     return (TELEM)1 << (n % (sizeof(TELEM) * 8)); // sizeof возвращает размер в байтах
    // return 1 << (~n | ~(sizeof(TELEM) * 8) -1);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0) {
        throw "Bit index less than zero";
    }
	if (n > this->BitLen) {
        throw "Bit index out of range";
	}
    size_t memi = GetMemIndex(n);
    this->pMem[memi] = this->pMem[memi] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0) {
        throw "Bit index less than zero";
	}
	if (n > this->BitLen) {
        throw "Bit index out of range";
	}
	size_t memi = GetMemIndex(n);
	this->pMem[memi] = (this->pMem[memi] | GetMemMask(n)) ^ GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0) {
        throw "Bit index less than zero";
	}
	if (n > this->BitLen) {
        throw "Bit index out of range";
	}
    size_t memi = GetMemIndex(n);
    TELEM b = (this->pMem[memi] & GetMemMask(n)) >> (n % (sizeof(TELEM) * 8));
    return (int)b;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    delete[] pMem;
    this->pMem = new TELEM[bf.MemLen];
    this->MemLen = bf.MemLen;
	this->BitLen = bf.BitLen;
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen) return 0;
    for (size_t i = 0; i < min(this->MemLen, bf.MemLen); ++i) {
        if (this->pMem[i] != bf.pMem[i]) {
        return 0;
        }
    }
    
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (bf == *this) return 0;
    return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    TBitField x (bf.BitLen > this->BitLen ? bf : *this);
    size_t minLen = min(bf.MemLen, this->MemLen);
    for (size_t i = 0; i < minLen; ++i) {
        x.pMem[i] = bf.pMem[i] | this->pMem[i];
    }
    return x;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	TBitField x(bf.BitLen > this->BitLen ? bf.BitLen : this->BitLen);
    size_t minLen = min(bf.MemLen, this->MemLen);
	for (size_t i = 0; i < minLen; ++i) {
		x.pMem[i] = bf.pMem[i] & this->pMem[i];
	}
	return x;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField x (*this);
	for (size_t i = 0; i < this->MemLen; ++i) {
		x.pMem[i] = ~(this->pMem[i]);
	}

    x.pMem[MemLen - 1] = x.pMem[MemLen - 1] << MemLen * (sizeof(TELEM) * 8) - (BitLen) >> MemLen * (sizeof(TELEM) * 8) - (BitLen);
	
	return x;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    TELEM inp = 0;
    for (size_t i = 0; i < bf.BitLen; ++i) {
        istr >> inp;
        if (inp == 1) {
            bf.SetBit(i);
        }
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (size_t i = 0; i < bf.BitLen; ++i) {
		ostr << bf.GetBit(i);
	}
    return ostr;
}
