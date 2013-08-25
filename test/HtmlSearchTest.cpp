#include <boost/test/unit_test.hpp>
#include "HtmlSearch.hpp"
#include "HtmlTestContent.hpp"

struct HtmlSearchTestFixture
{
	HtmlSearchTestFixture()
	{

	}

	void MinimalHtml5()
	{
		doc.clear();
        doc.append(Html5Begin());
		doc.append(Html5End());
	}

	void WithBody(const std::string& content)
	{
		doc.clear();
        doc.append(Html5Begin());

		doc.append(content);
		doc.append(Html5End());
	}

	void Search()
	{
        result = search.Search(doc);
	}

	const std::vector<std::string>& SearchLinks()
	{
		Search();
		return result.links;
	}

	bool HasLink(const std::string& url)
	{
		auto links = result.links;
		return std::find(begin(links), end(links), url)
			!= end(links);
	}

	HtmlSearch search;
	HtmlSearchResult result;
	std::string doc;
};


BOOST_FIXTURE_TEST_SUITE(HtmlSearchTestSuite, HtmlSearchTestFixture)

BOOST_AUTO_TEST_CASE(SearchLinks_EmptyDoc_IsEmpty)
{	
	auto links = SearchLinks();
	BOOST_CHECK_EQUAL(links.size(), 0);
}

BOOST_AUTO_TEST_CASE(SearchLinks_MinimalHtml5_IsEmpty)
{
	MinimalHtml5();
	auto links = SearchLinks();
	BOOST_CHECK_EQUAL(links.size(), 0);
}

BOOST_AUTO_TEST_CASE(SearchLinks_HasSingleAnchorInBody_ReturnsUrl)
{
	WithBody(R"(<a href="http://www.testurl.com">Text</a>)");
	auto links = SearchLinks();
	BOOST_REQUIRE(!links.empty());
	BOOST_CHECK_EQUAL(links.front(), "http://www.testurl.com");
}

BOOST_AUTO_TEST_CASE(SearchLinks_HasSingleAnchorWithAdditionalAttributeInBody_ReturnsUrl)
{
	WithBody(R"(<a rel="nofollow" href="http://www.testurl.com">Text</a>)");
	auto links = SearchLinks();
	BOOST_REQUIRE(!links.empty());
	BOOST_CHECK_EQUAL(links.front(), "http://www.testurl.com");
}

BOOST_AUTO_TEST_CASE(SearchLinks_HasSingleAnchorWithAdditionalAttributeInBodyUpperCase_ReturnsUrl)
{
	WithBody(R"(<A rel="nofollow" HREF="http://www.testurl.com">Text</A>)");
	auto links = SearchLinks();
	BOOST_REQUIRE(!links.empty());
	BOOST_CHECK_EQUAL(links.front(), "http://www.testurl.com");
}

BOOST_AUTO_TEST_CASE(SearchLinks_HasHrefInSingleQuotes_ReturnsUrl)
{
    WithBody(R"(<a rel="nofollow" href='http://www.testurl.com'>Text</a>)");
    auto links = SearchLinks();
    BOOST_REQUIRE(!links.empty());
    BOOST_CHECK_EQUAL(links.front(), "http://www.testurl.com");
}

BOOST_AUTO_TEST_CASE(SearchLinks_SingleQuotesInDoubleQuotes_ReturnsUrl)
{
    WithBody(R"(<a rel="nofollow" href="http://www.testurl.com/'hallo'">Text</a>)");
    auto links = SearchLinks();
    BOOST_REQUIRE(!links.empty());
    BOOST_CHECK_EQUAL(links.front(), "http://www.testurl.com/'hallo'");
}

BOOST_AUTO_TEST_CASE(SearchLinks_HasWhiteSpace_ReturnsUrl)
{
    WithBody(R"(<a href="hello world">Text</a>)");
    auto links = SearchLinks();
    BOOST_REQUIRE(!links.empty());
    BOOST_CHECK_EQUAL(links.front(), "hello world");
}


BOOST_AUTO_TEST_CASE(SearchLinks_Has3AnchorInBody_Returns3Urls)
{
	WithBody(R"(
		<p>This is a test 
		<a href="1">link1</a>. <a 
		href="2" />2</a>
		<a href="3" ><img src="1.png"></a></p>
		)"
		);
	Search();
	BOOST_CHECK(HasLink("1"));
	BOOST_CHECK(HasLink("2"));
	BOOST_CHECK(HasLink("3"));
}

BOOST_AUTO_TEST_CASE(SearchLinks_HasBrokenTag_Returns2Urls)
{
	WithBody(R"(
		<p This is a test 
		<a href="1">link1</a>. <a 
		href="2" />2</a>
		<a href="3" ><img src="1.png"></a></p>
		)"
		);
	Search();
	BOOST_CHECK(HasLink("2"));
	BOOST_CHECK(HasLink("3"));
}

BOOST_AUTO_TEST_CASE(SearchLinks_HasLinkInComment_LinkNotAdded)
{
	WithBody(R"(
		<!--
		<a href="1">link1</a>. <a 
		href="2" />2</a>
        -->
		<a href="3" ><img src="1.png"></a></p>
		)"
		);
	Search();
	BOOST_CHECK_EQUAL(result.links.size(), 1);
	BOOST_CHECK(HasLink("3"));
}


BOOST_AUTO_TEST_SUITE_END()
