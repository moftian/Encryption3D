#ifndef MINIPROJET_CHIFFRE32_HPP
#define MINIPROJET_CHIFFRE32_HPP

#include <cstdint>
#include <bitset>
#include "rsa.hpp"

namespace encrypt3d {
  
  typedef uint32_t uint32;

  class Chiffre32 {
  private:
    float  m_float {};
    uint32 m_uint32 {};

  public:
    Chiffre32() = default;

    float toFloat() const { return m_float; }
    uint32 toUint32() const { return m_uint32; }

    static Chiffre32 fromFloat(float value);

    static Chiffre32 fromUint32(uint32 value);

    Chiffre32 mantisse() const;

    Chiffre32 remplaceMantisse(const Chiffre32& m) const;

    Chiffre32 encrypteMantisseRSA(int n, const RSA& rsa) const;
    Chiffre32 decrypteMantisseRSA(int n, const RSA& rsa) const;

    Chiffre32 insereMessageLSB(int n, uint32 value) const;
    Chiffre32 retireMessageLSB(int n) const;

  private:
    static uint32 get_n_bits_(uint32 f, int beg, int n);
    static uint32 remplace_n_bits_(uint32 f, int beg, int n, uint32 val);
  };

  std::ostream& operator<<(std::ostream& os, const Chiffre32& c);
}


#endif //MINIPROJET_CHIFFRE32_HPP
