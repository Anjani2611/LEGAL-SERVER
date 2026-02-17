#include "handlers/RequestHandler.h"
#include "services/LegalService.h"
#include "utils/Logger.h"

RequestHandler::RequestHandler(
    std::shared_ptr<Logger> logger,
    std::shared_ptr<LegalService> legal_service,
    std::shared_ptr<RedisCache> cache
) : logger_(std::move(logger)),
    legal_service_(std::move(legal_service)),
    cache_(std::move(cache)) {}

void RequestHandler::handleRequest(
    const std::string& method,
    const std::string& path,
    const std::string& body,
    std::string& response
) {
    json resp_json;

    try {
        if (method == "GET" && path == "/health") {
            handleHealthCheck(resp_json);
        } else if (method == "POST" && path == "/api/v1/simplify") {
            auto req_json = json::parse(body);
            handleSimplifyClause(req_json, resp_json);
        } else if (method == "POST" && path == "/api/v1/classify") {
            auto req_json = json::parse(body);
            handleClassifyClause(req_json, resp_json);
        } else {
            resp_json = createErrorResponse(404, "Not Found");
            resp_json["path"] = path;
            resp_json["method"] = method;
        }
    } catch (const json::exception& e) {
        resp_json = createErrorResponse(400, std::string("Invalid JSON: ") + e.what());
    } catch (const std::exception& e) {
        resp_json = createErrorResponse(500, std::string("Internal error: ") + e.what());
    }

    response = resp_json.dump(2);
}

void RequestHandler::handleHealthCheck(json& response) {
    response = {
        {"status", "healthy"},
        {"service", "legal-server"},
        {"version", "1.0.0"},
        {"week", 2},
        {"timestamp", std::time(nullptr)}
    };
}

bool RequestHandler::validateRequest(
    const json& request,
    const std::vector<std::string>& required_fields
) {
    for (const auto& f : required_fields) {
        if (!request.contains(f) || request[f].is_null()) {
            return false;
        }
    }
    return true;
}

void RequestHandler::handleSimplifyClause(
    const json& request,
    json& response
) {
    if (!validateRequest(request, {"clause"})) {
        response = createErrorResponse(400, "Missing required field: clause");
        return;
    }

    std::string clause = request.at("clause").get<std::string>();

    auto result = legal_service_->simplifyClause(clause);

    response = createSuccessResponse(json{
        {"simplified", result.text},
        {"confidence", result.confidence},
        {"model_version", result.model_version},
        {"inference_time_ms", result.inference_time_ms}
    });
}

void RequestHandler::handleClassifyClause(
    const json& request,
    json& response
) {
    if (!validateRequest(request, {"clause"})) {
        response = createErrorResponse(400, "Missing required field: clause");
        return;
    }

    std::string clause = request.at("clause").get<std::string>();

    auto result = legal_service_->classifyClause(clause);

    response = createSuccessResponse(json{
        {"label", result.text},
        {"score", result.confidence},
        {"model_version", result.model_version},
        {"inference_time_ms", result.inference_time_ms}
    });
}

json RequestHandler::createErrorResponse(int status_code, const std::string& message) {
    return json{
        {"status", "error"},
        {"code", status_code},
        {"message", message}
    };
}

json RequestHandler::createSuccessResponse(const json& data) {
    return json{
        {"status", "ok"},
        {"data", data}
    };
}
