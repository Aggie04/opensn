#ifndef CHITECH_BOOLEANLOGICALVOLUME_H
#define CHITECH_BOOLEANLOGICALVOLUME_H

#include "LogicalVolume.h"

namespace chi_mesh
{

// ###################################################################
/**Boolean volume*/
class BooleanLogicalVolume : public LogicalVolume
{
public:
  std::vector<std::pair<bool, std::shared_ptr<const LogicalVolume>>> parts;

  static chi::InputParameters GetInputParameters();
  explicit BooleanLogicalVolume(const chi::InputParameters& params);

  bool Inside(const chi_mesh::Vector3& point) const override;
};

}

#endif // CHITECH_BOOLEANLOGICALVOLUME_H
