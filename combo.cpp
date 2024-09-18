#include <vector>
#include <set>
#include <iostream>
#include <random>
namespace combo
{
	using namespace std;

	// Normal distribution -> "https://stackoverflow.com/a/13445752"
	float randomf() 
	{
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist(20,180);
		return (float) ((float)dist(rng))/100.0;
	}
	void addCheck(const vector<vector<float>> &matrix, set<pair<int,int>> &checked, float &bag, const int i, const int j, const int nil) //ignore mess
	{
		if(checked.insert({i, j}).second)
		{
			bag += matrix[i][j];
			//cout << "added " << i << " " << j << endl;
		}
		else
		{
			//cout << "out of bounds, so adding nil" << endl;
			bag += nil;
		}
	}

	//1to1 modified discrete convolution
	vector<vector<float>> d_convolve(vector<vector<float>> matrix, float nil, const float airTemp, const float airTransfer)
	{
		vector<vector<float>> r = matrix;
		int side = matrix.size();
		for(int i = 0; i < side; ++i)
		{
			for(int j = 0; j < side; ++j) //overflows on all sides
			{
				set<pair<int,int>> checked;
				float avg = 0;
				addCheck(matrix, checked, avg, i, j, nil);
				bool w,a,s,d;
				w = i!=0; a = j!=0; s = i!=side-1; d = j!=side-1;
				vector<pair<int,int>> combination = {{i-w,j-a},{i-w,j+d},{i-w,j},{i,j-a},{i,j+d},{i+s,j-a},{i+s,j+d},{i+s,j}};
				for(auto c : combination)
				{
					addCheck(matrix, checked, avg, c.first, c.second, nil);
				}
				avg /= 9;
				r[i][j] = avg - (avg-airTemp)*airTransfer*randomf(); //MODLINE
			}
		}
		return r;
	}

	void print(const vector<vector<float>> &matrix)
	{
		for(auto i : matrix)
		{
			for(auto j : i)
			{
				cout << j << " ";	
			}
			cout << endl;
		}
		cout << endl;
	}

	void log(const vector<vector<float>> &matrix, string &timeLog)
	{
		timeLog += "$map1 << EOD\n";
		for(auto i : matrix)
		{
			for(auto j : i)
			{
				timeLog += to_string(j)+" ";	
			}
			timeLog += "\n";
		}
		timeLog += "EOD\n";

		timeLog += "plot $map1 matrix with image\n";

	}
};
