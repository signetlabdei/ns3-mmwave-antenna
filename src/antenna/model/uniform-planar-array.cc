
#include "uniform-planar-array.h"
#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("UniformPlanarArray");

NS_OBJECT_ENSURE_REGISTERED (UniformPlanarArray);



UniformPlanarArray::UniformPlanarArray () : PhasedArrayModel()
{

}

UniformPlanarArray::~UniformPlanarArray ()
{

}

TypeId
UniformPlanarArray::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UniformPlanarArray")
    .SetParent<Object> ()
    .AddConstructor<UniformPlanarArray> ()
    .SetGroupName("Antenna")
    .AddAttribute ("AntennaHorizontalSpacing",
               "Horizontal spacing between antenna elements, in multiples of wave length",
               DoubleValue (0.5),
               MakeDoubleAccessor (&UniformPlanarArray::m_disH),
               MakeDoubleChecker<double> ())
    .AddAttribute ("AntennaVerticalSpacing",
               "Vertical spacing between antenna elements, in multiples of wave length",
               DoubleValue (0.5),
               MakeDoubleAccessor (&UniformPlanarArray::m_disV),
               MakeDoubleChecker<double> ())
    .AddAttribute ("NumColumns",
               "Horizontal size of the array",
               UintegerValue (4),
               MakeUintegerAccessor (&UniformPlanarArray::m_numColumns),
               MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("NumRows",
               "Vertical size of the array",
               UintegerValue (4),
               MakeUintegerAccessor (&UniformPlanarArray::m_numRows),
               MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("BearingAngle",
               "The bearing angle in radians",
               DoubleValue (0.0),
               MakeDoubleAccessor (&UniformPlanarArray::m_alpha),
               MakeDoubleChecker<double> (-M_PI, M_PI))
    .AddAttribute ("DowntiltAngle",
               "The downtilt angle in radians",
               DoubleValue (0.0),
               MakeDoubleAccessor (&UniformPlanarArray::m_beta),
               MakeDoubleChecker<double> (0, M_PI))
  ;
  return tid;
}


std::pair<double, double>
UniformPlanarArray::GetElementFieldPattern (Angles a) const
{
  NS_LOG_FUNCTION (this);


  // normalize phi (if needed)
  a.NormalizeAngles();

  NS_ASSERT_MSG (a.theta >= 0 && a.theta <= M_PI, "The vertical angle should be between 0 and M_PI");
  NS_ASSERT_MSG (a.phi >= -M_PI && a.phi <= M_PI, "The horizontal angle should be between -M_PI and M_PI");

  // convert the theta and phi angles from GCS to LCS using eq. 7.1-7 and 7.1-8 in 3GPP TR 38.901
  // NOTE we assume a fixed slant angle of 0 degrees
  double thetaPrime = std::acos (cos (m_beta)*cos (a.theta) + sin (m_beta)*cos (a.phi-m_alpha)*sin (a.theta));
  double phiPrime = std::arg (std::complex<double> (cos (m_beta)*sin (a.theta)*cos (a.phi-m_alpha) - sin (m_beta)*cos (a.theta), sin (a.phi-m_alpha)*sin (a.theta)));
  Angles aPrime (phiPrime, thetaPrime);
  NS_LOG_DEBUG (a << " -> " << aPrime);

   // compute the antenna element field pattern in the vertical polarization using
  // eq. 7.3-4 in 3GPP TR 38.901
  // NOTE we assume vertical polarization, hence the field pattern in the
  // horizontal polarization is 0
  
  double aPrimeDb = m_antennaElement->GetGainDb (aPrime);
  double fieldThetaPrime = pow (10, aPrimeDb / 20); // convert to linear magnitude

  // compute psi using eq. 7.1-15 in 3GPP TR 38.901, assuming that the slant
  // angle (gamma) is 0
  double psi = std::arg (std::complex<double> (cos (m_beta) * sin (a.theta) - sin (m_beta) * cos (a.theta)* cos (a.phi - m_alpha), sin (m_beta)* sin (a.phi-m_alpha)));
  NS_LOG_DEBUG ("psi " << psi);


  // convert the antenna element field pattern to GCS using eq. 7.1-11
  // in 3GPP TR 38.901
  double fieldTheta = cos (psi) * fieldThetaPrime;
  double fieldPhi = sin (psi) * fieldThetaPrime;
  NS_LOG_DEBUG (RadiansToDegrees (a.phi) << " " << RadiansToDegrees (a.theta) << " " << fieldTheta*fieldTheta + fieldPhi*fieldPhi);

  return std::make_pair (fieldPhi, fieldTheta);
}



Vector
UniformPlanarArray::GetElementLocation (uint64_t index) const
{
  NS_LOG_FUNCTION (this);

  // compute the element coordinates in the LCS
  // assume the left bottom corner is (0,0,0), and the rectangular antenna array is on the y-z plane.
  double xPrime = 0;
  double yPrime = m_disH * (index % m_numColumns);
  double zPrime = m_disV * floor (index / m_numColumns);

  // convert the coordinates to the GCS using the rotation matrix 7.1-4 in 3GPP
  // TR 38.901
  Vector loc;
  loc.x = cos(m_alpha)*cos (m_beta)*xPrime - sin (m_alpha)*yPrime + cos (m_alpha)*sin (m_beta)*zPrime;
  loc.y = sin (m_alpha)*cos(m_beta)*xPrime + cos (m_alpha)*yPrime + sin (m_alpha)*sin (m_beta)*zPrime;
  loc.z = -sin (m_beta)*xPrime+cos(m_beta)*zPrime;
  return loc;
}

uint64_t
UniformPlanarArray::GetNumberOfElements () const
{
  return m_numRows*m_numColumns;
}

} /* namespace ns3 */
