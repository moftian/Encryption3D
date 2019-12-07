#ifndef MINIPROJET_ENCRYPTION3D_HPP
#define MINIPROJET_ENCRYPTION3D_HPP

#include <Eigen/Core>

namespace encrypt3d {

  struct Encryption3D {

    /// Insert message in encrypted mesh RSA.
    /// \param msg     [in]
    /// \param inMesh  [out]
    /// \param outMesh [out]
    static void insereMsgRSA(const char *msg, int len, const Eigen::MatrixXf &inMesh, Eigen::MatrixXf &outMesh);

    ///
    /// \param msg
    /// \param inMesh
    /// \param outMesh
    static void insereMsgPaillier1(const char *msg, int len, const Eigen::MatrixXf &inMesh, Eigen::MatrixXf &outMesh);

    ///
    /// \param msg
    /// \param inMesh
    /// \param outMesh
    static void insereMsgPaillier2(const char *msg, int len, int blockSize, const Eigen::MatrixXf &inMesh, Eigen::MatrixXf &outMesh);
  };

}

#endif //MINIPROJET_ENCRYPTION3D_HPP
