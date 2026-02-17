#include "services/LegalService.h"
#include "utils/Logger.h"

// For now we don't actually use db_ or cache_ yet;
// they will be wired in Week 3+.

LegalService::LegalService(
    std::shared_ptr<Logger> logger,
    std::shared_ptr<DocumentRepository> db,
    std::shared_ptr<RedisCache> cache,
    std::shared_ptr<MLInferenceService> ml_service
) : logger_(std::move(logger)),
    db_(std::move(db)),
    cache_(std::move(cache)),
    ml_service_(std::move(ml_service)) {

    logger_->info("LegalService initialized");
}

InferenceResult LegalService::simplifyClause(const std::string& clause) {
    if (clause.empty()) {
        throw std::invalid_argument("Clause must not be empty");
    }
    return ml_service_->simplifyClause(clause);
}

InferenceResult LegalService::classifyClause(const std::string& clause) {
    if (clause.empty()) {
        throw std::invalid_argument("Clause must not be empty");
    }
    return ml_service_->classifyClause(clause);
}

std::vector<InferenceResult> LegalService::simplifyClauses(
    const std::vector<std::string>& clauses
) {
    return ml_service_->simplifyBatch(clauses);
}

std::vector<InferenceResult> LegalService::classifyClauses(
    const std::vector<std::string>& clauses
) {
    return ml_service_->classifyBatch(clauses);
}
