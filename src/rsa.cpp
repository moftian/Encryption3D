#include "rsa.hpp"
#include <iostream>

namespace encrypt3d {

    bool isPremier(uint32_t n)
    {
        for(uint32_t i=2 ; i<=sqrt(n);i++){
            if(n % i == 0 ){return false ;}
        }
        return true ;
    }

    uint32_t pgcd(uint32_t a , uint32_t b )
    {
        if(a==0){ return b ;}
        if(b==0){ return a ;}
        if(a==b){return a ;}
        if(a>b){return pgcd(a-b,b);}
        return pgcd(a,b-a);
    }


    bool isPremierEntreEux(uint32_t p , uint32_t q )
    {
        if(pgcd(p,q)==1){return true;}
        else 
            return false;
    }

    uint32_t expMod(uint32_t a , uint32_t e, uint32_t n)
    {
        uint32_t resultat=1;
        for(uint32_t i =0 ; i<e;i++)
            {
                resultat = ((resultat % n )*(a%n))%n;
            }
        return resultat;
    }

    uint32_t modInverse(uint32_t a , uint32_t m)
    {
        a = a % m;
        for(int x = 1; x < m ; x++){
            if((a*x)% m == 1)
                return x ;
        }
    }

    void RSA::generate(uint32_t p , uint32_t q)
    {
        if (!isPremier(p)) {std::cout<<"chiffre " << p << " non premier" << std::endl; exit(-1);};
        if (!isPremier(q)) {std::cout<<"chiffre " << q<< " non premier" << std::endl; exit(-1);};
        m_p = p;
        m_q = q;
        //std::cout << "p: " << m_p << "\nq: " << m_q << std::endl;
        m_n = p * q;
        uint32_t m_phi=(p-1)*(q-1);
        //std::cout << "n: " << m_n << std::endl << "phi: " << m_phi << std::endl;
        //std::cout << "e: " << m_e << std::endl;
        m_d = modInverse(m_e, m_phi);
        //std::cout << "d: " << m_d << std::endl;
    }

    uint32_t RSA::encrypte(uint32_t msg) const
    {
        return expMod(msg, m_e, m_n);   
    }

    uint32_t RSA::decrypte(uint32_t msg) const
    {
        return expMod(msg, m_d, m_n);
    }

    RSA::RSA(uint32_t e)
    { 
        m_e = e;
        m_p = m_q = m_d = m_n = m_phi = 0;
    }
}