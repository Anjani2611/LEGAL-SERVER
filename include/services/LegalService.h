#pragma once

#include <string>
#include <memory>
#include <vector>

#include "services/MLInferenceService.h"

class Logger;
class DocumentRepository;
class RedisCache;

class LegalService {
public:
    LegalService(
        std::shared_ptr<Logger> logger,
        std::shared_ptr<DocumentRepository> db,
        std::shared_ptr<RedisCache> cache,
        std::shared_ptr<MLInferenceService> ml_service
    );

    InferenceResult simplifyClause(const std::string& clause);
    InferenceResult classifyClause(const std::string& clause);

    std::vector<InferenceResult> simplifyClauses(
        const std::vector<std::string>& clauses
    );
    std::vector<InferenceResult> classifyClauses(
        const std::vector<std::string>& clauses
    );

private:
    std::shared_ptr<Logger> logger_;
    std::shared_ptr<DocumentRepository> db_;
    std::shared_ptr<RedisCache> cache_;
    std::shared_ptr<MLInferenceService> ml_service_;
};
