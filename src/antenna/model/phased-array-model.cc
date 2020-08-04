
#include "phased-array-model.h"
#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("PhasedArrayModel");

NS_OBJECT_ENSURE_REGISTERED (PhasedArrayModel);


PhasedArrayModel::PhasedArrayModel()
{
}


PhasedArrayModel::~PhasedArrayModel()
{
    
}


TypeId
PhasedArrayModel::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::PhasedArrayModel")
    .SetParent<Object> ()
    .SetGroupName("Antenna")
  ;
  return tid;
}


void
PhasedArrayModel::SetBeamformingVector (const ComplexVector &beamformingVector)
{
  NS_LOG_FUNCTION (this);
  m_beamformingVector = beamformingVector;
}


void
PhasedArrayModel::SetBeamformingVector (const Angles& a)
{
  NS_LOG_FUNCTION (this << a);
  m_beamformingVector = GetSteeringVector (a);
}


const PhasedArrayModel::ComplexVector &
PhasedArrayModel::GetBeamformingVector() const
{
  NS_LOG_FUNCTION (this);
  return m_beamformingVector;
}


// The vector components calculated with (7.1-6), given the desired angle
const PhasedArrayModel::ComplexVector
PhasedArrayModel::GetSteeringVector(const Angles& a) const
{
  NS_LOG_FUNCTION (this);
  ComplexVector steeringVector;
  steeringVector.resize (this->GetNumberOfElements ());
  for (uint64_t i = 0; i < this->GetNumberOfElements (); i++)
    {
      Vector loc = this->GetElementLocation (i);
      steeringVector[i] = -2 * M_PI * (sin (a.theta) * cos (a.phi) * loc.x +
                                       sin (a.theta) * sin (a.phi) * loc.y +
                                       cos (a.theta) * loc.z);
    }
  return steeringVector;
}


void
PhasedArrayModel::SetAntennaElement (const Ptr<AntennaModel> &antennaElement)
{
  NS_LOG_FUNCTION (this);
  m_antennaElement = antennaElement;
}


const Ptr<AntennaModel> &
PhasedArrayModel::GetAntennaElement() const
{
  NS_LOG_FUNCTION (this);
  return m_antennaElement;
}


} /* namespace ns3 */
 
