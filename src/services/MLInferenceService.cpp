#include "services/MLInferenceService.h"
#include "utils/Logger.h"
#include <chrono>

using json = nlohmann::json;

MLInferenceService::MLInferenceService(
    const std::string& models_path,
    std::shared_ptr<Logger> logger,
    std::shared_ptr<RedisCache> cache
) : models_path_(models_path),
    logger_(std::move(logger)),
    cache_(std::move(cache)) {

    logger_->info("MLInferenceService initialized with models path: {}", models_path_);
    // NOTE: In Week 2 we do NOT load real ONNX models yet.
}

InferenceResult MLInferenceService::makeStubResult(
    const std::string& output_text,
    float confidence,
    const std::string& model_version
) {
    InferenceResult r;
    r.text = output_text;
    r.confidence = confidence;
    r.inference_time_ms = 5; // dummy small latency
    r.model_version = model_version;
    return r;
}

InferenceResult MLInferenceService::simplifyClause(const std::string& clause) {
    logger_->debug("Stub simplifyClause called, input length={}", clause.size());
    // Stub: just append "[simplified]" for now
    return makeStubResult(
        clause + " [simplified]",
        0.90f,
        "t5_stub_v1"
    );
}

std::vector<InferenceResult> MLInferenceService::simplifyBatch(
    const std::vector<std::string>& clauses
) {
    std::vector<InferenceResult> results;
    results.reserve(clauses.size());
    for (const auto& c : clauses) {
        results.push_back(simplifyClause(c));
    }
    return results;
}

InferenceResult MLInferenceService::classifyClause(const std::string& clause) {
    logger_->debug("Stub classifyClause called, input length={}", clause.size());
    // Stub: pretend everything is "NDA" with high confidence
    std::string label = "NDA";
    return makeStubResult(
        label,
        0.95f,
        "legal_bert_stub_v1"
    );
}

std::vector<InferenceResult> MLInferenceService::classifyBatch(
    const std::vector<std::string>& clauses
) {
    std::vector<InferenceResult> results;
    results.reserve(clauses.size());
    for (const auto& c : clauses) {
        results.push_back(classifyClause(c));
    }
    return results;
}
