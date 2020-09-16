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

#ifndef THREE_GPP_ANTENNA_MODEL_H
#define THREE_GPP_ANTENNA_MODEL_H


#include <ns3/object.h>
#include <ns3/antenna-model.h>

namespace ns3 {

/**
 *
 * \brief  Antenna model based on a parabolic approximation of the main lobe radiation pattern.
 *
 * This class implements the parabolic model as described in 3GPP TR 38.901 v15.0.0
 *
 */
class ThreeGppAntennaModel : public AntennaModel
{
public:
  // inherited from Object
  static TypeId GetTypeId ();

  // inherited from AntennaModel
  virtual double GetGainDb (Angles a);

  // attribute getters/setters
  double GetVerticalBeamwidth () const;
  double GetHorizontalBeamwidth () const;
  double GetSlaV () const;
  double GetMaxAttenuation () const;

  void SetOrientation (double orientationDegrees);
  double GetOrientation () const;

private:
  double m_verticalBeamwidthDegrees;
  double m_horizontalBeamwidthDegrees;
  double m_orientationRadians;
  double m_maxAttenuation;
  double m_SlaV;
  double m_gEmax;
};



} // namespace ns3


#endif // THREE_GPP_ANTENNA_MODEL_H
