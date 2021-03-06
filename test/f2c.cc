/*
 * Copyright (C) 2020 Connor Ward.
 *
 * This file is part of PerpleX-cpp.
 *
 * PerpleX-cpp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PerpleX-cpp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PerpleX-cpp.  If not, see <https://www.gnu.org/licenses/>.
 */


#include <gtest/gtest.h>

#include "f2c.h"


using namespace f2c;

class InterfaceTest : public ::testing::Test {
  protected:
    void SetUp() override {
      const char *filename = "test";
      const double pressure = 20000;
      const double temperature = 1500;
      const double composition[4] = { 38.500, 2.820, 50.500, 5.880 };

      // Change working directory.
      if (chdir("./simple") != 0)
	throw std::invalid_argument("Could not change directory.");

      solver_init(filename);
      solver_set_pressure(pressure);
      solver_set_temperature(temperature);
      for (size_t i = 0; i < 4; i++)
	bulk_props_set_composition(i, composition[i]);

      solver_minimize();
    }
};


TEST_F(InterfaceTest, CheckPressureBounds)
{
  EXPECT_NEAR(get_min_pressure(), 1, 0.5);
  EXPECT_NEAR(get_max_pressure(), 50000, 0.5);
}


TEST_F(InterfaceTest, CheckTemperatureBounds)
{
  EXPECT_NEAR(get_min_temperature(), 1073, 0.5);
  EXPECT_NEAR(get_max_temperature(), 2273, 0.5);
}


TEST_F(InterfaceTest, CheckCompositionProps) {
  EXPECT_EQ(composition_props_get_n_components(), 4);

  EXPECT_STREQ(composition_props_get_name(0), "SiO2");
  EXPECT_STREQ(composition_props_get_name(1), "CaO");
  EXPECT_STREQ(composition_props_get_name(2), "MgO");
  EXPECT_STREQ(composition_props_get_name(3), "FeO");
}



TEST_F(InterfaceTest, CheckBulkProps) {
  EXPECT_NEAR(bulk_props_get_composition(3), 5.880, 5e-4);
}

TEST_F(InterfaceTest, CheckSolnPhaseProps) {
  EXPECT_EQ(soln_phase_props_get_n(), 4);

  EXPECT_STREQ(soln_phase_props_get_name(3), "Opx(HGP)");
  EXPECT_STREQ(soln_phase_props_get_abbr_name(2), "Ol");
}

TEST_F(InterfaceTest, CheckResPhaseProps) {
  EXPECT_EQ(res_phase_props_get_n(), 3); 

  EXPECT_STREQ(res_phase_props_get_name(0), "Cpx(HGP)");

  EXPECT_NEAR(res_phase_props_get_weight_frac(1)*100, 62.02, 5e-3);

  EXPECT_NEAR(res_phase_props_get_mol_frac(2)*100, 19.70, 5e-3);

  EXPECT_NEAR(res_phase_props_get_mol(0), 3.07, 5e-2);

  EXPECT_NEAR(get_endmember_composition_ratio(0, 0), 2.00000, 5e-6);
  EXPECT_NEAR(get_endmember_composition_ratio(0, 1), 0.74805, 5e-6);
  EXPECT_NEAR(get_endmember_composition_ratio(0, 2), 1.14355, 5e-6);
  EXPECT_NEAR(get_endmember_composition_ratio(0, 3), 0.10840, 5e-6);

  EXPECT_NEAR(get_endmember_composition_ratio(1, 0), 1.00000, 5e-6);
  EXPECT_NEAR(get_endmember_composition_ratio(1, 1), 0.00391, 5e-6);
  EXPECT_NEAR(get_endmember_composition_ratio(1, 2), 1.77645, 5e-6);
  EXPECT_NEAR(get_endmember_composition_ratio(1, 3), 0.21965, 5e-6);

  EXPECT_NEAR(get_endmember_composition_ratio(2, 0), 2.00000, 5e-6);
  EXPECT_NEAR(get_endmember_composition_ratio(2, 1), 0.07617, 5e-6);
  EXPECT_NEAR(get_endmember_composition_ratio(2, 2), 1.75224, 5e-6);
  EXPECT_NEAR(get_endmember_composition_ratio(2, 3), 0.17159, 5e-6);
}

TEST_F(InterfaceTest, CheckSysProps) {
  EXPECT_NEAR(sys_props_get_density(), 3249.3, 0.05);
  EXPECT_NEAR(sys_props_get_expansivity(), 0.38575e-4, 5e-9);
  EXPECT_NEAR(sys_props_get_mol_entropy(), 11996, 0.5);
  EXPECT_NEAR(sys_props_get_mol_heat_capacity(), 6244.7, 0.05);
}


TEST_F(InterfaceTest, CheckMolarWeights)
{
  EXPECT_NEAR(get_composition_molar_mass(0)*1000, 60.084, 5e-4);
  EXPECT_NEAR(get_composition_molar_mass(1)*1000, 56.077, 5e-4);
  EXPECT_NEAR(get_composition_molar_mass(2)*1000, 40.304, 5e-4);
  EXPECT_NEAR(get_composition_molar_mass(3)*1000, 71.844, 5e-4);
}

