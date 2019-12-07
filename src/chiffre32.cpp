#include <iostream>
#include "chiffre32.hpp"

namespace encrypt3d {

  Chiffre32 Chiffre32::fromFloat(float value)
  {
    Chiffre32 c;
    c.m_float = value;
    c.m_uint32 = *reinterpret_cast<uint32*>(&value);
    return c;
  }

  Chiffre32 Chiffre32::fromUint32(uint32 value)
  {
    Chiffre32 c;
    c.m_uint32 = value;
    c.m_float = *reinterpret_cast<float*>(&value);
    return c;
  }

  Chiffre32 Chiffre32::mantisse() const
  {
    return Chiffre32::fromUint32(get_n_bits_(m_uint32, 9, 23));
  }

  Chiffre32 Chiffre32::remplaceMantisse(const Chiffre32& m) const
  {
    uint32 v = remplace_n_bits_(m_uint32, 9, 23, m.toUint32());
    return Chiffre32::fromUint32(v);
  }

  Chiffre32 Chiffre32::encrypteMantisseRSA(int n, const RSA& rsa) const
  {
    if (n <= 0) { return *this; }
    uint32 m = get_n_bits_(m_uint32, 9, n);
    uint32 l = 32 - ((9 + (unsigned)n) > 32 ? 32 : (9 +(unsigned)n));
    //std::cout << l << std::endl;
    //std::cout << "E  : " << std::bitset<32>(m) << " " << m << std::endl;
    m >>= l;
    //std::cout << "E- : " << std::bitset<32>(m) << " " << m << std::endl;
    uint32 mc = rsa.encrypte(m);
    //std::cout << "EE : " << std::bitset<32>(mc) << " " << mc << std::endl;
    mc <<= l;
    //std::cout << "EE-: " << std::bitset<32>(mc) << " " << mc << std::endl;

    return Chiffre32::fromUint32(remplace_n_bits_(m_uint32, 9, n, mc));
  }

  Chiffre32 Chiffre32::decrypteMantisseRSA(int n, const RSA& rsa) const
  {
    if (n <= 0) { return *this; }
    uint32 m = get_n_bits_(m_uint32, 9, n);
    //std::cout << "D  : " << std::bitset<32>(m) << " " << m << std::endl;
    uint32 l = 32 - ((9 + (unsigned)n) > 32 ? 32 : (9 +(unsigned)n));
    m >>= l;
    //std::cout << "D- : " << m << std::endl;
    uint32 mc = rsa.decrypte(m);
    //std::cout << "DD : " << std::bitset<32>(mc) << " " << mc << std::endl;
    mc <<= l;
    //std::cout << "DD-: " << std::bitset<32>(mc) << " " << mc << std::endl;

    return Chiffre32::fromUint32(remplace_n_bits_(m_uint32, 9, n, mc));
  }

  Chiffre32 Chiffre32::insereMessageLSB(int n, uint32 value) const
  {
    if (n <= 0) { return *this; }
    return Chiffre32::fromUint32(remplace_n_bits_(m_uint32, (32 - n), n, value));
  }

  Chiffre32 Chiffre32::retireMessageLSB(int n) const
  {
    if (n <= 0) { return *this; }
    return Chiffre32::fromUint32(get_n_bits_(m_uint32, (32 - n), n));
  }

  uint32 Chiffre32::get_n_bits_(uint32 f, int beg, int n)
  {
    if (n <= 0 || beg < 0 || beg > 32) {
      return 0;
    }
    uint32 masque = (0xFFFFFFFF >> (unsigned)beg) & (0xFFFFFFFF << (unsigned)(32 - ((beg+n)>32?32:(beg+n))));
    return f & masque;
  }

  uint32 Chiffre32::remplace_n_bits_(uint32 f, int beg, int n, uint32 val)
  {
    uint32 pre = get_n_bits_(f, 0, beg);
    uint32 post = get_n_bits_(f, beg + n, 32 - beg - n);
    return (val | pre | post);
  }

  std::ostream& operator<<(std::ostream& os, const Chiffre32& c)
  {
    os << std::bitset<32>(c.toUint32()) << std::endl;
    return os;
  }
}
