#pragma once

#include <list>

class Results {
public:
	enum ResultValue { hitEnemy, finished, noResult, SCORE };
private:
	std::list<std::pair<size_t, ResultValue>> results; // pair: iteration, result
public:
	static Results loadResults(const std::string& filename);
	void saveResults(const std::string& filename) const;
	void addResult(size_t iteration, ResultValue result) {
		results.push_back({ iteration, result });
	}
	bool isEmpty() const {
		return results.empty();
	}
	std::pair<size_t, ResultValue> popResult() {
		if (results.empty()) return { 0, Results::noResult };
		auto result = results.front();
		results.pop_front();
		return result;
	}
	bool isFinishedBy(size_t iteration) const {
		return results.empty() || results.back().first <= iteration;
	}
	size_t getNextBombIteration() const;
	bool validateResult(size_t iteration, ResultValue expected);
	auto begin() { return results.begin(); }
	auto end() { return results.end(); }

	std::list<std::pair<size_t, ResultValue>> getResults(){
		return results;
	}
	void clear() {
		results.clear();  // Clear the list of recorded steps
	}
	static std::string ResultValueToString(ResultValue result);

};
