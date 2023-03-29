#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::Return;
using ::testing::StrictMock;

class IDiskUitl {
 public:
  virtual int write(int offset, char* data, int len) = 0;
};

class FileWriter {
 public:
  IDiskUitl* diskutil_;
  uint64_t offset_;

  FileWriter(IDiskUitl *diskutil, uint64_t offset) : diskutil_(diskutil), offset_(offset) {}

  int writeFile(char* data, int len) {
    int ret = diskutil_->write(offset_, data, len);
    if (ret == 0) {
      offset_ += len;
    }
    return ret;
  }
};

class MockDiskUtil : public IDiskUitl {
 public:
  MOCK_METHOD(int, write, (int, char*, int), (override));
};

class FileWriterTestSuite : public testing::Test {
 protected:
  // 每个case执行前都会调用
  void SetUp() override {}

  StrictMock<MockDiskUtil> diskutil_;
};

TEST_F(FileWriterTestSuite, testWriteSuccess) {
  uint64_t woff = 1024;
  FileWriter w(&diskutil_, woff);

  std::string data = "hello";
  EXPECT_CALL(diskutil_, write(woff, (char*)data.c_str(), data.length())).WillOnce(Return(0));

  int ret = w.writeFile((char*)data.c_str(), data.length());
  EXPECT_EQ(ret, 0);
  EXPECT_EQ(w.offset_, woff + data.length());
}

TEST_F(FileWriterTestSuite, testWriteFailed) {
  uint64_t woff = 1024;
  FileWriter w(&diskutil_, woff);

  std::string data = "hello";
  EXPECT_CALL(diskutil_, write(woff, (char*)data.c_str(), data.length())).WillOnce(Return(1));

  int ret = w.writeFile((char*)data.c_str(), data.length());
  EXPECT_EQ(ret, 1);
  EXPECT_EQ(w.offset_, woff);
}

