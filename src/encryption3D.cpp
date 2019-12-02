#include <iostream>
#include "encryption3D.hpp"

namespace encrypt3d {

  Chiffre Chiffre::fromFloat(float value)
  {
    Chiffre c;
    c.m_float = value;
    c.m_uint32 = *reinterpret_cast<uint32*>(&value);
    return c;
  }

  Chiffre Chiffre::fromUint32(uint32 value)
  {
    Chiffre c;
    c.m_uint32 = value;
    c.m_float = *reinterpret_cast<float*>(&value);
    return c;
  }

  Chiffre Chiffre::mantisse() const
  {
    return Chiffre::fromUint32(get_n_bits_(m_uint32, 9, 23));
  }

  Chiffre Chiffre::remplaceMantisse(const Chiffre& m) const
  {
    uint32 v = remplace_n_bits_(m_uint32, 9, 23, m.toUint32());
    return Chiffre::fromUint32(v);
  }

  Chiffre Chiffre::encrypteMantisseRSA(int n, const RSA& rsa) const
  {
    if (n <= 0) { return *this; }
    uint32 m = get_n_bits_(m_uint32, 9, n);
    uint32 l = 32 - ((9 + (unsigned)n) > 32 ? 32 : (9 +(unsigned)n));
    //std::cout << l << std::endl;
    //std::cout << "E  : " << std::bitset<32>(m) << " " << m << std::endl;
    m >>= l;
    std::cout << "E- : " << std::bitset<32>(m) << " " << m << std::endl;
    uint32 mc = rsa.encrypte(m);
    std::cout << "EE : " << std::bitset<32>(mc) << " " << mc << std::endl;
    mc <<= l;
    //std::cout << "EE-: " << std::bitset<32>(mc) << " " << mc << std::endl;

    return Chiffre::fromUint32(remplace_n_bits_(m_uint32, 9, n, mc));
  }

  Chiffre Chiffre::decrypteMantisseRSA(int n, const RSA& rsa) const
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

    return Chiffre::fromUint32(remplace_n_bits_(m_uint32, 9, n, mc));
  }

  Chiffre Chiffre::insereMessageLSB(int n, uint32 value) const
  {
    if (n <= 0) { return *this; }
    return Chiffre::fromUint32(remplace_n_bits_(m_uint32, (32-n), n, value));
  }

  Chiffre Chiffre::retireMessageLSB(int n) const
  {
    if (n <= 0) { return *this; }
    return Chiffre::fromUint32(get_n_bits_(m_uint32, (32-n), n));
  }

  uint32 Chiffre::get_n_bits_(uint32 f, int beg, int n)
  {
    if (n <= 0 || beg < 0 || beg > 32) {
      return 0;
    }
    uint32 masque = (0xFFFFFFFF >> (unsigned)beg) & (0xFFFFFFFF << (unsigned)(32 - ((beg+n)>32?32:(beg+n))));
    return f & masque;
  }

  uint32 Chiffre::remplace_n_bits_(uint32 f, int beg, int n, uint32 val)
  {
    uint32 pre = get_n_bits_(f, 0, beg);
    uint32 post = get_n_bits_(f, beg + n, 32 - beg - n);
    return (val | pre | post);
  }

  std::ostream& operator<<(std::ostream& os, const Chiffre& c)
  {
    os << std::bitset<32>(c.toUint32()) << std::endl;
    return os;
  }

  /*
  uint32 getNBits(uint32 f, uint32 beg, int n) {
    if (n <= 0) {
      return 0;
    }
    uint32 masque = (0xFFFFFFFF >> beg) & (0xFFFFFFFF << (32 - ((beg+n)>32?32:(beg+n)) ));
    return f & masque;
  }

  uint32 getMantisse(float f) {
    uint32 v = *reinterpret_cast<uint32*>(&f);
    return getNBits(v, 9, 23);
  }

  float remplaceMantisse(float f, uint32 m)
  {
    uint32 f1 = *reinterpret_cast<uint32*>(&f);
    uint32 v = remplaceNBits(f1, 9, 23, m);
    return *reinterpret_cast<float*>(&v);
  }

  uint32 remplaceNBits(uint32 f, uint32 beg, int n, uint32 v)
  {
    uint32 pre = getNBits(f, 0, beg);
    std::cout << "Pre : " << bits(pre) << std::endl;
    uint32 post = getNBits(f, beg + n, 32 - beg - n);
    std::cout << "Post: " << bits(post) << std::endl;
    uint32 mid = getNBits(f, beg, n);
    std::cout << "Mid : " << bits(mid) << std::endl;
    uint32 res = (v | pre | post);
    std::cout << "Val : " << bits(v) << std::endl;
    std::cout << "Res : " << bits(res) << std::endl;
    return res;
  }
   */
}
