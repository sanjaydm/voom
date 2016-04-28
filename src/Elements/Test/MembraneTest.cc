#include "C0Membrane.h"
#include "VoomMath.h"
#include "TriangleQuadrature.h"
#include "ShapeTri3.h"
#include "EvansElastic_Skewed.h"

using namespace voom;
using namespace std;

int main() {
  
  std::vector<Vector3D> X(3);
  X[0] = -4.04505, 0.754686, 20.0; // 0.0, 0.0, 0.0;
  X[1] = -3.03114, 1.004260, 20.0; // 2.0, 0.0, 0.0;
  X[2] =  3.13147, 0.119671, 20.0; // 0.0, 2.0, 0.0;

  std::vector<Vector3D> u(3);
  u[0] = 0.02146030, -0.00400383, -0.4233000;
  u[1] = 0.00967376, -0.00320540, -0.2549090;
  u[2] = 0.00962490, -0.000367821, -0.245511;

  srand(time(0));

  double mu = 1.2;
  double kS = 2.1 ;

  // typedef EvansElastic TestMaterial;
  // TestMaterial stretch(0.,0.,0.,mu,kS);

  typedef EvansElastic_Skewed TestMaterial;
  TestMaterial stretch(0.0, 0.0, 0.0, mu, kS, 1.0, 45.0, 0.0, 0.0);

  typedef C0Membrane<TriangleQuadrature, TestMaterial,  ShapeTri3> MemEl;
  MemEl::NodeContainer dnodes;

  int dof = 0;
  for(int id = 0; id < 3; id++)
  {
    Vector3D  x( X[id] );
    // perturb 
    x+=u[id];

    NodeBase::DofIndexMap idx(3);
    for(int j = 0; j < 3; j++) idx[j] = dof++;
    dnodes.push_back( new DeformationNode<3>(id,idx,X[id],x) );
  }
 
  TriangleQuadrature quad(2);
  NodeBase::DofIndexMap idxM(1);
  idxM[0]=-1;
  MultiplierNode * pressureNode = new MultiplierNode(dnodes.size(), idxM, 0.0 );
  MultiplierNode * tensionNode = new MultiplierNode(dnodes.size(), idxM, 0.0 );

  MemEl* elem = new MemEl(quad, stretch, dnodes, pressureNode, tensionNode);

  elem->checkConsistency();

  return 0;

}