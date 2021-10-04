#include "ns3/uniform-planar-array.h"
#include "ns3/parabolic-antenna-model.h"
#include <fstream>
#include <numeric>
#include "ns3/core-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("UPASimulator");

int 
main (int argc, char *argv[])
{
  NS_LOG_UNCOND ("Uniform Planar Array - Simulator");

  std::ofstream myfile;
  myfile.open ("simulation_results.csv");
  myfile << "theta_rad,phi_rad,gain_db\n";
  
  /*
    - 1x1 isotropic
    - 1x1 cosine
    - 1x1 parabolic
    - 1x8 parabolic
    - 8x1 parabolic
    - 8x8 parabolic
    - 8x8 parabolic con vertical spacing 0.8 (e horizontal spacing di default, i.e., 0.5)
    Come beamforming angles puoi provare, oltre a quello di default (0, pi/2):
    - (pi/4, pi/2)
    - (pi/2, pi/2)
    - (0, pi/4)
    - (0, 0)
  */
  
  //Build the scenario for the test
  uint8_t txAntennaElements[] {1, 1}; // tx antenna dimensions
  
  // Create the antenna and set the dimensions
  Ptr<PhasedArrayModel> txAntenna = CreateObjectWithAttributes<UniformPlanarArray> ("NumColumns", UintegerValue (txAntennaElements [0]), "NumRows", UintegerValue (txAntennaElements [1]));
  
  // Create and set AntennaElement
  Ptr<AntennaModel> txAntennaModel = CreateObject<ParabolicAntennaModel> ();
  txAntenna->SetAntennaElement(txAntennaModel);
  
  // Get the beamforming vector related to that angle pair
  PhasedArrayModel::ComplexVector bf = txAntenna->GetBeamformingVector (Angles (0, M_PI/2));
  
  // angles resolution
  int res = 100;
  
  for (double phi = -M_PI; phi < M_PI; phi = phi + (2*M_PI/res))
  {
    for (double theta = 0; theta < M_PI; theta = theta + (2*M_PI/res))
    {
      Angles a = Angles(phi, theta);
      PhasedArrayModel::ComplexVector sv = txAntenna->GetSteeringVector(a);
       std::pair<double, double> efp = txAntenna->GetElementFieldPattern(a);
       double efp_norm = std::sqrt(std::pow(std::get<0>(efp),2) + std::pow(std::get<1>(efp),2));
       double af = std::abs(sv[0]*bf[0]);
       double gain =  af * efp_norm;
       double gain_db = 20 * std::log10(gain);
      myfile << theta << "," << phi << "," << gain_db << "\n";
    }  
  }
  
  myfile.close();
  
} 
