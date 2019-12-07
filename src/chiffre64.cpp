#include <iostream>
#include "chiffre64.hpp"

namespace encrypt3d {

  Chiffre64 Chiffre64::fromFloat(float value)
  {
    Chiffre64 c;
    c.m_double = static_cast<double>(value);
    c.m_uint64 = *reinterpret_cast<uint64*>(&c.m_double);
    return c;
  }

  Chiffre64 Chiffre64::fromUint32(uint32 value)
  {
    Chiffre64 c;
    c.m_uint64 = static_cast<uint64>(value);
    c.m_double = *reinterpret_cast<double*>(&c.m_uint64);
    return c;
  }

  Chiffre64 Chiffre64::fromUint64(uint64 value)
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
    c.m_uint64 = *reinterpret_cast<uint64*>(&c.m_double);
    return c;
  }

  Chiffre64 Chiffre64::fromChiffre32(const Chiffre32& value)
  {
    return Chiffre64::fromUint32(value.toUint32());
  }

  Chiffre64 Chiffre64::mantisse() const
  {
    return Chiffre64::fromUint64(get_n_bits_(m_uint64, 12, 52));
  }

  Chiffre64 Chiffre64::remplaceMantisse(const encrypt3d::Chiffre64 &m) const
  {
    uint64 v = remplace_n_bits(m_uint64, 12, 52, m.toUint64());
    return Chiffre64::fromUint64(v);
  }


  uint64 Chiffre64::get_n_bits_(encrypt3d::uint64 f, int beg, int n)
  {
    if (n <= 0 || beg < 0 || beg > 64)
      return 0;

    uint64 masque = (0xFFFFFFFFFFFFFFFF >> (unsigned)beg) & (0xFFFFFFFFFFFFFFFF << (unsigned)(64 - ((beg+n)>64?64:(beg+n))));
    return f & masque;
  }

  uint64 Chiffre64::remplace_n_bits(encrypt3d::uint64 f, int beg, int n, encrypt3d::uint64 val)
  {
    uint64 pre = get_n_bits_(f, 0, beg);
    uint32 post = get_n_bits_(f, beg+n, 64 - beg - n);
    return (val | pre | post);
  }

  std::ostream& operator<<(std::ostream& os, const Chiffre64& c)
  {
    os << std::bitset<64>(c.toUint64()) << std::endl;
    return os;
  }

}
