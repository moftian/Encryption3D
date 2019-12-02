#ifndef RSA_HPP
#define RSA_HPP

#include <cmath>
#include <cstdint>

namespace encrypt3d {

    bool isPremier(uint32_t n);
    uint32_t pgcd(uint32_t a , uint32_t b);
    bool isPremierEntreEux(uint32_t p , uint32_t q);
    uint32_t expMod(uint32_t a , uint32_t e, uint32_t n);
    uint32_t modInverse(uint32_t a , uint32_t m);

    class RSA{
    public :
        explicit RSA(uint32_t e);
        uint32_t encrypte(uint32_t msg) const;
        uint32_t decrypte(uint32_t msg) const;
        void generate(uint32_t p ,uint32_t q);
        uint32_t p() const { return m_p; }
        uint32_t q() const { return m_q; }
        uint32_t e() const { return m_e; }
        uint32_t d() const { return m_d; }
        uint32_t n() const { return m_n; }
        uint32_t phi() const { return m_phi; }
    private:
        uint32_t m_p, m_q, m_e, m_d, m_n, m_phi;
    };

}

#endif
