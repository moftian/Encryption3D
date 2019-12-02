#include <igl/readOFF.h>
#include <igl/readOBJ.h>
#include <igl/opengl/glfw/Viewer.h>
#include "encryption3D.hpp"

int main(int argc, char** argv)
{
  using namespace encrypt3d;

  RSA rsa(17);
  rsa.generate(11, 23);

  Chiffre a = Chiffre::fromFloat(372.345239239f);
  Chiffre b = Chiffre::fromFloat(1.8091024502f);
  Chiffre am = a.mantisse();
  Chiffre bm = b.mantisse();
  Chiffre c = a.remplaceMantisse(bm);

  Chiffre ac = a.encrypteMantisseRSA(8, rsa);
  Chiffre ad = ac.decrypteMantisseRSA(8, rsa);

  Chiffre ai = ac.insereMessageLSB(3, 7);
  Chiffre ar = ai.retireMessageLSB(3);

  std::cout << " a : " << a
//            << "am : " << am
//            << " b : " << b
//            << "bm : " << bm
//            << " c : " << c
            <<  "   : " << a.toFloat() << std::endl
            <<  "ac : " << ac
            <<  "   : " << ac.toFloat() << std::endl
            <<  "ad : " << ad
            <<  "   : " << ad.toFloat() << std::endl
            <<  "ai : " << ai
            <<  "   : " << ai.toFloat() << std::endl
            <<  "ar : " << ar
            <<  "   : " << ar.toFloat() << std::endl;

  Eigen::MatrixXd vertices;
  Eigen::MatrixXi faces;

  igl::readOBJ("../models/suzanne.obj", vertices, faces);

  Eigen::MatrixXf verticesEncrypte(vertices.rows(), vertices.cols());
  Eigen::MatrixXf verticesF = vertices.cast<float>();

  RSA rsa2(17);
  rsa2.generate(23, 11);

  for (long i = 0; i < verticesF.rows(); ++i) {
    Eigen::Vector3f v = verticesF.row(i);
    Eigen::Vector3f ve;
    for (int j = 0; j < 3; ++j) {
      ve(j) = Chiffre::fromFloat(v(j)).encrypteMantisseRSA(8, rsa2).toFloat();
    }
    verticesEncrypte.row(i) = ve;
  }

  igl::opengl::glfw::Viewer viewer;
  viewer.data().set_mesh(verticesEncrypte.cast<double>(), faces);
  viewer.launch();
}