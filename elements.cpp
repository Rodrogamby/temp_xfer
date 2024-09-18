#include <utility>
#include "combo.h"
#include <iostream>
#include <vector>
#include <cfloat>

namespace combo
{
	class Burner
	{
		private:
			float airTransfer = .65; //different from rest of the system
			float temp = 15;
			float trgt = 15;
			/*
			 * k has been set so that the burner reaches 99.9%
			 * of the selected temp. in ~257 iterations. (~2.57seconds for current delta)
			 */
			const float k = .025;
			const float deadT = 1.5; // Dead time (seconds/speed) 1.5

			float time = 0;
			float lastOn = -100;
		public:
			void put(float t)
			{
				if(t <= this->trgt)
				{
					this->trgt = t;
				}
				else if(time-lastOn >= deadT)
				{
					this->trgt = t;
					this->lastOn = this->time;
				}
			}

			void next(const float& delta)
			{
				float step = trgt-temp;
				temp += step > 0 ? step*k*randomf() : step*k*airTransfer*randomf();
				time += delta;
			}

			float getTemp()
			{
				return temp;
			}
	};

	using namespace std;
	class Heatplate
	{
		private:
			vector<vector<float>> matrix;
			size_t plateSize;
			vector<pair<int,int>> sensor_locations = {{15, 11},{25, 19}};
			combo::Burner flame;

		public:
			Heatplate(size_t plateSize, float iniTemp)
			{
				this->plateSize = plateSize;
				vector<float> row(plateSize, iniTemp);
				while(plateSize--)
				{
					matrix.push_back(row);
				}
			}

			float readTemperature()
			{
				float sum = 0;
				for(auto i : sensor_locations)
				{
					sum += matrix[i.first][i.second];
				}
				return (sum)/((float)sensor_locations.size());
			}

			vector<vector<float>>* getArray()
			{
				return &matrix;
			}
	};

	class ControlPolicy
	{
		private:
			void logPeaks()
			{
				static pair<float,float> last = {0.0, 0.0};
				static pair<float,float> lastV = {0.0, FLT_MAX};
				static int i,j = 0;
				const int thrs = 40; //sensitivity

				float ctemp = heatplate->readTemperature();
				if(ctemp > last.second)
				{
					last = {time, ctemp};
					i = 0;
				}
				else
				{
					++i;
				}
				if(i == thrs)
				{
					peaks.push_back(last);
					j=0;
					last = {time,ctemp};
				}

				if(ctemp < lastV.second)
				{
					lastV = {time, ctemp};
					j = 0;
				}
				else
				{
					++j;
				}
				if(j == thrs)
				{
					lastValley = lastV;
					lastV = {time,ctemp};
				}
			}

		protected:
			combo::Heatplate* heatplate;
			combo::Burner* burner;
			vector<vector<float>>* matrix;
			float trgt_T = 24;
			float delta;
			float time = 0;
			float airT = 15;
			float airTransfer = .005;
			int hotTemp = 60;
			string log = "";
			FILE* tmp_log;
			FILE* brn_log;
			vector<pair<float,float>> peaks;
			pair<float,float> lastValley; //assumed to be in steady state

			void save()
			{
				combo::log(*matrix, this->log);
				fprintf(brn_log, "%f %f\n", time, burner->getTemp());
				fprintf(tmp_log, "%f %f\n", time, heatplate->readTemperature());
				logPeaks();
			}

		public:
			ControlPolicy(combo::Heatplate& hp, combo::Burner& burner, float delta, FILE* tmp_log, FILE* brn_log)
			{
				this->heatplate = &hp;
				this->burner = &burner;
				this->matrix = heatplate->getArray();
				this->delta = delta;
				this->tmp_log = tmp_log;
				this->brn_log = brn_log;
				save();
			}
			virtual void next() = 0;
			void setAirTemp(float t)
			{
				this->airT = t;
			}
			void setTarget(float t)
			{
				this->trgt_T = t;
			}
			string getLog()
			{
				return log;
			}
			float getAmplitude()
			{
				if(peaks.size()) return peaks[peaks.size()-1].second - lastValley.second;
				return 0;
			}
			float getPeriod()
			{
				if(peaks.size()>1) return peaks[peaks.size()-1].first - peaks[peaks.size()-2].first;
				return 0;
			}
			float getTT()
			{
				if(peaks.size()) return peaks[0].first;
				return 0;
			}
	};

	class BangBang : public ControlPolicy
	{
		public:
			using ControlPolicy::ControlPolicy;
			void next()
			{
				if(heatplate->readTemperature() > trgt_T+1) burner->put(this->airT);
				if(heatplate->readTemperature() < trgt_T-1) burner->put(hotTemp);
				burner->next(delta);
				*matrix = combo::d_convolve(*matrix, burner->getTemp(), airT, airTransfer);
				time += delta;
				save();
			}
	};

	class CustomControl : public ControlPolicy
	{
		public:
			using ControlPolicy::ControlPolicy;
			void next()
			{
				const float k1 = .12*airT;
				const float k2 = .03*hotTemp;
				if(heatplate->readTemperature() > trgt_T+ k1) burner->put(this->airT);
				if(heatplate->readTemperature() < trgt_T+ k2) burner->put(hotTemp);
				burner->next(delta);
				*matrix = combo::d_convolve(*matrix, burner->getTemp(), airT, airTransfer);
				time += delta;
				save();
			}
	};
};
