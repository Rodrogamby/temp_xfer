#include <vector>
#include <set>
#include <iostream>
namespace combo
{
	std::vector<std::vector<float>> d_convolve(std::vector<std::vector<float>> matrix, float nil, const float airTemp, const float airTransfer);
	void print(const std::vector<std::vector<float>> &matrix);	
	void log(const std::vector<std::vector<float>> &matrix, std::string &timeLog);
	float randomf();
};

