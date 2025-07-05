#include <winsock2.h>
#include <iostream>
#include <fstream>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

std::string load_html(const char* filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    WSADATA wsa;
    SOCKET s, new_socket;
    struct sockaddr_in server, client;
    int c;
    char buffer[1024];

    std::cout << "Initializing Winsock..." << std::endl;
    WSAStartup(MAKEWORD(2,2),&wsa);

    s = socket(AF_INET , SOCK_STREAM , 0 );
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    bind(s , (struct sockaddr *)&server , sizeof(server));
    listen(s , 3);

    std::cout << "Waiting for incoming connections..." << std::endl;
    c = sizeof(struct sockaddr_in);
    new_socket = accept(s , (struct sockaddr *)&client, &c);

    recv(new_socket , buffer , 1024 , 0);
    std::cout << "Request received:\n" << buffer << std::endl;

    // HTML dosyasını oku
    std::string html = load_html("index.html");
    std::string response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n" +
        html;

    send(new_socket , response.c_str() , response.size() , 0);

    closesocket(s);
    WSACleanup();

    return 0;
} 