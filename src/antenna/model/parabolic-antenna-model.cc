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
    .AddAttribute ("Beamwidth",
                   "The 3dB beamwidth (degrees)",
                   DoubleValue (60),
                   MakeDoubleAccessor (&ParabolicAntennaModel::SetBeamwidth,
                                       &ParabolicAntennaModel::GetBeamwidth),
                   MakeDoubleChecker<double> (0, 180))
    .AddAttribute ("Orientation",
                   "The angle (degrees) that expresses the orientation of the antenna on the x-y plane relative to the x axis",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&ParabolicAntennaModel::SetOrientation,
                                       &ParabolicAntennaModel::GetOrientation),
                   MakeDoubleChecker<double> (-360, 360))
    .AddAttribute ("MaxAttenuation",
                   "The maximum attenuation (dB) of the antenna radiation pattern.",
                   DoubleValue (30.0),
                   MakeDoubleAccessor (&ParabolicAntennaModel::m_maxAttenuation),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaxDirectionalGain",
                   "The maximum gain (dB) of the antenna radiation pattern.",
                   DoubleValue (8.0),
                   MakeDoubleAccessor (&ParabolicAntennaModel::m_gEmax),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

void 
ParabolicAntennaModel::SetBeamwidth (double beamwidthDegrees)
{ 
  NS_LOG_FUNCTION (this << beamwidthDegrees);
  m_beamwidthRadians = DegreesToRadians (beamwidthDegrees);
}

double
ParabolicAntennaModel::GetBeamwidth () const
{
  return RadiansToDegrees (m_beamwidthRadians);
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

double 
ParabolicAntennaModel::GetGainDb (Angles a)
{
  NS_LOG_FUNCTION (this << a);
  // azimuth angle w.r.t. the reference system of the antenna
  double phi = a.phi - m_orientationRadians;

  // make sure phi is in (-pi, pi]
  a.NormalizeAngles();

  NS_LOG_LOGIC ("phi = " << phi << " + theta = " << a.theta);
  
  double phiDeg = RadiansToDegrees(phi);
  double thetaDeg = RadiansToDegrees(a.theta);
  
  // compute the radiation power pattern using equations in table 7.3-1 in
  // 3GPP TR 38.901
  double A_M = 30; // front-back ratio expressed in dB
  double SLA = 30; // side-lobe level limit expressed in dB

  double A_v = -1 * std::min (SLA,12 * pow ((thetaDeg - 90) / 65,2)); // vertical cut of the radiation power pattern (dB)
  double A_h = -1 * std::min (A_M,12 * pow (phiDeg / 65,2)); // horizontal cut of the radiation power pattern (dB)

  double gainDb = m_gEmax - 1 * std::min (A_M,- A_v - A_h); // 3D radiation power pattern (dB)
  
  NS_LOG_LOGIC ("gain = " << gainDb);
  return gainDb;
 
}


}

