#ifndef HTML_TEST_CONTENT_HPP
#define HTML_TEST_CONTENT_HPP

inline std::string Html5Begin()
{
    return 
R"(
<!doctype html>
<head>
</head>
<body>
)";
}

inline std::string Html5End()
{
    return 
R"(
</body>
</html>
)";
}

#endif  // HTML_TEST_CONTENT_HPP
