#ifndef UNIFORM_PLANAR_ARRAY_H
#define UNIFORM_PLANAR_ARRAY_H

#include <ns3/object.h>
#include "phased-array-model.h"

// SILVIA: Remember to remove all SILVIA comments, each SILVIA comments means something Silvia modified to keep track of what I modified myself (in case I forgot some)

namespace ns3 {
   
    
/**
 * \ingroup antenna
 *
 * \brief Class implementing the phased array model
 * 
 * \note the current implementation supports the modeling of antenna arrays 
 * composed of a single panel and with single (vertical) polarization.
 */
class UniformPlanarArray : public PhasedArrayModel
{
public:

  // inherited from Object
  static TypeId GetTypeId (void);
  
  
  // SILVIA: ALL THESE ARE INHERITED FROM PHASED ARRAY MODEL  
  /**
   * Returns the horizontal and vertical components of the antenna element field
   * pattern at the specified direction. Only vertical polarization is considered.
   * \param a the angle indicating the interested direction
   * \return a pair in which the first element is the horizontal component
   *         of the field pattern and the second element is the vertical
   *         component of the field pattern
   */
  virtual std::pair<double, double> GetElementFieldPattern (Angles a) const;

  
   // SILVIA: modified the numbering 
   /** 
   * Returns the location of the antenna element with the specified
   * index assuming the left bottom corner is (0,0,0), normalized
   * with respect to the wavelength.
   * Antenna elements are numbered as specified in Sec. 7.3
   * For example, an antenna with 2 rows and 3 columns will be ordered as follows:
   * ^ z
   * |  (1,0) (1,1) (1,2)
   * |  (0,0) (0,1) (0,2)
   * ---------------------> y
   *
   * \param index index of the antenna element
   * \return the 3D vector that represents the position of the element
   */
  virtual Vector GetElementLocation (uint64_t row, uint64_t col) const;

  /**
   * Returns the number of antenna elements
   * \return the number of antenna elements
   */
  virtual uint64_t GetNumberOfElements (void) const;

  /**
   * Sets the beamforming vector to be used
   * \param beamformingVector the beamforming vector
   */
  virtual void SetBeamformingVector (const ComplexVector &beamformingVector);
  
  //SILVIA
  /**
   * Sets the beamforming vector to point towards the specified position
   * \param beamformingVector the beamforming vector
   */
  virtual void SetBeamformingVector (const Angles a);

  /**
   * Returns the beamforming vector that is currently being used
   * \return the current beamforming vector
   */
  virtual const ComplexVector & GetBeamformingVector (void) const;
  
  //SILVIA
  /**
   * Returns the steering vector that points toward the specified position
   * \return the current beamforming vector
   */
  virtual const ComplexVector & GetSteeringVector (const Angles a) const;
  
  //SILVIA
  /**
   * Sets the antenna model to be used
   * \param AntennaModel the antenna model
   */
  virtual void SetAntennaElement (const AntennaModel &antennaElement);

  /**
   * Returns the beamforming vector that is currently being used
   * \return the current beamforming vector
   */
  virtual const AntennaModel & GetAntennaElement (void) const;

private:
  /**
   * Returns the radiation power pattern of a single antenna element in dB,
   * generated according to Table 7.3-1 in 3GPP TR 38.901
   * \param vAngleRadian the vertical angle in radians
   * \param hAngleRadian the horizontal angle in radians
   * \return the radiation power pattern in dB
   */
  double GetRadiationPattern (double vAngleRadian, double hAngleRadian) const;
    

  bool m_isOmniTx; //!< true if the antenna is configured for omni transmissions
  uint32_t m_numColumns; //!< number of columns
  uint32_t m_numRows; //!< number of rows
  double m_disV; //!< antenna spacing in the vertical direction in multiples of wave length
  double m_disH; //!< antenna spacing in the horizontal direction in multiples of wave length
  double m_alpha; //!< the bearing angle in radians
  double m_beta; //!< the downtilt angle in radians
  double m_gE; //!< directional gain of a single antenna element (dBi)
};

} /* namespace ns3 */

#endif /* UNIFORM_PLANAR_ARRAY_H */
