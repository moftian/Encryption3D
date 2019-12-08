#ifndef MINIPROJET_PAILLIER_HPP
#define MINIPROJET_PAILLIER_HPP

#include "chiffre32.hpp"
#include "chiffre64.hpp"

#include <gmp.h>

extern "C" {
  #include <paillier/paillier.h>
}

#include <cstdint>

namespace encrypt3d {

  struct Paillier {
  public:
    /// Constructs Paillier Crypto-System
    /// \param [in] bits number of bits of the modulus
    explicit Paillier(int32_t bits);

    ~Paillier();

    Chiffre64 encrypte(const Chiffre32 &m) const;

    Chiffre32 decrypte(const Chiffre64 &c) const;

    /// E(a) * E(b) = E(a + b)
    /// \param a [in] first encrypted number
    /// \param b [in] second encrypted number
    /// \return Sum the encrypted a and b
    Chiffre64 multiply(const Chiffre64& a, const Chiffre64& b) const;

  private:
    int32_t m_bits;
    paillier_prvkey_t* m_prv_key;
    paillier_pubkey_t* m_pub_key;

  private:
    uint64_t encrypte(uint32_t m) const;
    uint32_t decrypte(uint64_t c) const;
    uint64_t multiply(uint64_t a, uint64_t b) const;
  };

}


#endif //MINIPROJET_PAILLIER_HPP
