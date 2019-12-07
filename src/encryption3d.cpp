#include <iostream>
#include "encryption3d.hpp"

namespace encrypt3d {

  void Encryption3D::insereMsgPaillier2(const char* msg, int len, int blockSize, const Eigen::MatrixXf& inMesh, Eigen::MatrixXf& outMesh)
  {
    if (len <= 0 || msg == nullptr || blockSize <= 0)
      return;

    auto coords = inMesh.array();

    int msg_len_bits = len * 8;

    std::cout << "Mesh vertices num : " << inMesh.rows() << std::endl;
    std::cout << "Mesh float num : " << coords.array().size() << std::endl;
    std::cout << "Message bits : " << msg_len_bits << std::endl;

    // Combien de bits a inserer dans un chiffre flottant
    int k = 0;
    for (k = 1; k < sizeof(float);) {
      if (msg_len_bits / k > coords.size())
        k++;
      else
        break;
    }

    std::cout << k << " bits de message per float.\n";
  }

}
