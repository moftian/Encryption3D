#include <igl/readOFF.h>
#include <igl/readOBJ.h>
#include <igl/opengl/glfw/Viewer.h>

#include "encryption3d.hpp"
#include "chiffre32.hpp"
#include "chiffre64.hpp"
#include "paillier.hpp"

#ifndef __STDC_IEC_559__
  #error The following Programm only supports float operations using the IEEE 754 Standard.
#endif

int main(int argc, char** argv)
{
  using namespace encrypt3d;

  Eigen::MatrixXd vertices;
  Eigen::MatrixXi faces;

  igl::readOBJ("../models/suzanne.obj", vertices, faces);

  std::string msg = "Je sais pas quoi faire.";

  Eigen::MatrixXf verticesEncrypte(vertices.rows(), vertices.cols());
  Eigen::MatrixXf verticesF = vertices.cast<float>();

  Encryption3D::insereMsgPaillier2(msg.c_str(), msg.size(), 4, verticesF, verticesEncrypte);


  /*

  RSA rsa2(17);
  rsa2.generate(23, 11);
  //rsa2.generate(863, 379);

  for (long i = 0; i < verticesF.rows(); ++i) {
    Eigen::Vector3f v = verticesF.row(i);
    Eigen::Vector3f ve;
    for (int j = 0; j < 3; ++j) {
      Chiffre32 chiffre = Chiffre32::fromFloat(v(j)).encrypteMantisseRSA(8, rsa2);
      chiffre.insereMessageLSB(4, 13);
      ve(j) = chiffre.toFloat();
    }
    verticesEncrypte.row(i) = ve;
  }

  igl::opengl::glfw::Viewer viewer;
  //viewer.data().set_mesh(verticesEncrypte.cast<double>(), faces);
  viewer.data().set_mesh(verticesEncrypte.cast<double>(), faces);
  viewer.launch();
   */

  /*
  RSA rsa(17);
  rsa.generate(11, 23);

  Chiffre32 a32 = Chiffre32::fromFloat(372.345239239f);
  Chiffre32 b32 = Chiffre32::fromFloat(1.8091024502f);
  Chiffre32 am32 = a32.mantisse();
  Chiffre32 bm32 = b32.mantisse();
  Chiffre32 c32 = a32.remplaceMantisse(bm32);

  Chiffre32 ac32 = a32.encrypteMantisseRSA(8, rsa);
  Chiffre32 ad32 = ac32.decrypteMantisseRSA(8, rsa);

  Chiffre32 ai32 = ac32.insereMessageLSB(3, 7);
  Chiffre32 ar32 = ai32.retireMessageLSB(3);
   */

  /*
  Paillier pailler(26);
  float a = 372.345239239f;

  Chiffre32 ca = Chiffre32::fromFloat(a);

  Chiffre32 ca_m = ca.mantisse();

  Chiffre64 ca_m_c = pailler.encrypte(ca_m);

  Chiffre32 ca_m_c_d = pailler.decrypte(ca_m_c);

  std::cout << "Chiffre  : " << ca << " : " << ca.toFloat() << std::endl
            << "Mantisse : " << ca_m << " : " << ca_m.toUint32() << std::endl
            << "Encrypt  : " << ca_m_c << " : " << ca_m_c.toUint64() << std::endl
            << "Decrypt  : " << ca_m_c_d << " : " << ca_m_c_d.toUint32() << std::endl;
            */

  return 0;
}