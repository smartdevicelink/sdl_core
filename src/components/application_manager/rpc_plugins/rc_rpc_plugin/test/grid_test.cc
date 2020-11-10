/*
 * Copyright (c) 2019, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "gtest/gtest.h"

#include "rc_rpc_plugin/rc_app_extension.h"

namespace rc_rpc_plugin_test {
namespace grid_test {
using namespace rc_rpc_plugin;

TEST(Grid, IsLevelIntersectionExists) {
  Grid grid_level0{1, 1, 0, 1, 1, 1};
  Grid grid_level1{1, 1, 1, 1, 1, 1};
  EXPECT_FALSE(grid_level0.LevelIntersectionExists(grid_level1));

  Grid grid_level1_v2{1, 2, 1, 1, 2, 1};
  EXPECT_TRUE(grid_level1.LevelIntersectionExists(grid_level1_v2));
}

TEST(Grid, TwoSameGrisAreEqual) {
  Grid grid2{1, 1, 0, 1, 1, 1};
  Grid grid1{1, 1, 0, 1, 1, 1};

  EXPECT_EQ(grid1, grid2);
}

TEST(Grid, IsIntersectionExists) {
  Grid grid_1{0, 1, 2, 1, 1, 1};
  Grid grid_2{0, 0, 1, 0, 0, 1};

  // There is intersection but levels missmatch
  EXPECT_FALSE(grid_1.IntersectionExists(grid_2));

  Grid grid_3{1, 0, 2, 0, 0, 0};
  // There isn't intersection but level match
  EXPECT_FALSE(grid_1.IntersectionExists(grid_3));

  Grid grid_4{0, 1, 2, 1, 1, 1};
  EXPECT_TRUE(grid_1.IntersectionExists(grid_4));
}
}  // namespace grid_test
}  // namespace rc_rpc_plugin_test
