#pragma once

#include <list>


class Steps {
	long randomSeed = 0;
	std::list<std::pair<size_t, char>> steps; // pair: iteration, step
public:
	static Steps loadSteps(const std::string& filename);
	void saveSteps(const std::string& filename) const;
	bool isEmpty() const { return steps.empty(); }
	long getRandomSeed() const {
		return randomSeed;
	}
	void setRandomSeed(long seed) {
		randomSeed = seed;
	}
	void addStep(size_t iteration, char step) {
		steps.push_back({ iteration, step });
	}
	bool isNextStepOnIteration(size_t iteration) const {
		return !steps.empty() && steps.front().first == iteration;
	}
	char popStep() {
		char step = steps.front().second;
		steps.pop_front();
		return step;
	}
	std::list<std::pair<size_t, char>> getSteps() {
		return steps;
	}
	void clear() {
		steps.clear();  // Clear the list of recorded steps
	}

};