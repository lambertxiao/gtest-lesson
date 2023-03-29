# gtest 使用指北

## 准备工作

下载googletest库，并编译安装

## 怎么运行当前示例

```
mkdir build 
cd build 
cmake ../ && make 
./test_main
```

## 示例讲解

## test1.cpp

演示了如何做最基本的单元测试

```c++
#include <gtest/gtest.h>

int add(int x, int y) {
  return x + y;
}

TEST(AdditionTest, testAdd) {
  EXPECT_EQ(add(1, 2), 3);
}
```

## test2.cpp

演示了如何写一个测试用例集

```c++
#include <gtest/gtest.h>

class Math {
 public:
  int add(int x, int y) {
    return x + y;
  }
  int sub(int x, int y) {
    return x - y;
  }
};

class MathTestSuite : public testing::Test {
 protected:
  Math math;
};

TEST_F(MathTestSuite, testAdd) {
  EXPECT_EQ(math.add(1, 2), 3);
}

TEST_F(MathTestSuite, testSub) {
  EXPECT_EQ(math.sub(3, 1), 2);
}
```

## test3.cpp

演示了如何做mock测试，定义了一个简单的常见,FileWriter需要依赖DiskUtil做文件写入，而通过mock掉DiskUtil的写入返回值，从而决定FileWriter应该执行什么逻辑

```c++
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
```

