#ifndef MINIPROJET_CHIFFRE32_HPP
#define MINIPROJET_CHIFFRE32_HPP

#include <cstdint>
#include <bitset>
#include "rsa.hpp"

namespace encrypt3d {

  class Chiffre64;

  class Chiffre32 {
  private:
    float  m_float {};
    uint32_t m_uint32 {};

  public:
    Chiffre32() = default;

    float toFloat() const { return m_float; }
    uint32_t toUint32() const { return m_uint32; }

    static Chiffre32 fromFloat(float value);

    static Chiffre32 fromUint32(uint32_t value);

    static Chiffre32 fromDouble(double value);

    static Chiffre32 fromUint64(uint64_t value);

    static Chiffre32 fromChiffre64(const Chiffre64& value);

    Chiffre32 mantisse() const;

    Chiffre32 remplaceMantisse(const Chiffre32& m) const;

    Chiffre32 encrypteMantisseRSA(int n, const RSA& rsa) const;
    Chiffre32 decrypteMantisseRSA(int n, const RSA& rsa) const;

    Chiffre32 insereMessageLSB(int n, uint32_t value) const;
    Chiffre32 retireMessageLSB(int n) const;

    Chiffre32 leftShift(int n) const;
    Chiffre32 rightShift(int n) const;

  public:
    static uint32_t getNBits(uint32_t f, int beg, int n);
    static uint32_t replaceNBits(uint32_t f, int beg, int n, uint32_t val);
  };

  std::ostream& operator<<(std::ostream& os, const Chiffre32& c);
}


#endif //MINIPROJET_CHIFFRE32_HPP
