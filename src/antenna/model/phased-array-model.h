#ifndef PHASED_ARRAY_MODEL_H
#define PHASED_ARRAY_MODEL_H

#include <ns3/angles.h>
#include <complex>


namespace ns3 {


/**
 * \ingroup antenna
 *
 * \brief Class implementing the phased array model virtual base class.
 */
class PhasedArrayModel : public Object
{
public:

  /**
   * Constructor
   */
  virtual PhasedArrayModel (void);


  /**
   * Destructor
   */
  virtual ~PhasedArrayModel (void);


  // inherited from Object
  static TypeId GetTypeId (void);


  typedef std::vector<std::complex<double> > ComplexVector; //!< type definition for complex vectors


  /**
   * Returns the horizontal and vertical components of the antenna element field
   * pattern at the specified direction. Only vertical polarization is considered.
   * \param a the angle indicating the interested direction
   * \return a pair in which the first element is the horizontal component
   *         of the field pattern and the second element is the vertical
   *         component of the field pattern
   */
  virtual std::pair<double, double> GetElementFieldPattern (Angles a) const = 0;


  /**
   * Returns the location of the antenna element with the specified
   * index, normalized with respect to the wavelength.
   * \param idx index of the antenna element
   * \return the 3D vector that represents the position of the element
   */
  virtual Vector GetElementLocation (uint64_t row, uint64_t col) const = 0;


  /**
   * Returns the number of antenna elements
   * \return the number of antenna elements
   */
  virtual uint32_t GetNumberOfElements (void) const = 0;


  /**
   * Sets the beamforming vector to be used
   * \param beamformingVector the beamforming vector
   */
  void SetBeamformingVector (const ComplexVector &beamformingVector);


  /**
   * Sets the beamforming vector to point towards the specified position
   * \param beamformingVector the beamforming vector
   */
  void SetBeamformingVector (const Angles a);


  /**
   * Returns the beamforming vector that is currently being used
   * \return the current beamforming vector
   */
  const ComplexVector & GetBeamformingVector (void) const;


  /**
   * Returns the steering vector that points toward the specified position
   * \return the current beamforming vector
   */
  ComplexVector GetSteeringVector (const Angles a) const;


  /**
   * Sets the antenna model to be used
   * \param AntennaModel the antenna model
   */
  void SetAntennaElement (const AntennaModel &antennaElement);


  /**
   * Returns the beamforming vector that is currently being used
   * \return the current beamforming vector
   */
  const AntennaModel & GetAntennaElement (void) const;



  ComplexVector m_beamformingVector; //!< the beamforming vector in use
  Ptr<AntennaModel> m_antennaElement; //!< the model of the antenna element in use

};

} /* namespace ns3 */

#endif /* PHASED_ARRAY_MODEL_H */
