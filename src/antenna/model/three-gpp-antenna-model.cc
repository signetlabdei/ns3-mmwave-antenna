/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2020 University of Padova, Dep. of Information Engineering, SIGNET lab.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include <ns3/log.h>
#include <ns3/double.h>
#include <cmath>

#include "antenna-model.h"
#include "three-gpp-antenna-model.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("ThreeGppAntennaModel");

NS_OBJECT_ENSURE_REGISTERED (ThreeGppAntennaModel);


TypeId
ThreeGppAntennaModel::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ThreeGppAntennaModel")
    .SetParent<AntennaModel> ()
    .SetGroupName ("Antenna")
    .AddConstructor<ThreeGppAntennaModel> ()
    .AddAttribute ("VerticalBeamwidth",
                   "The 3dB vertical beamwidth (degrees)",
                   DoubleValue (65),
                   MakeDoubleAccessor (&ThreeGppAntennaModel::m_verticalBeamwidthDegrees),
                   MakeDoubleChecker<double> (0.0))
    .AddAttribute ("HorizontalBeamwidth",
                   "The 3dB horizontal beamwidth (degrees)",
                   DoubleValue (65),
                   MakeDoubleAccessor (&ThreeGppAntennaModel::m_horizontalBeamwidthDegrees),
                   MakeDoubleChecker<double> (0.0))
    .AddAttribute ("Orientation",
                   "The angle (degrees) that expresses the orientation of the antenna on the x-y plane relative to the x axis",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&ThreeGppAntennaModel::SetOrientation,
                                       &ThreeGppAntennaModel::GetOrientation),
                   MakeDoubleChecker<double> (-360, 360))
    .AddAttribute ("MaxAttenuation",
                   "The maximum attenuation (dB) of the antenna radiation pattern.",
                   DoubleValue (30.0),
                   MakeDoubleAccessor (&ThreeGppAntennaModel::m_maxAttenuation),
                   MakeDoubleChecker<double> (0.0))
    .AddAttribute ("VerticalSideLobeAttenuation",
                   "The attenuation (dB) of the side lobe in the vertical direction",
                   DoubleValue (30.0),
                   MakeDoubleAccessor (&ThreeGppAntennaModel::m_SlaV),
                   MakeDoubleChecker<double> (0.0))
    .AddAttribute ("MaxDirectionalGain",
                   "The maximum gain (dB) of the antenna radiation pattern.",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&ThreeGppAntennaModel::m_gEmax),
                   MakeDoubleChecker<double> (0.0))
  ;
  return tid;
}


double
ThreeGppAntennaModel::GetVerticalBeamwidth () const
{
  return m_verticalBeamwidthDegrees;
}


double
ThreeGppAntennaModel::GetHorizontalBeamwidth () const
{
  return m_horizontalBeamwidthDegrees;
}


void
ThreeGppAntennaModel::SetOrientation (double orientationDegrees)
{
  NS_LOG_FUNCTION (this << orientationDegrees);
  m_orientationRadians = DegreesToRadians (orientationDegrees);
}


double
ThreeGppAntennaModel::GetOrientation () const
{
  return RadiansToDegrees (m_orientationRadians);
}


double
ThreeGppAntennaModel::GetSlaV () const
{
  return m_SlaV;
}


double
ThreeGppAntennaModel::GetMaxAttenuation () const
{
  return m_maxAttenuation;
}


double
ThreeGppAntennaModel::GetGainDb (Angles a)
{
  NS_LOG_FUNCTION (this << a);

  // make sure phi is in (-pi, pi]
  a.phi -= m_orientationRadians;
  a.NormalizeAngles ();

  NS_LOG_LOGIC ("phi = " << a.phi << " + theta = " << a.theta);

  double phiDeg = RadiansToDegrees (a.phi);
  double thetaDeg = RadiansToDegrees (a.theta);

  // compute the radiation power pattern using equations in table 7.3-1 in
  // 3GPP TR 38.901
  double A_v = -1 * std::min (m_SlaV,12 * pow ((thetaDeg - 90) / m_verticalBeamwidthDegrees, 2)); // vertical cut of the radiation power pattern (dB)
  double A_h = -1 * std::min (m_maxAttenuation,12 * pow (phiDeg / m_horizontalBeamwidthDegrees, 2)); // horizontal cut of the radiation power pattern (dB)

  double gainDb = m_gEmax - 1 * std::min (m_maxAttenuation,-A_v - A_h);  // 3D radiation power pattern (dB)

  NS_LOG_LOGIC ("gain = " << gainDb);
  return gainDb;

}


}

