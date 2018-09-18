/*
 * (C) Copyright 2017-2018 UCAR
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 */

#include "ufo/constituents/aod/ObsAod.h"

#include <ostream>
#include <string>
#include <vector>

#include "ioda/ObsVector.h"

#include "oops/base/Variables.h"

#include "ufo/GeoVaLs.h"
#include "ufo/ObsBias.h"

namespace ufo {

// -----------------------------------------------------------------------------
static ObsOperatorMaker<ObsAod> makerAOD_("Aod");
// -----------------------------------------------------------------------------

ObsAod::ObsAod(const ioda::ObsSpace & odb, const eckit::Configuration & config)
  : keyOperAod_(0), varin_(), odb_(odb)
{
  const eckit::Configuration * configc = &config;
  ufo_aod_setup_f90(keyOperAod_, &configc);
  const std::vector<std::string> vv{"temperature", "humidity_mixing_ratio",
      "air_pressure", "air_pressure_levels",
      "sulf", "bc1", "bc2", "oc1", "oc2", "dust1", "dust2", "dust3", "dust4", "dust5",
      "seas1", "seas2", "seas3", "seas4", "p25"};
  varin_.reset(new oops::Variables(vv));
  oops::Log::trace() << "ObsAod created." << std::endl;
}

// -----------------------------------------------------------------------------

ObsAod::~ObsAod() {
  ufo_aod_delete_f90(keyOperAod_);
  oops::Log::trace() << "ObsAod destructed" << std::endl;
}

// -----------------------------------------------------------------------------

void ObsAod::simulateObs(const GeoVaLs & gom, ioda::ObsVector & ovec,
                         const ObsBias & bias) const {
  ufo_aod_simobs_f90(keyOperAod_, gom.toFortran(), odb_.toFortran(), ovec.toFortran(),
                  bias.toFortran());
}

// -----------------------------------------------------------------------------

void ObsAod::print(std::ostream & os) const {
  os << "ObsAod::print not implemented";
}

// -----------------------------------------------------------------------------

}  // namespace ufo
