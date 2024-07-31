#include "request_queue.h"

RequestQueue::RequestQueue(const SearchServer& search_server) : search_server_(search_server)  {}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentStatus status) {
    std::vector<Document> result = search_server_.FindTopDocuments(raw_query, status);
    AddRequest(result);
    return result;
}
std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query) {
    std::vector<Document> result = search_server_.FindTopDocuments(raw_query);
    AddRequest(result);
    return result;
}

int RequestQueue::GetNoResultRequests() const {
    return no_result_request_;
}

void RequestQueue::AddRequest(const std::vector<Document>& documents) {
    current_time_++;
    if (requests_.size() >= min_in_day_) {
        if (requests_.front().documents.empty()) {
            --no_result_request_;
        }
        requests_.pop_front();
        current_time_ = 0;
    }
    requests_.push_back({documents, current_time_});
    if (documents.empty()) {
        ++no_result_request_;
    }
}
