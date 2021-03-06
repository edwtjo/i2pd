#include <boost/test/unit_test.hpp>
#include "util/util.h"

BOOST_AUTO_TEST_SUITE(UtilityTests)

using namespace i2p::util::http;

BOOST_AUTO_TEST_CASE(DecodeEmptyUrl)
{
    BOOST_CHECK_EQUAL(urlDecode(""), "");
}

BOOST_AUTO_TEST_CASE(DecodeUrl)
{
    BOOST_CHECK_EQUAL(urlDecode("%20"), " ");
}
BOOST_AUTO_TEST_CASE(ParseUrlProtocol)
{
    BOOST_CHECK_EQUAL(url("http://127.0.0.1:7070/asdasd?qqqqqqqqqqqq").protocol_, "http");
    BOOST_CHECK_EQUAL(url("http://user:password@site.com:err_port/A/B?q").protocol_, "http");
    BOOST_CHECK_EQUAL(url("ftp://user@localhost:123").protocol_, "ftp");
    BOOST_CHECK_EQUAL(url("SSH://user:pass@localhost:123").protocol_, "ssh");
    BOOST_CHECK_EQUAL(url("").protocol_, "");
}

BOOST_AUTO_TEST_CASE(ParseUrlHost)
{
    BOOST_CHECK_EQUAL(url("http://127.0.0.1:7070/asdasd?qqqqqqqqqqqq").host_, "127.0.0.1");
    BOOST_CHECK_EQUAL(url("http://user:password@site.com:err_port/A/B?q").host_, "site.com");
    BOOST_CHECK_EQUAL(url("ftp://user@localhost:123").host_, "localhost");
    BOOST_CHECK_EQUAL(url("SSH://user:pass@localhost:123").host_, "localhost");
    BOOST_CHECK_EQUAL(url("").host_, "");
}


BOOST_AUTO_TEST_CASE(ParseUrlPath)
{
    BOOST_CHECK_EQUAL(url("http://127.0.0.1:7070/asdasd?qqqqqqqqqqqq").path_, "/asdasd");
    BOOST_CHECK_EQUAL(url("http://user:password@site.com:err_port/A/B?q").path_, "/A/B");
    BOOST_CHECK_EQUAL(url("ftp://user@localhost:123/A/B/C/D?x=A").path_, "/A/B/C/D");
    BOOST_CHECK_EQUAL(url("SSH://user:pass@localhost:123").path_, "");
    BOOST_CHECK_EQUAL(url("").path_, "");
} 
BOOST_AUTO_TEST_CASE(ParseUrlQuery)
{
    BOOST_CHECK_EQUAL(url("http://127.0.0.1:7070/asdasd?qqqqqqqqqqqq").query_, "qqqqqqqqqqqq");
    BOOST_CHECK_EQUAL(url("http://user:password@site.com:err_port/A/B?q").query_, "q");
    BOOST_CHECK_EQUAL(url("ftp://user@localhost:123/A/B/C/D?x=A").query_, "x=A");
    BOOST_CHECK_EQUAL(url("SSH://user:pass@localhost:123").query_, "");
    BOOST_CHECK_EQUAL(url("").query_, "");
}

BOOST_AUTO_TEST_CASE(ParseUrlPortStr)
{
    BOOST_CHECK_EQUAL(url("http://127.0.0.1:7070/asdasd?qqqqqqqqqqqq").portstr_, "7070");
    BOOST_CHECK_EQUAL(url("http://user:password@site.com:err_port/A/B?q").portstr_, "err_port");
    BOOST_CHECK_EQUAL(url("ftp://user@localhost:123/A/B/C/D?x=A").portstr_, "123");
    BOOST_CHECK_EQUAL(url("SSH://user:pass@localhost:123").portstr_, "123");
    BOOST_CHECK_EQUAL(url("").portstr_, "80");
}

BOOST_AUTO_TEST_CASE(ParseUrlPort)
{
    BOOST_CHECK_EQUAL(url("http://127.0.0.1:7070/asdasd?qqqqqqqqqqqq").port_, 7070);
    BOOST_CHECK_EQUAL(url("http://user:password@site.com:err_port/A/B?q").port_, 80);
    BOOST_CHECK_EQUAL(url("ftp://user@localhost:123/A/B/C/D?x=A").port_, 123);
    BOOST_CHECK_EQUAL(url("SSH://user:pass@localhost:123").port_, 123);
    BOOST_CHECK_EQUAL(url("").port_, 80);
}

BOOST_AUTO_TEST_CASE(ParseUrlUser)
{
    BOOST_CHECK_EQUAL(url("http://127.0.0.1:7070/asdasd?qqqqqqqqqqqq").user_, "");
    BOOST_CHECK_EQUAL(url("http://user:password@site.com:err_port/A/B?q").user_, "user");
    BOOST_CHECK_EQUAL(url("ftp://user@localhost:123/A/B/C/D?x=A").user_, "user");
    BOOST_CHECK_EQUAL(url("SSH://@localhost:123").user_, "");
    BOOST_CHECK_EQUAL(url("SSH://user:@localhost:123").user_, "user");
    BOOST_CHECK_EQUAL(url("").user_, "");
}

BOOST_AUTO_TEST_CASE(ParseUrlPassword)
{
    BOOST_CHECK_EQUAL(url("http://127.0.0.1:7070/asdasd?qqqqqqqqqqqq").pass_, "");
    BOOST_CHECK_EQUAL(url("http://user:password@site.com:err_port/A/B?q").pass_, "password");
    BOOST_CHECK_EQUAL(url("ftp://user@localhost:123/A/B/C/D?x=A").pass_, "");
    BOOST_CHECK_EQUAL(url("SSH://@localhost:123").pass_, "");
    BOOST_CHECK_EQUAL(url("SSH://:password@localhost:123").pass_, "password");
    BOOST_CHECK_EQUAL(url("").pass_, "");
}

BOOST_AUTO_TEST_SUITE_END()
