
#include "phased-array-model.h"
#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("UniformPlanarArray");

NS_OBJECT_ENSURE_REGISTERED (UniformPlanarArray);


TypeId
UniformPlanarArray::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UniformPlanarArray")
    .SetParent<Object> ()
    .AddConstructor<UniformPlanarArray> ()
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
    .AddAttribute ("ElementGain",
               "Directional gain of an antenna element in dBi",
               DoubleValue (4.97),
               MakeDoubleAccessor (&UniformPlanarArray::m_gE),
               MakeDoubleChecker<double> (0, 8))
  ;
  return tid;
}

bool
UniformPlanarArray::IsOmniTx () const
{
  NS_LOG_FUNCTION (this);
  return m_isOmniTx;
}

void
UniformPlanarArray::ChangeToOmniTx ()
{
  NS_LOG_FUNCTION (this);
  m_isOmniTx = true;
}

void
UniformPlanarArray::SetBeamformingVector (const ComplexVector &beamformingVector)
{
  NS_LOG_FUNCTION (this);
  m_isOmniTx = false;
  m_beamformingVector = beamformingVector;
}

//SILVIA: considering radius equal to 1
//SILVIA: the vector component calculated with (7.1-6), given the desired angle
void
UniformPlanarArray::SetBeamformingVector (const Angles a)
{
  NS_LOG_FUNCTION (this);
  m_isOmniTx = false;
  ComplexVector beamformingVector = (sin(a.theta) * cos(a.phi) , sin(a.theta)*sin(a.phi), cos(a.theta));  
  m_beamformingVector = beamformingVector;
}

const UniformPlanarArray::ComplexVector &
UniformPlanarArray::GetBeamformingVector() const
{
  NS_LOG_FUNCTION (this);
  return m_beamformingVector;
}

//SILVIA: the vector component calculated with (7.1-6), given the desired angle
const UniformPlanarArray::ComplexVector &
UniformPlanarArray::GetSteeringVector(const Angles a) const
{
  NS_LOG_FUNCTION (this);
  ComplexVector steeringVector = (sin(a.theta) * cos(a.phi) , sin(a.theta)*sin(a.phi), cos(a.theta));  
  return steeringVector;
}

//SILVIA
void
UniformPlanarArray::SetAntennaElement (const AntennaModel &antennaElement)
{
  NS_LOG_FUNCTION (this);
  m_antennaElement = antennaElement;
}

//SILVIA
const UniformPlanarArray::AntennaModel &
UniformPlanarArray::GetAntennaElement() const
{
  NS_LOG_FUNCTION (this);
  return m_antennaElement;
}



std::pair<double, double>
UniformPlanarArray::GetElementFieldPattern (Angles a) const
{
  NS_LOG_FUNCTION (this);

  // normalize phi (if needed)
  a.phi = fmod (a.phi + M_PI, 2 * M_PI);
  if (a.phi < 0)
      a.phi += M_PI;
  else 
      a.phi -= M_PI;

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
  double aPrimeDb = m_antennaElement.GetGainDb (aPrime);
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

double
UniformPlanarArray::GetRadiationPattern (double thetaRadian, double phiRadian) const
{
  if (m_isIsotropic)
  {
    return 0;
  }

  // convert the angles in degrees
  double thetaDeg = thetaRadian * 180 / M_PI;
  double phiDeg = phiRadian * 180 / M_PI;
  NS_ASSERT_MSG (thetaDeg >= 0 && thetaDeg <= 180, "the vertical angle should be the range of [0,180]");
  NS_ASSERT_MSG (phiDeg >= -180 && phiDeg <= 180, "the horizontal angle should be the range of [-180,180]");

  // compute the radiation power pattern using equations in table 7.3-1 in
  // 3GPP TR 38.901
  double A_M = 30; // front-back ratio expressed in dB
  double SLA = 30; // side-lobe level limit expressed in dB

  double A_v = -1 * std::min (SLA,12 * pow ((thetaDeg - 90) / 65,2)); // vertical cut of the radiation power pattern (dB)
  double A_h = -1 * std::min (A_M,12 * pow (phiDeg / 65,2)); // horizontal cut of the radiation power pattern (dB)

  double A = m_gE - 1 * std::min (A_M,- A_v - A_h); // 3D radiation power pattern (dB)

  return A; // 3D radiation power pattern in dB
}

Vector
UniformPlanarArray::GetElementLocation (uint64_t row, uint64_t col) const
{
  NS_LOG_FUNCTION (this);

  // compute the element coordinates in the LCS
  // assume the left bottom corner is (0,0,0), and the rectangular antenna array is on the y-z plane.
  double xPrime = 0;
  double yPrime = m_disH * col;
  double zPrime = m_disV * row;

  // convert the coordinates to the GCS using the rotation matrix 7.1-4 in 3GPP
  // TR 38.901
  Vector loc;
  loc.x = cos(m_alpha)*cos (m_beta)*xPrime - sin (m_alpha)*yPrime + cos (m_alpha)*sin (m_beta)*zPrime;
  loc.y = sin (m_alpha)*cos(m_beta)*xPrime + cos (m_alpha)*yPrime + sin (m_alpha)*sin (m_beta)*zPrime;
  loc.z = -sin (m_beta)*xPrime+cos(m_beta)*zPrime;
  return loc;
}

uint64_t
UniformPlanarArray::GetNumberOfElements (void) const
{
  NS_LOG_FUNCTION (this);
  return m_numRows*m_numColumns;
}

} /* namespace ns3 */
 
