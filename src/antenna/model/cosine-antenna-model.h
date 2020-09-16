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

#ifndef COSINE_ANTENNA_MODEL_H
#define COSINE_ANTENNA_MODEL_H


#include <ns3/object.h>
#include <ns3/antenna-model.h>

namespace ns3 {

/**
 * 
 * \brief Cosine Antenna Model 
 *
 * This class implements the cosine model as described in:
 *
 * Cosine Antenna Element, Mathworks, Phased Array System Toolbox (Sep. 2020)
 * Available online: https://www.mathworks.com/help/phased/ug/cosine-antenna-element.html
 *
 * Note: an extra settable gain is added to the original model, to improve its generality.
 */
class CosineAntennaModel : public AntennaModel
{
public:

  // inherited from Object
  static TypeId GetTypeId ();

  // inherited from AntennaModel
  virtual double GetGainDb (Angles a);


  // attribute getters/setters
  void SetVerticalBeamwidth (double verticalBeamwidthDegrees);
  void SetHorizontalBeamwidth (double horizontalBeamwidthDegrees);
  double GetVerticalBeamwidth (void) const;
  double GetHorizontalBeamwidth (void) const;
  void SetOrientation (double orientationDegrees);
  double GetOrientation (void) const;

private:

  static double GetExponentFromBeamwidth(double beamwidthRadians);
  
  double m_verticalexponent; 

  double m_horizontalexponent; 
  
  double m_verticalBeamwidthRadians;
  
  double m_horizontalBeamwidthRadians;

  double m_orientationRadians;

  double m_maxGain;
};



} // namespace ns3


#endif // COSINE_ANTENNA_MODEL_H
