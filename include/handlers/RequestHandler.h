#pragma once

#include <string>
#include <memory>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Logger;
class LegalService;
class RedisCache;

class RequestHandler {
public:
    RequestHandler(
        std::shared_ptr<Logger> logger,
        std::shared_ptr<LegalService> legal_service,
        std::shared_ptr<RedisCache> cache = nullptr
    );

    // Main entry point used by HttpServer
    void handleRequest(
        const std::string& method,
        const std::string& path,
        const std::string& body,
        std::string& response
    );

private:
    std::shared_ptr<Logger> logger_;
    std::shared_ptr<LegalService> legal_service_;
    std::shared_ptr<RedisCache> cache_;

    // Route handlers
    void handleHealthCheck(json& response);
    void handleSimplifyClause(const json& request, json& response);
    void handleClassifyClause(const json& request, json& response);

    // Helpers
    json createErrorResponse(int status_code, const std::string& message);
    json createSuccessResponse(const json& data);
    bool validateRequest(
        const json& request,
        const std::vector<std::string>& required_fields
    );
};
