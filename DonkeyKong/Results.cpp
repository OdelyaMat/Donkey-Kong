#include <limits>
#include <fstream>

#include "Results.h"

Results Results::loadResults(const std::string& filename) {
	std::ifstream results_file(filename);
	Results results;
	size_t size;
	//results_file >> size;
	while (!results_file.eof()/* && size-- != 0*/) {
		size_t iteration;
		int result;
		results_file >> iteration >> result;
		results.addResult(iteration, static_cast<ResultValue>(result));
	}
	return results;
}

void Results::saveResults(const std::string& filename) const {
	std::ofstream results_file(filename);
	//results_file << results.size();
	for (const auto& result : results) {
		results_file << '\n' << result.first << ' ' << result.second;
	}
	results_file.close();
}

size_t Results::getNextBombIteration() const {
	if (!results.empty() && results.front().second == hitEnemy) {
		return results.front().first;
	}
	else return std::numeric_limits<size_t>::max(); // a big number we will never reach
}

bool Results::validateResult(size_t iteration, ResultValue expected) {
	if (results.empty()) return false;
	return results.front().first == iteration && results.front().second == expected;
}

 std::string Results::ResultValueToString(ResultValue result) {
	switch (result) {
	case ResultValue::hitEnemy: return "Hit Enemy";
	case ResultValue::finished: return "Not finished";
	case ResultValue::noResult: return "No Result";
	default: return "Invalid Result";
	}
}