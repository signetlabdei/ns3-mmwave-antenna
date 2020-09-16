/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 CTTC
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
#include "cosine-antenna-model.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("CosineAntennaModel");

NS_OBJECT_ENSURE_REGISTERED (CosineAntennaModel);


TypeId
CosineAntennaModel::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::CosineAntennaModel")
    .SetParent<AntennaModel> ()
    .SetGroupName ("Antenna")
    .AddConstructor<CosineAntennaModel> ()
    .AddAttribute ("VerticalBeamwidth",
                   "The 3dB vertical beamwidth (degrees)",
                   DoubleValue (std::numeric_limits<double>::infinity ()),
                   MakeDoubleAccessor (&CosineAntennaModel::SetVerticalBeamwidth,
                                       &CosineAntennaModel::GetVerticalBeamwidth),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("HorizontalBeamwidth",
                   "The 3dB horizontal beamwidth (degrees)",
                   DoubleValue (60),
                   MakeDoubleAccessor (&CosineAntennaModel::SetHorizontalBeamwidth,
                                       &CosineAntennaModel::GetHorizontalBeamwidth),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("Orientation",
                   "The angle (degrees) that expresses the orientation of the antenna on the x-y plane relative to the x axis",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&CosineAntennaModel::SetOrientation,
                                       &CosineAntennaModel::GetOrientation),
                   MakeDoubleChecker<double> (-360, 360))
    .AddAttribute ("MaxGain",
                   "The gain (dB) at the antenna boresight (the direction of maximum gain)",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&CosineAntennaModel::m_maxGain),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}


double
CosineAntennaModel::GetExponentFromBeamwidth (double beamwidthRadians)
{
  double exponent = 0;
  if (!(std::isinf (beamwidthRadians)))
    {
      exponent = -3.0 / (20 * std::log10 (std::cos (beamwidthRadians / 4.0)));
    }
  return exponent;
}


void
CosineAntennaModel::SetVerticalBeamwidth (double verticalBeamwidthDegrees)
{
  NS_LOG_FUNCTION (this << verticalBeamwidthDegrees);
  NS_ASSERT_MSG (verticalBeamwidthDegrees > 0, "Beamwidth must be positive");

  if (std::isinf (verticalBeamwidthDegrees))
    {
      m_verticalBeamwidthRadians = std::numeric_limits<double>::infinity ();
    }
  else
    {
      m_verticalBeamwidthRadians = DegreesToRadians (verticalBeamwidthDegrees);
    }

  m_verticalexponent = GetExponentFromBeamwidth (m_verticalBeamwidthRadians);
}


void
CosineAntennaModel::SetHorizontalBeamwidth (double horizontalBeamwidthDegrees)
{
  NS_LOG_FUNCTION (this << horizontalBeamwidthDegrees);
  NS_ASSERT_MSG (horizontalBeamwidthDegrees > 0, "Beamwidth must be positive");

  if (std::isinf (horizontalBeamwidthDegrees))
    {
      m_horizontalBeamwidthRadians = std::numeric_limits<double>::infinity ();
    }
  else
    {
      m_horizontalBeamwidthRadians = DegreesToRadians (horizontalBeamwidthDegrees);
    }

  m_horizontalexponent = GetExponentFromBeamwidth (m_horizontalBeamwidthRadians);
}


double
CosineAntennaModel::GetVerticalBeamwidth () const
{
  return RadiansToDegrees (m_verticalBeamwidthRadians);
}


double
CosineAntennaModel::GetHorizontalBeamwidth () const
{
  return RadiansToDegrees (m_horizontalBeamwidthRadians);
}


void
CosineAntennaModel::SetOrientation (double orientationDegrees)
{
  NS_LOG_FUNCTION (this << orientationDegrees);
  m_orientationRadians = DegreesToRadians (orientationDegrees);
}


double
CosineAntennaModel::GetOrientation () const
{
  return RadiansToDegrees (m_orientationRadians);
}


double
CosineAntennaModel::GetGainDb (Angles a)
{
  NS_LOG_FUNCTION (this << a);

  // make sure phi is in (-pi, pi]
  a.phi -= m_orientationRadians;
  a.NormalizeAngles ();

  NS_LOG_LOGIC (a);

  // element factor: amplitude gain of a single antenna element in linear units
  double ef = (std::pow (std::cos (a.phi / 2.0), m_horizontalexponent)) *
    (std::pow (std::cos ((a.theta - M_PI / 2) / 2.0), m_verticalexponent));

  // the array factor is not considered. Note that if we did consider
  // the array factor, the actual beamwidth would change, and in
  // particular it would be different from the one specified by the
  // user. Hence it is not desirable to use the array factor, for the
  // ease of use of this model.

  double gainDb = 20 * std::log10 (ef);
  NS_LOG_LOGIC ("gain = " << gainDb << " + " << m_maxGain << " dB");
  return gainDb + m_maxGain;
}


}
