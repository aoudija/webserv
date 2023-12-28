#include "../server.hpp"

using std::string;

string httpResponse() {
    static int flag;

    std::ifstream htmlFile("index.html");
    if (!htmlFile.is_open()) {
        std::cerr << "Error opening HTML file\n";
        // Log the error, return an error response, or handle it appropriately
    }

    string buffer,c;
    while (std::getline(htmlFile, c)){
        buffer += c + "\n";
    }
    // Load HTML content
    std::string htmlContent(buffer);

    std::ifstream imageFile("img.png");
    if (!imageFile.is_open()) {
        std::cerr << "Error opening image file\n";
        // Log the error, return an error response, or handle it appropriately
    }
    c = "";
    std::string imageContent;
    while (std::getline(imageFile, c))
        imageContent += c;
    // Load image content


    std::string msg;
    if (flag == 0)
    {
        std::cout << "FIRST " << std::endl;
        msg = "HTTP/1.1 200 OK\r\n"
            "Content-Length: " + std::to_string(htmlContent.size()) + "\r\n"
            "Content-Type: text/html\r\n"
            "\r\n" + htmlContent;
        flag = 1;
    }
    else if (flag == 1)
    {
        std::cout << "SECONDE " << std::endl;
        msg = "HTTP/1.1 200 OK\r\n"
        "Content-Length: " + std::to_string(imageContent.size()) + "\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n";
        // + imageContent
        flag = 0;
    }
    // Create the HTTP response
    // cout << "ktaba : " << msg << endl;
    return msg;
}