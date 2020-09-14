/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 CTTC
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
 *
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */


#include <ns3/log.h>
#include <ns3/double.h>
#include <cmath>

#include "antenna-model.h"
#include "parabolic-antenna-model.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("ParabolicAntennaModel");

NS_OBJECT_ENSURE_REGISTERED (ParabolicAntennaModel);


TypeId 
ParabolicAntennaModel::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ParabolicAntennaModel")
    .SetParent<AntennaModel> ()
    .SetGroupName("Antenna")
    .AddConstructor<ParabolicAntennaModel> ()
    .AddAttribute ("VerticalBeamwidth",
                   "The 3dB vertical beamwidth (degrees)",
                   DoubleValue (65),
                   MakeDoubleAccessor (&ParabolicAntennaModel::SetVerticalBeamwidth,
                                       &ParabolicAntennaModel::GetVerticalBeamwidth),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("HorizontalBeamwidth",
                   "The 3dB horizontal beamwidth (degrees)",
                   DoubleValue (65),
                   MakeDoubleAccessor (&ParabolicAntennaModel::SetHorizontalBeamwidth,
                                       &ParabolicAntennaModel::GetHorizontalBeamwidth),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("Orientation",
                   "The angle (degrees) that expresses the orientation of the antenna on the x-y plane relative to the x axis",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&ParabolicAntennaModel::SetOrientation,
                                       &ParabolicAntennaModel::GetOrientation),
                   MakeDoubleChecker<double> (-360, 360))
    .AddAttribute ("MaxAttenuation",
                   "The maximum attenuation (dB) of the antenna radiation pattern.",
                   DoubleValue (30.0),
                   MakeDoubleAccessor (&ParabolicAntennaModel::SetMaxAttenuation,
                                       &ParabolicAntennaModel::GetMaxAttenuation),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("VerticalSideLobeAttenuation",
                   "The attenuation (dB) of the side lobe in the vertical direction",
                   DoubleValue (30.0),
                   MakeDoubleAccessor (&ParabolicAntennaModel::SetSlaV,
                                       &ParabolicAntennaModel::GetSlaV),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaxDirectionalGain",
                   "The maximum gain (dB) of the antenna radiation pattern.",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&ParabolicAntennaModel::m_gEmax),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

void 
ParabolicAntennaModel::SetVerticalBeamwidth (double verticalBeamwidthDegrees)
{ 
  NS_ASSERT_MSG (verticalBeamwidthDegrees > 0, "Beamwidth must be positive");
    
  NS_LOG_FUNCTION (this << verticalBeamwidthDegrees);
  
  m_verticalBeamwidthDegrees = verticalBeamwidthDegrees;
}


double
ParabolicAntennaModel::GetVerticalBeamwidth () const
{
  return m_verticalBeamwidthDegrees;
}


void 
ParabolicAntennaModel::SetHorizontalBeamwidth (double horizontalBeamwidthDegrees)
{ 
  NS_ASSERT_MSG (horizontalBeamwidthDegrees > 0, "Beamwidth must be positive");
    
  NS_LOG_FUNCTION (this << horizontalBeamwidthDegrees);
  
  m_horizontalBeamwidthDegrees = horizontalBeamwidthDegrees;
}


double
ParabolicAntennaModel::GetHorizontalBeamwidth () const
{
  return m_horizontalBeamwidthDegrees;
}


void 
ParabolicAntennaModel::SetOrientation (double orientationDegrees)
{
  NS_LOG_FUNCTION (this << orientationDegrees);
  m_orientationRadians = DegreesToRadians (orientationDegrees);
}


double
ParabolicAntennaModel::GetOrientation () const
{
  return RadiansToDegrees (m_orientationRadians);
}


void
ParabolicAntennaModel::SetSlaV (double SlaV)
{
  NS_LOG_FUNCTION (this << SlaV);
  m_SlaV = SlaV;
}


double
ParabolicAntennaModel::GetSlaV () const
{
  return m_SlaV;
}


void
ParabolicAntennaModel::SetMaxAttenuation (double maxAttenuation)
{
  NS_LOG_FUNCTION (this << maxAttenuation);
  m_maxAttenuation = maxAttenuation;
}


double
ParabolicAntennaModel::GetMaxAttenuation () const
{
  return m_maxAttenuation;    
}


double 
ParabolicAntennaModel::GetGainDb (Angles a)
{
  NS_LOG_FUNCTION (this << a);
 
  // make sure phi is in (-pi, pi]
  a.phi -= m_orientationRadians;
  a.NormalizeAngles();

  NS_LOG_LOGIC ("phi = " << a.phi << " + theta = " << a.theta);
  
  double phiDeg = RadiansToDegrees(a.phi);
  double thetaDeg = RadiansToDegrees(a.theta);

  // compute the radiation power pattern using equations in table 7.3-1 in
  // 3GPP TR 38.901
  double A_v = -1 * std::min (m_SlaV,12 * pow ((thetaDeg - 90) / m_verticalBeamwidthDegrees,2)); // vertical cut of the radiation power pattern (dB)
  double A_h = -1 * std::min (m_maxAttenuation,12 * pow (phiDeg / m_horizontalBeamwidthDegrees,2)); // horizontal cut of the radiation power pattern (dB)

  double gainDb = m_gEmax - 1 * std::min (m_maxAttenuation,- A_v - A_h); // 3D radiation power pattern (dB)
  
  NS_LOG_LOGIC ("gain = " << gainDb);
  return gainDb;
 
}


}

