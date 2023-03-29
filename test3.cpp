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

  FileWriter(IDiskUitl *diskutil) : diskutil_(diskutil) {}

  std::string writeFile(int offset, char* data, int len) {
    int ret = diskutil_->write(offset, data, len);
    return ret == 0 ? "success" : "error";
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
  FileWriter w(&diskutil_);

  std::string data = "hello";
  EXPECT_CALL(diskutil_, write(1024, (char*)data.c_str(), data.length())).WillOnce(Return(0));

  std::string ret = w.writeFile(1024, (char*)data.c_str(), data.length());
  EXPECT_EQ(ret, "success");
}

TEST_F(FileWriterTestSuite, testWriteFailed) {
  FileWriter w(&diskutil_);

  std::string data = "hello";
  EXPECT_CALL(diskutil_, write(1024, (char*)data.c_str(), data.length())).WillOnce(Return(-1));

  std::string ret = w.writeFile(1024, (char*)data.c_str(), data.length());
  EXPECT_EQ(ret, "error");
}

