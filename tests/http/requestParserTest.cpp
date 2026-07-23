#include "http/requestParser.h"

#include <gtest/gtest.h>


class RequestParserTest : public ::testing::Test {
protected:
    const std::string requestHeader =
    "GET /get HTTP/1.1\r\n"
    "Host: httpbin.org\r\n"
    "User-Agent: MyCppClient/1.0\r\n"
    "Accept: */*\r\n"
    "Connection: close\r\n"
    "\r\n";

    const std::string requestBody = "";
    

};


TEST_F(RequestParserTest, ParseHeaderAttributes) {
    EXPECT_TRUE(true);
}