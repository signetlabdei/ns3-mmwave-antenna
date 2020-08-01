
#include "phased-array-model.h"
#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("PhasedArrayModel");

NS_OBJECT_ENSURE_REGISTERED (PhasedArrayModel);

PhasedArrayModel::PhasedArrayModel ()
{
    
}

PhasedArrayModel::~PhasedArrayModel ()
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


} /* namespace ns3 */
 
