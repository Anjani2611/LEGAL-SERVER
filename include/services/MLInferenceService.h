#pragma once

#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>

class Logger;
class RedisCache;

struct InferenceResult {
    std::string text;
    float confidence;
    long inference_time_ms;
    std::string model_version;
};

class MLInferenceService {
public:
    MLInferenceService(
        const std::string& models_path,
        std::shared_ptr<Logger> logger,
        std::shared_ptr<RedisCache> cache = nullptr
    );

    InferenceResult simplifyClause(const std::string& clause);
    std::vector<InferenceResult> simplifyBatch(
        const std::vector<std::string>& clauses
    );

    InferenceResult classifyClause(const std::string& clause);
    std::vector<InferenceResult> classifyBatch(
        const std::vector<std::string>& clauses
    );

private:
    std::string models_path_;
    std::shared_ptr<Logger> logger_;
    std::shared_ptr<RedisCache> cache_;

    InferenceResult makeStubResult(
        const std::string& output_text,
        float confidence,
        const std::string& model_version
    );
};
