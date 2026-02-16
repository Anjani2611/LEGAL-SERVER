#include "server/HttpServer.h"
#include "utils/Logger.h"
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>
#include <sstream>
#include <iostream>

HttpServer::HttpServer(
    const std::string& host,
    unsigned short port,
    size_t thread_pool_size,
    std::shared_ptr<Logger> logger
) : logger_(logger), host_(host), port_(port) {

    // Create work guard to keep io_context alive
    work_guard_ = std::make_unique<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>>(
        io_context_.get_executor()
    );

    logger_->info("HTTP Server initialized for {}:{} with {} threads",
                  host, port, thread_pool_size);

    // Create thread pool
    for (size_t i = 0; i < thread_pool_size; ++i) {
        thread_pool_.emplace_back([this, i]() {
            logger_->debug("Worker thread {} started", i);
            io_context_.run();
            logger_->debug("Worker thread {} stopped", i);
        });
    }
}

HttpServer::~HttpServer() {
    stop();
}

void HttpServer::start() {
    if (running_.load()) {
        logger_->warn("Server already running");
        return;
    }

    running_.store(true);
    logger_->info("Starting HTTP server...");

    // Create acceptor
    boost::asio::ip::tcp::endpoint endpoint(
        boost::asio::ip::make_address(host_),
        port_
    );

    acceptor_ = std::make_unique<boost::asio::ip::tcp::acceptor>(
        io_context_,
        endpoint
    );

    acceptor_->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));

    // Start accepting connections
    acceptConnections();

    logger_->info("HTTP server started successfully on {}:{}", host_, port_);
}

void HttpServer::stop() {
    if (!running_.load()) {
        return;
    }

    logger_->info("Stopping HTTP server...");
    running_.store(false);

    if (acceptor_) {
        acceptor_->close();
    }

    work_guard_.reset();
    io_context_.stop();

    for (auto& thread : thread_pool_) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    logger_->info("HTTP server stopped");
}

void HttpServer::setRequestHandler(RequestCallback handler) {
    request_handler_ = handler;
}

void HttpServer::acceptConnections() {
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(io_context_);

    acceptor_->async_accept(*socket, [this, socket](const boost::system::error_code& ec) {
        if (!ec) {
            active_connections_.fetch_add(1);
            logger_->info("New connection accepted");

            // Handle session in separate thread
            std::thread([this, socket]() {
                handleSession(socket);
                active_connections_.fetch_sub(1);
            }).detach();
        } else if (ec != boost::asio::error::operation_aborted) {
            logger_->error("Accept error: {}", ec.message());
        }

        if (running_.load()) {
            acceptConnections();
        }
    });
}

void HttpServer::handleSession(std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
    try {
        boost::asio::streambuf buffer;
        boost::system::error_code ec;

        boost::asio::read_until(*socket, buffer, "\r\n\r\n", ec);

        if (ec) {
            logger_->error("Read error: {}", ec.message());
            return;
        }

        std::istream stream(&buffer);
        std::string raw_request;
        std::string line;
        while (std::getline(stream, line)) {
            raw_request += line + "\n";
        }

        HttpRequest request;
        if (!parseRequest(raw_request, request)) {
            std::string response = buildResponse(400, R"({"error": "Bad Request"})");
            boost::asio::write(*socket, boost::asio::buffer(response), ec);
            return;
        }

        logger_->info("Request: {} {}", request.method, request.path);

        std::string response_body;
        if (request_handler_) {
            request_handler_(request.method, request.path, request.body, response_body);
        } else {
            response_body = R"({"error": "No handler configured"})";
        }

        std::string response = buildResponse(200, response_body);
        boost::asio::write(*socket, boost::asio::buffer(response), ec);

        if (ec) {
            logger_->error("Write error: {}", ec.message());
        }

    } catch (const std::exception& e) {
        logger_->error("Session error: {}", e.what());
    }
}

bool HttpServer::parseRequest(const std::string& raw_request, HttpRequest& request) {
    std::istringstream stream(raw_request);
    std::string line;

    if (!std::getline(stream, line)) {
        return false;
    }

    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }

    std::istringstream request_line(line);
    if (!(request_line >> request.method >> request.path >> request.version)) {
        return false;
    }

    while (std::getline(stream, line) && line != "\r") {
        if (line.empty() || line == "\r") {
            break;
        }

        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        size_t colon_pos = line.find(':');
        if (colon_pos != std::string::npos) {
            std::string key = line.substr(0, colon_pos);
            std::string value = line.substr(colon_pos + 1);

            if (!value.empty() && value[0] == ' ') {
                value = value.substr(1);
            }

            request.headers[key] = value;
        }
    }

    auto content_length_it = request.headers.find("Content-Length");
    if (content_length_it != request.headers.end()) {
        size_t content_length = std::stoul(content_length_it->second);
        request.body.resize(content_length);
        stream.read(&request.body[0], content_length);
    }

    return true;
}

std::string HttpServer::buildResponse(
    int status_code,
    const std::string& body,
    const std::string& content_type
) {
    std::ostringstream response;

    std::string status_message;
    switch (status_code) {
        case 200: status_message = "OK"; break;
        case 400: status_message = "Bad Request"; break;
        case 404: status_message = "Not Found"; break;
        case 500: status_message = "Internal Server Error"; break;
        default: status_message = "Unknown"; break;
    }

    response << "HTTP/1.1 " << status_code << " " << status_message << "\r\n";
    response << "Content-Type: " << content_type << "\r\n";
    response << "Content-Length: " << body.size() << "\r\n";
    response << "Connection: close\r\n";
    response << "\r\n";
    response << body;

    return response.str();
}
