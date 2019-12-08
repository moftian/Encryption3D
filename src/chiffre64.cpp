#include <iostream>
#include "chiffre64.hpp"

namespace encrypt3d {

  Chiffre64 Chiffre64::fromFloat(float value)
  {
    Chiffre64 c;
    c.m_double = static_cast<double>(value);
    c.m_uint64 = *reinterpret_cast<uint64_t*>(&c.m_double);
    return c;
  }

  Chiffre64 Chiffre64::fromUint32(uint32_t value)
  {
    Chiffre64 c;
    c.m_uint64 = static_cast<uint64_t>(value);
    c.m_double = *reinterpret_cast<double*>(&c.m_uint64);
    return c;
  }

  Chiffre64 Chiffre64::fromUint64(uint64_t value)
  {
    Chiffre64 c;
    c.m_uint64 = value;
    c.m_double = *reinterpret_cast<double*>(&c.m_uint64);
    return c;
  }

  Chiffre64 Chiffre64::fromDouble(double value)
  {
    Chiffre64 c;
    c.m_double = value;
    c.m_uint64 = *reinterpret_cast<uint64_t*>(&c.m_double);
    return c;
  }

  Chiffre64 Chiffre64::fromChiffre32(const Chiffre32& value)
  {
    return Chiffre64::fromUint32(value.toUint32());
  }

  Chiffre64 Chiffre64::mantisse() const
  {
    return Chiffre64::fromUint64(getNBits(m_uint64, 12, 52));
  }

  Chiffre64 Chiffre64::remplaceMantisse(const encrypt3d::Chiffre64 &m) const
  {
    uint64_t v = replaceNBits(m_uint64, 12, 52, m.toUint64());
    return Chiffre64::fromUint64(v);
  }


  uint64_t Chiffre64::getNBits(uint64_t f, int beg, int n)
  {
    if (n <= 0 || beg < 0 || beg > 64)
      return 0;

    uint64_t masque = (0xFFFFFFFFFFFFFFFF >> (unsigned)beg) & (0xFFFFFFFFFFFFFFFF << (unsigned)(64 - ((beg+n)>64?64:(beg+n))));
    return f & masque;
  }

  uint64_t Chiffre64::replaceNBits(uint64_t f, int beg, int n, uint64_t val)
  {
    uint64_t pre = getNBits(f, 0, beg);
    uint32_t post = getNBits(f, beg + n, 64 - beg - n);
    return (val | pre | post);
  }

  std::ostream& operator<<(std::ostream& os, const Chiffre64& c)
  {
    os << std::bitset<64>(c.toUint64()) << std::endl;
    return os;
  }

}
