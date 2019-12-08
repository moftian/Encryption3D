
#include <cstdlib>
#include "paillier.hpp"

namespace encrypt3d {

  Paillier::Paillier(int32_t n)
      : m_pub_key{nullptr}, m_prv_key{nullptr}
  {
    m_bits = n;
    // generate public and private keys
    paillier_keygen(n, &m_pub_key, &m_prv_key, paillier_get_rand_devurandom);
  }

  Paillier::~Paillier() {
    paillier_freepubkey(m_pub_key);
    paillier_freeprvkey(m_prv_key);
  }

  Chiffre64 Paillier::encrypte(const encrypt3d::Chiffre32& m) const
  {
    return Chiffre64::fromUint64(encrypte(m.toUint32()));
  }

  uint64_t Paillier::encrypte(uint32_t m) const
  {
    paillier_plaintext_t *mtxt = paillier_plaintext_from_ui(m);
    paillier_ciphertext_t *ctxt = paillier_enc(nullptr, m_pub_key, mtxt, paillier_get_rand_devurandom);

    uint64_t c = mpz_get_ui(ctxt->c);

    paillier_freeplaintext(mtxt);
    paillier_freeciphertext(ctxt);

    return c;
  }

  Chiffre32 Paillier::decrypte(const encrypt3d::Chiffre64& c) const
  {
    return Chiffre32::fromUint32(decrypte(c.toUint64()));
  }

  uint32_t Paillier::decrypte(uint64_t c) const
  {

    auto *ctxt = (paillier_ciphertext_t *)malloc(sizeof(paillier_ciphertext_t));
    mpz_init_set_ui(ctxt->c, c);

    paillier_plaintext_t *mtxt = paillier_dec(nullptr, m_pub_key, m_prv_key, ctxt);

    uint32_t m = mpz_get_ui(mtxt->m);

    paillier_freeciphertext(ctxt);
    paillier_freeplaintext(mtxt);

    return m;
  }

  Chiffre64 Paillier::multiply(const encrypt3d::Chiffre64 &a, const encrypt3d::Chiffre64 &b) const
  {
    return Chiffre64::fromUint64(multiply(a.toUint64(), b.toUint64()));
  }

  uint64_t Paillier::multiply(uint64_t a, uint64_t b) const
  {
    paillier_ciphertext_t* sum = paillier_create_enc_zero();
    auto *ctxt_a = (paillier_ciphertext_t *)malloc(sizeof(paillier_ciphertext_t));
    auto *ctxt_b = (paillier_ciphertext_t *)malloc(sizeof(paillier_ciphertext_t));
    mpz_init_set_ui(ctxt_a->c, a);
    mpz_init_set_ui(ctxt_b->c, b);

    paillier_mul(m_pub_key, sum, ctxt_a, ctxt_b);

    uint64_t c = mpz_get_ui(sum->c);

    paillier_freeciphertext(ctxt_a);
    paillier_freeciphertext(ctxt_b);
    paillier_freeciphertext(sum);

    return c;
  }

}
