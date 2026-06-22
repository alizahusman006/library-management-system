/*
 * Simple HTTP Server for Library Management System
 * Uses basic socket programming - no external libraries needed
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "library.h"

using namespace std;

// Global library instance
Library lib;

// Helper function to send HTTP response
void sendResponse(int clientSocket, const string& status, const string& contentType, const string& body) {
    string response = "HTTP/1.1 " + status + "\r\n";
    response += "Content-Type: " + contentType + "\r\n";
    response += "Access-Control-Allow-Origin: *\r\n";
    response += "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n";
    response += "Access-Control-Allow-Headers: Content-Type\r\n";
    response += "Connection: close\r\n\r\n";
    response += body;
    
    send(clientSocket, response.c_str(), response.length(), 0);
}

// Helper function to send JSON response
void sendJSON(int clientSocket, const string& body) {
    sendResponse(clientSocket, "200 OK", "application/json", body);
}

// Helper function to send HTML response
void sendHTML(int clientSocket, const string& body) {
    sendResponse(clientSocket, "200 OK", "text/html", body);
}

// Helper function to send CSS response
void sendCSS(int clientSocket, const string& body) {
    sendResponse(clientSocket, "200 OK", "text/css", body);
}

// Helper function to send JS response
void sendJS(int clientSocket, const string& body) {
    sendResponse(clientSocket, "200 OK", "application/javascript", body);
}

// Helper function to send 404 response
void send404(int clientSocket) {
    string body = "<html><body><h1>404 Not Found</h1></body></html>";
    sendResponse(clientSocket, "404 Not Found", "text/html", body);
}

// Helper function to send 400 response
void send400(int clientSocket) {
    string body = "{\"error\":\"Bad Request\"}";
    sendJSON(clientSocket, body);
}

// Parse URL path from request
string getPath(const string& request) {
    size_t start = request.find(" ");
    if (start == string::npos) return "";
    size_t end = request.find(" ", start + 1);
    if (end == string::npos) return "";
    return request.substr(start + 1, end - start - 1);
}

// Get form field value from POST body
string getFormField(const string& body, const string& field) {
    size_t pos = body.find(field + "=");
    if (pos == string::npos) return "";
    
    pos += field.length() + 1;
    size_t end = body.find("&", pos);
    if (end == string::npos) end = body.length();
    
    string value = body.substr(pos, end - pos);
    // URL decode
    string decoded;
    for (size_t i = 0; i < value.length(); i++) {
        if (value[i] == '%' && i + 2 < value.length()) {
            string hex = value.substr(i + 1, 2);
            decoded += (char)stoi(hex, nullptr, 16);
            i += 2;
        } else if (value[i] == '+') {
            decoded += ' ';
        } else {
            decoded += value[i];
        }
    }
    return decoded;
}

// Get POST body
string getPostBody(const string& request) {
    size_t pos = request.find("\r\n\r\n");
    if (pos == string::npos) return "";
    return request.substr(pos + 4);
}

// Helper to check if request method is GET
bool isGET(const string& req) {
    return req.substr(0, 4) == "GET ";
}

// Helper to check if request method is POST
bool isPOST(const string& req) {
    return req.substr(0, 5) == "POST ";
}

// Handle API requests
void handleAPI(int clientSocket, const string& path, const string& request) {
    // GET /api/resources - Get all resources
    if (path == "/api/resources" && isGET(request)) {
        sendJSON(clientSocket, lib.getAllResourcesJSON());
        return;
    }

    // GET /api/members - Get all members
    if (path == "/api/members" && isGET(request)) {
        sendJSON(clientSocket, lib.getAllMembersJSON());
        return;
    }

    // POST /api/resources - Add new resource
    if (path == "/api/resources" && isPOST(request)) {
        string body = getPostBody(request);
        
        // Parse form data
        string type, title, author, extra1, extra2;
        size_t pos = 0;
        
        // Simple parsing of form data
        pos = body.find("type=");
        if (pos != string::npos) {
            size_t end = body.find("&", pos);
            type = body.substr(pos + 5, end - pos - 5);
        }
        
        pos = body.find("title=");
        if (pos != string::npos) {
            size_t end = body.find("&", pos);
            title = body.substr(pos + 6, end - pos - 6);
        }
        
        pos = body.find("author=");
        if (pos != string::npos) {
            size_t end = body.find("&", pos);
            author = body.substr(pos + 7, end - pos - 7);
        }
        
        pos = body.find("extra1=");
        if (pos != string::npos) {
            size_t end = body.find("&", pos);
            extra1 = body.substr(pos + 7, end - pos - 7);
        }
        
        pos = body.find("extra2=");
        if (pos != string::npos) {
            extra2 = body.substr(pos + 7);
        }
        
        int id = lib.addResource(type, title, author, extra1, extra2);
        string response = "{\"id\":";
        response += to_string(id);
        response += ",\"success\":true}";
        sendJSON(clientSocket, response);
        return;
    }

    // POST /api/members - Add new member
    if (path == "/api/members" && isPOST(request)) {
        string body = getPostBody(request);
        
        size_t pos = body.find("name=");
        if (pos != string::npos) {
            string name = body.substr(pos + 5);
            int id = lib.addMember(name);
            string response = "{\"id\":";
            response += to_string(id);
            response += ",\"success\":true}";
            sendJSON(clientSocket, response);
            return;
        }
        
        send400(clientSocket);
        return;
    }

    // POST /api/borrow - Borrow a resource
    if (path == "/api/borrow" && isPOST(request)) {
        string body = getPostBody(request);
        
        string memberIDStr = getFormField(body, "memberID");
        string resourceIDStr = getFormField(body, "resourceID");
        
        if (memberIDStr.empty() || resourceIDStr.empty()) {
            send400(clientSocket);
            return;
        }
        
        int memberID = stoi(memberIDStr);
        int resourceID = stoi(resourceIDStr);
        
        bool success = lib.borrowResource(memberID, resourceID);
        string response = "{\"success\":";
        response += string(success ? "true" : "false");
        response += "}";
        sendJSON(clientSocket, response);
        return;
    }

    // POST /api/return - Return a resource
    if (path == "/api/return" && isPOST(request)) {
        string body = getPostBody(request);
        
        string memberIDStr = getFormField(body, "memberID");
        string resourceIDStr = getFormField(body, "resourceID");
        
        if (memberIDStr.empty() || resourceIDStr.empty()) {
            send400(clientSocket);
            return;
        }
        
        int memberID = stoi(memberIDStr);
        int resourceID = stoi(resourceIDStr);
        
        bool success = lib.returnResource(memberID, resourceID);
        string response = "{\"success\":";
        response += string(success ? "true" : "false");
        response += "}";
        sendJSON(clientSocket, response);
        return;
    }

    // GET /api/stats - Get statistics
    if (path == "/api/stats" && isGET(request)) {
        string response = "{\"resources\":";
        response += to_string(lib.getResourceCount());
        response += ",\"members\":";
        response += to_string(lib.getMemberCount());
        response += "}";
        sendJSON(clientSocket, response);
        return;
    }

    send404(clientSocket);
}

// Main server function
int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int port = 8080;

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        cerr << "Error creating socket" << endl;
        return 1;
    }

    // Set socket options
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Bind socket
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (::bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Error binding socket" << endl;
        return 1;
    }

    // Listen for connections
    listen(serverSocket, 5);
    cout << "========================================" << endl;
    cout << "  Library Management Server Started" << endl;
    cout << "  Server running on http://localhost:" << port << endl;
    cout << "  Press Ctrl+C to stop" << endl;
    cout << "========================================" << endl;

    // Load library data
    cout << "Loading library data..." << endl;
    cout << "Resources: " << lib.getResourceCount() << endl;
    cout << "Members: " << lib.getMemberCount() << endl;
    cout << endl;

    // Read static files
    ifstream indexFile("../frontend/index.html");
    ifstream cssFile("../frontend/styles.css");
    ifstream jsFile("../frontend/script.js");

    string indexHTML, cssContent, jsContent;
    
    if (indexFile.is_open()) {
        stringstream buffer;
        buffer << indexFile.rdbuf();
        indexHTML = buffer.str();
        indexFile.close();
    }
    
    if (cssFile.is_open()) {
        stringstream buffer;
        buffer << cssFile.rdbuf();
        cssContent = buffer.str();
        cssFile.close();
    }
    
    if (jsFile.is_open()) {
        stringstream buffer;
        buffer << jsFile.rdbuf();
        jsContent = buffer.str();
        jsFile.close();
    }

    // Main server loop
    while (true) {
        socklen_t clientLen = sizeof(clientAddr);
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        
        if (clientSocket < 0) {
            cerr << "Error accepting connection" << endl;
            continue;
        }

        // Read request
        char buffer[4096] = {0};
        read(clientSocket, buffer, 4095);
        string request(buffer);
        
        // Get path
        string path = getPath(request);
        
        // Route requests
        if (path == "/" || path == "/index.html") {
            sendHTML(clientSocket, indexHTML);
        } else if (path == "/styles.css") {
            sendCSS(clientSocket, cssContent);
        } else if (path == "/script.js") {
            sendJS(clientSocket, jsContent);
        } else if (path.find("/api/") == 0) {
            handleAPI(clientSocket, path, request);
        } else {
            send404(clientSocket);
        }

        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}