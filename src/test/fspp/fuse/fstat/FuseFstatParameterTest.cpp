#include "testutils/FuseFstatTest.h"

#include "fspp/impl/FuseErrnoException.h"

using ::testing::_;
using ::testing::StrEq;
using ::testing::WithParamInterface;
using ::testing::Values;
using ::testing::Eq;
using ::testing::Return;
using ::testing::Throw;

using namespace fspp;

// Cite from FUSE documentation on the fgetattr function:
// "Currently this is only called after the create() method if that is implemented (see above).
//  Later it may be called for invocations of fstat() too."
// So we need to issue a create to get our fstat called.

class FuseFstatParameterTest: public FuseFstatTest, public WithParamInterface<int> {
public:

};
INSTANTIATE_TEST_CASE_P(FuseFstatParameterTest, FuseFstatParameterTest, Values(0,1,10,1000,1024*1024*1024));


TEST_P(FuseFstatParameterTest, FileDescriptorIsCorrect) {
  ReturnDoesntExistOnLstat(FILENAME);
  EXPECT_CALL(fsimpl, createAndOpenFile(StrEq(FILENAME), _)).Times(1).WillOnce(Return(GetParam()));

  EXPECT_CALL(fsimpl, fstat(Eq(GetParam()), _)).Times(1).WillOnce(ReturnIsFileFstat);

  auto fs = TestFS();
  CreateFile(fs.get(), FILENAME);
}
