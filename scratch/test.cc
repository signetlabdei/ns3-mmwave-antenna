/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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

#include "ns3/core-module.h"
#include "ns3/file-beamforming-codebook.h"
#include "ns3/beamforming-codebook.h"
#include "ns3/uniform-planar-array.h"
#include "ns3/string.h"
#include "ns3/pointer.h"
#include "ns3/uinteger.h"
#include <iostream>

using namespace ns3;
using namespace mmwave;

NS_LOG_COMPONENT_DEFINE ("CodebookScratch");

double GetGain (const PhasedArrayModel::ComplexVector &v1, const PhasedArrayModel::ComplexVector &v2)
{
  std::complex<double> prod {0, 0};
  for (uint32_t i = 0; i < v1.size (); i++)
  {
    prod += v1[i] * std::conj (v2[i]);
  }
  double gainDb = 20 * std::log10 (std::abs (prod));
  return gainDb;
}

int 
main (int argc, char *argv[])
{
  NS_LOG_UNCOND ("Testing bf codebook");

  Ptr<PhasedArrayModel> ant = CreateObjectWithAttributes<UniformPlanarArray> ("NumRows", UintegerValue (1),
                                                                              "NumColumns", UintegerValue (2));
  Ptr<BeamformingCodebook> cb = CreateObjectWithAttributes<FileBeamformingCodebook> ("Array", PointerValue (ant),
                                "CodebookFilename", StringValue ("src/mmwave/model/Codebooks/1x2.txt")
                                );
  cb->Initialize ();
  NS_LOG_UNCOND ("cb size: " << cb->GetCodebookSize ());

  std::ofstream gainFile;
  gainFile.open ("gain.csv");
  std::ofstream thetaFile;
  thetaFile.open ("theta.csv");
  std::ofstream phiFile;
  phiFile.open ("phi.csv");

  auto cw = cb->GetCodeword (0);
  for (double t = 0; t <= M_PI; t += M_PI / 256)
  {
    for (double p = -M_PI; p <= M_PI; p += M_PI / 256)
    {
      Angles a (p,t);
      auto sv = ant->GetSteeringVector (a);
      double gain = GetGain (cw, sv);

      gainFile << gain << ",";
      thetaFile << t << ",";
      phiFile << p << ",";
    }

    gainFile << std::endl;
    thetaFile << std::endl;
    phiFile << std::endl;
  }

  gainFile.close();
  thetaFile.close();
  phiFile.close();
}
