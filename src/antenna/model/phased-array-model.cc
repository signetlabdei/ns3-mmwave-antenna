
#include "phased-array-model.h"
#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/pointer.h"

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
    .AddAttribute ("AntennaElement",
               "A pointer to the antenna element used by the phased array",
               PointerValue (),
               MakePointerAccessor (&PhasedArrayModel::m_antennaElement),
               MakePointerChecker<AntennaModel> ())
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


PhasedArrayModel::ComplexVector
PhasedArrayModel::GetBeamformingVector() const
{
  NS_LOG_FUNCTION (this);
  return m_beamformingVector;
}


/*PhasedArrayModel::ComplexVector
PhasedArrayModel::GetBeamformingVector(const Angles& a) const
{
  NS_LOG_FUNCTION (this);
  
  //std::norm
  //std::conj  
  
  return m_beamformingVector;
}
*/


PhasedArrayModel::ComplexVector
PhasedArrayModel::GetSteeringVector(const Angles& a) const
{
  NS_LOG_FUNCTION (this << a);
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
PhasedArrayModel::SetAntennaElement (Ptr<AntennaModel> antennaElement)
{
  NS_LOG_FUNCTION (this);
  m_antennaElement = antennaElement;
}


Ptr<const AntennaModel>
PhasedArrayModel::GetAntennaElement() const
{
  NS_LOG_FUNCTION (this);
  return m_antennaElement;
}


} /* namespace ns3 */
 
