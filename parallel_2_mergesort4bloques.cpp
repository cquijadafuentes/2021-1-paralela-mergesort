#include <iostream>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <omp.h>

using namespace std;

float mergesort4(float* arr, int n, int th);
void mergesortRec4Par(float* arr, int l, int r, int th);
void mergesortRec4Sec(float* arr, int l, int r);
void merge(float* arr, int l, int r, int m);
void revisar(float* arr, int n){
	int x = 0;
	for(int i=1; i<n; i++){
		if(arr[i] < arr[i-1]){
			x++;
		}
	}
	cout << x << " incumplimientos" <<  endl;
}

int main(int argc, char const *argv[]){
	if(argc < 3){
		cout << "Uso: " << argv[0] << " n th [r]" << endl;
		cout << "n: tamaño del arreglo" << endl;
		cout << "th: número aproximado de hebras a crear" << endl;
		cout << "r: repeticiones" << endl;
		return -1;
	}
	int n = (int) atoi(argv[1]);
	int th = (int) atoi(argv[2]);
	int rep = 1;
	if(argc > 3){
		rep = (int) atoi(argv[3]);
	}
	float* arr = new float[n];
	srand((unsigned)time(0));
	float milli = 0.0;
	for(int i=0; i<rep; i++){
		for(int i=0; i<n; i++){
			arr[i] = rand();
		}
		milli += (float)mergesort4(arr, n, th);
	}
	delete[] arr;
	milli = milli/rep;
	cout << milli << " [ms] ";
	if(rep == 1){
		cout << endl;
	}
	return 0;
}

float mergesort4(float* arr, int n, int th){
	// En este método se divide el arreglo en 4 bloques que se ejecutan por 4 hebras
	// Posteriormente, 2 hebras se preocupan de realizar el merge de cada mitad
//	revisar(arr, n);
	auto start = chrono::high_resolution_clock::now();
	/********************************/
	omp_set_num_threads(th);
	cout << "(" << "th:" << th << ")";
	mergesortRec4Par(arr, 0, n-1, th);
	/********************************/
	auto finish = chrono::high_resolution_clock::now();
//	revisar(arr, n);
	auto tiempo = chrono::duration_cast<chrono::milliseconds>(finish-start).count();
//	cout << tiempo << " [ms]" << endl;
	return tiempo+0.0;
}

void mergesortRec4Par(float* arr, int l, int r, int th){
	// En casos en los que la cantidad de elementos es baja, se usa un sort tradicional
	// https://stackoverflow.com/questions/12590621/optimizing-mergesort
	if(r-l < 64){
		sort(arr+l, arr+r+1);
		return;
	}

	// Limitación en el número de hebras a crear según th
	if(th <= 1){
		mergesortRec4Sec(arr, l, r);
		return;
	}

	// m es un cuarto de la porción de arr entre l y r
	int m = (r-l+1)/4;
	int thcuarto = th / 4;
	// Flags para las dependencias en el paralelismo
	short int a,b,c,d,e,f;
	a=0;
	b=0; 
	c=0;
	d=0;
	e=0;
	// Hacer merge entre todos los bloques
	#pragma omp parallel
	{
		#pragma omp single
		{
			#pragma omp task depend(out:a)
			{
				// Sort del primer cuarto
				mergesortRec4Par(arr, l, l+m, thcuarto);
				a = 1;
			}
			#pragma omp task depend(out:b)
			{
				// Sort del segundo cuarto
				mergesortRec4Par(arr, l+m+1, l+(2*m), thcuarto);
				b = 2;
			}
			#pragma omp task depend(out:c)
			{
				// Sort del tercer cuarto
				mergesortRec4Par(arr, l+(2*m)+1, l+(3*m), thcuarto);
				c=3;
			}
			#pragma omp task depend(out:d)
			{
				// Sort del último cuarto
				mergesortRec4Par(arr, l+(3*m)+1, r, th - 3*thcuarto);
				d=4;
			}
			#pragma omp task depend(in:a,b) depend(out:e)
			{
				// Merge de los dos primeros cuartos
				merge(arr, l, l+2*m, l+m+1);
				e=5;
			}
			#pragma omp task depend(in:c,d)
			{
				// Merge de los dos últimos cuartos
				merge(arr, l+2*m+1, r, l+(3*m)+1);
			}
		}
	}
	merge(arr, l, r, l+(2*m)+1);
}

void mergesortRec4Sec(float* arr, int l, int r){
	// En casos en los que la cantidad de elementos es baja, se usa un sort tradicional
	// https://stackoverflow.com/questions/12590621/optimizing-mergesort
	if(r-l < 64){
		sort(arr+l, arr+r+1);
		return;
	}

	// m es un cuarto de la porción de arr entre l y r
	int m = (r-l+1)/4;
	// Hacer merge entre todos los bloques
	mergesortRec4Sec(arr, l, l+m);
	mergesortRec4Sec(arr, l+m+1,l+(2*m));
	mergesortRec4Sec(arr, l+(2*m)+1,l+(3*m));
	mergesortRec4Sec(arr, l+(3*m)+1,r);
	// Hacer mezcla de a dos cuartos
	merge(arr, l, l+(2*m), l+m+1);
	merge(arr, l+(2*m)+1, r, l+(3*m)+1);
	// Mezclar las dos mitades ya ordenadas
	merge(arr, l, r, l+(2*m)+1);
}

void merge(float* arr, int l, int r, int m){
	// Mezcla las dos mitades del segmento de arr entre las posiciones l y r (inclusives)
	// La primera mitda comienza en l y la segunda en m
	float* aar = new float[r-l+1];
	int pl = l;
	int pr = m;
	int paar = 0;
	// Recorrer ambas mitadas
	while(pl < m && pr <= r){
		if(arr[pl] < arr[pr]){
			aar[paar++] = arr[pl++];
		}else{
			aar[paar++] = arr[pr++];
		}
	}
	// Cuando alguna de las dos mitades procesa todo, quedan elementos en la otra
	// Sólo uno de los dos whiles que siguen son ejecutados
	// Elementos restantes de la primera mitad
	while(pl < m){
		aar[paar++] = arr[pl++];
	}
	// Elementos restantes de la segunda mitad
	while(pr <= r){
		aar[paar++] = arr[pr++];
	}
	// Ubicar los valores en el arreglo original
	int j = 0;
	for(int i=l; i<=r; i++){
		arr[i] = aar[j++];
	}
	delete[] aar;
}