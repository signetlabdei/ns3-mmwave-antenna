#ifndef PHASED_ARRAY_MODEL_H
#define PHASED_ARRAY_MODEL_H

#include <ns3/object.h>
#include <ns3/angles.h>
#include <complex>
#include "antenna-model.h"

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
  PhasedArrayModel (void);


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
  virtual Vector GetElementLocation (uint64_t index) const = 0;


  /**
   * Returns the number of antenna elements
   * \return the number of antenna elements
   */
  virtual uint64_t GetNumberOfElements (void) const = 0;


  /**
   * Sets the beamforming vector to be used
   * \param beamformingVector the beamforming vector
   */
  void SetBeamformingVector (const ComplexVector &beamformingVector);


  /**
   * Returns the beamforming vector that is currently being used
   * \return the current beamforming vector
   */
  ComplexVector GetBeamformingVector (void) const;


  /**
   * Returns the beamforming vector that points towards the specified position
   * \param a the beamforming angle
   * \return the beamforming vector
   */
  ComplexVector GetBeamformingVector (Angles a) const;


  /**
   * Returns the steering vector that points toward the specified position
   * \param a the steering angle
   * \return the steering vector
   */
  ComplexVector GetSteeringVector (Angles a) const;


  /**
   * Sets the antenna model to be used
   * \param AntennaModel the antenna model
   */
  void SetAntennaElement (Ptr<AntennaModel> antennaElement);


  /**
   * Returns a pointer to the AntennaModel instance used to model the elements of the array
   * \return pointer to the AntennaModel instance
   */
  Ptr<const AntennaModel> GetAntennaElement (void) const;

protected:
  static double ComputeNorm (const ComplexVector &vector);

  ComplexVector m_beamformingVector; //!< the beamforming vector in use
  Ptr<AntennaModel> m_antennaElement; //!< the model of the antenna element in use

};

} /* namespace ns3 */

#endif /* PHASED_ARRAY_MODEL_H */
