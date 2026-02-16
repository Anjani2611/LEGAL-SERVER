#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <atomic>
#include <thread>
#include <map>

class Logger;

class HttpServer {
public:
    using RequestCallback = std::function<void(
        const std::string& method,
        const std::string& path,
        const std::string& body,
        std::string& response
    )>;

    HttpServer(
        const std::string& host,
        unsigned short port,
        size_t thread_pool_size,
        std::shared_ptr<Logger> logger
    );

    ~HttpServer();

    void start();
    void stop();
    void setRequestHandler(RequestCallback handler);

    bool isRunning() const { return running_.load(); }
    size_t getActiveConnections() const { return active_connections_.load(); }

private:
    boost::asio::io_context io_context_;
    std::unique_ptr<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>> work_guard_;
    std::unique_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
    std::vector<std::thread> thread_pool_;
    std::atomic<bool> running_{false};
    std::atomic<size_t> active_connections_{0};
    RequestCallback request_handler_;
    std::shared_ptr<Logger> logger_;
    std::string host_;
    unsigned short port_;

    void acceptConnections();
    void handleSession(std::shared_ptr<boost::asio::ip::tcp::socket> socket);

    struct HttpRequest {
        std::string method;
        std::string path;
        std::string version;
        std::map<std::string, std::string> headers;
        std::string body;
    };

    bool parseRequest(const std::string& raw_request, HttpRequest& request);
    std::string buildResponse(
        int status_code,
        const std::string& body,
        const std::string& content_type = "application/json"
    );
};
