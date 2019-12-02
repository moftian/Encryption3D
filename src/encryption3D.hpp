#ifndef MINIPROJET_ENCRYPTION3D_HPP
#define MINIPROJET_ENCRYPTION3D_HPP

#include <cstdint>
#include <bitset>
#include "rsa.hpp"

namespace encrypt3d {
  
  typedef uint32_t uint32;

  class Chiffre {
  private:
    float    m_float {};
    uint32 m_uint32 {};

  public:
    Chiffre() = default;

    float toFloat() const { return m_float; }
    uint32 toUint32() const { return m_uint32; }

    static Chiffre fromFloat(float value);

    static Chiffre fromUint32(uint32 value);

    Chiffre mantisse() const;

    Chiffre remplaceMantisse(const Chiffre& m) const;

    Chiffre encrypteMantisseRSA(int n, const RSA& rsa) const;
    Chiffre decrypteMantisseRSA(int n, const RSA& rsa) const;

    Chiffre insereMessageLSB(int n, uint32 value) const;
    Chiffre retireMessageLSB(int n) const;

  private:
    static uint32 get_n_bits_(uint32 f, int beg, int n);
    static uint32 remplace_n_bits_(uint32 f, int beg, int n, uint32 val);
  };

  std::ostream& operator<<(std::ostream& os, const Chiffre& c);
}


#endif //MINIPROJET_ENCRYPTION3D_HPP
