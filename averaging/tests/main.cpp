#include <gtest/gtest.h>
#include <glog/logging.h>
#include <gflags/gflags.h>

DEFINE_bool(write_generated_level_set_mesh, false,
            "Should the generated mesh by the level set mesh builder be "
            "written out to file");

int main(int argc, char** argv) {
  ::google::InitGoogleLogging(argv[0]);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
