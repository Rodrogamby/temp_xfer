#include <vector>
#include <iostream>
#include <utility>
#include "combo.h"
#include "elements.cpp"
using namespace std;

int main()
{
	//Data files for plotting
	FILE* vecbrn = fopen("a.dat","w"); 
	FILE* vectmp = fopen("b.dat","w");
	FILE* veclin = fopen("z.dat", "w");
	FILE* gnu = fopen("script.gnu", "a");

	float delta = .01; //seconds
	int N = 4000;
	
	float tgt = 24.0;

	fprintf(veclin,"0 %f\n%f %f\n", tgt, delta*N, tgt);
	fclose(veclin);

	combo::Heatplate plate(40, 0);
	combo::Burner burner;
	combo::CustomControl control(plate, burner, delta, vecbrn, vectmp);

	control.setTarget(tgt);

	//Main loop
	for(int i = 0; i < N; i++)
	{
		control.next();
	}

	fclose(vecbrn);
	fclose(vectmp);

	fprintf(gnu,"%s",control.getLog().c_str());
	fclose(gnu);

	printf("Periodo de oscilación: %f seg\n", control.getPeriod());
	printf("Histéresis real: +-%f °\n", control.getAmplitude()/2.0f);
	printf("Tiempo en estado transitorio: %f seg\n", control.getTT());

	return 0;
}
