#pragma once

#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>

template <typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end): begin_(begin), end_(end) {}
    Iterator begin() const {return begin_;}
    Iterator end() const {return end_;}
    size_t size() const {return distance(begin_, end_);}
private:
    Iterator begin_;
    Iterator end_;
};

template <typename Iterator>
std::ostream& operator<<(std::ostream& os, const IteratorRange<Iterator>& range) {
    for (auto it = range.begin(); it != range.end(); ++it) {
        os << *it;
    }
    return os;
}

template <typename Iterator>
class Paginator {
public:
    Paginator(Iterator begin, Iterator end, size_t page_size) {
        for(auto it = begin; it < end;) {
            auto next = std::distance(it, end) > page_size ? std::next(it, page_size) : end;
            page_.emplace_back(it, next);
            it = next;
        }
    }

    auto begin() const { return page_.begin();}
    auto end() const { return page_.end();}
    size_t size() const { return page_.size();}

private:
    std::vector<IteratorRange<Iterator>> page_;
};

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(std::begin(c), std::end(c), page_size);
}
