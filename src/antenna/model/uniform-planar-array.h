#ifndef UNIFORM_PLANAR_ARRAY_H
#define UNIFORM_PLANAR_ARRAY_H


#include <ns3/object.h>
#include "ns3/phased-array-model.h"


namespace ns3 {


/**
 * \ingroup antenna
 *
 * \brief Class implementing Uniform Planar Array (UPA) model.
 *
 * \note the current implementation supports the modeling of antenna arrays
 * composed of a single panel and with single (vertical) polarization.
 */
class UniformPlanarArray : public PhasedArrayModel
{
public:
  /**
   * Constructor
   */
  UniformPlanarArray (void);


  /**
   * Destructor
   */
  virtual ~UniformPlanarArray (void);


  // inherited from Object
  static TypeId GetTypeId (void);


  /**
   * Returns the horizontal and vertical components of the antenna element field
   * pattern at the specified direction. Only vertical polarization is considered.
   * \param a the angle indicating the interested direction
   * \return a pair in which the first element is the horizontal component
   *         of the field pattern and the second element is the vertical
   *         component of the field pattern
   */
  std::pair<double, double> GetElementFieldPattern (Angles a) const override;


  /**
  * Returns the location of the antenna element with the specified
  * index assuming the left bottom corner is (0,0,0), normalized
  * with respect to the wavelength.
  * Antenna elements are scanned row by row, left to right and bottom to top.
  * For example, an antenna with 2 rows and 3 columns will be ordered as follows:
  * ^ z
  * |  3 4 5
  * |  0 1 2
  * ----------> y
  *
  * \param index index of the antenna element
  * \return the 3D vector that represents the position of the element
  */
  Vector GetElementLocation (uint64_t index) const override;

  /**
   * Returns the number of antenna elements
   * \return the number of antenna elements
   */
  uint64_t GetNumberOfElements (void) const override;


private:
  uint32_t m_numColumns; //!< number of columns
  uint32_t m_numRows; //!< number of rows
  double m_disV; //!< antenna spacing in the vertical direction in multiples of wave length
  double m_disH; //!< antenna spacing in the horizontal direction in multiples of wave length
  double m_alpha; //!< the bearing angle in radians
  double m_beta; //!< the downtilt angle in radians

};

} /* namespace ns3 */

#endif /* UNIFORM_PLANAR_ARRAY_H */
