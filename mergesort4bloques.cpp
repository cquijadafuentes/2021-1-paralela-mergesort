#include <iostream>
#include <algorithm>
#include <ctime>
#include <chrono>

using namespace std;

float mergesort4(float* arr, int n);
void mergesortRec4(float* arr, int l, int r);
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
	if(argc < 2){
		cout << "Uso: " << argv[0] << " n [r]" << endl;
		cout << "n: tamaño del arreglo" << endl;
		cout << "r: repeticiones" << endl;
		return -1;
	}
	int rep = 1;
	if(argc > 2){
		rep = (int) atoi(argv[2]);
	}
	int n = (int) atoi(argv[1]);
	float* arr = new float[n];
	srand((unsigned)time(0));
	float milli = 0.0;
	for(int i=0; i<rep; i++){
		for(int i=0; i<n; i++){
			arr[i] = rand();
		}
		milli += mergesort4(arr, n);
	}
	delete[] arr;
	milli = milli/rep;
	cout << milli << " [ms] ";
	if(rep == 1){
		cout << endl;
	}
	return 0;
}

float mergesort4(float* arr, int n){
//	revisar(arr, n);
	auto start = chrono::high_resolution_clock::now();
	/********************************/
	mergesortRec4(arr, 0, n-1);
	/********************************/
	auto finish = chrono::high_resolution_clock::now();
//	revisar(arr, n);
	auto tiempo = chrono::duration_cast<chrono::milliseconds>(finish-start).count();
//	cout << tiempo << " [ms]" << endl;
	return tiempo+0.0;
}

void mergesortRec4(float* arr, int l, int r){
	// En casos en los que la cantidad de elementos es baja, se usa un sort tradicional
	// https://stackoverflow.com/questions/12590621/optimizing-mergesort
	if(r-l < 64){
		sort(arr+l, arr+r+1);
		return;
	}

	// m es un cuarto de la porción de arr entre l y r
	int m = (r-l+1)/4;
	// Hacer merge entre todos los bloques
	mergesortRec4(arr, l, l+m);
	mergesortRec4(arr, l+m+1,l+(2*m));
	mergesortRec4(arr, l+(2*m)+1,l+(3*m));
	mergesortRec4(arr, l+(3*m)+1,r);
	// Hacer mezcla de a dos cuartos
	merge(arr, l, l+(2*m), l+m+1);
	merge(arr, l+(2*m)+1, r, l+(3*m)+1);
	// Mezclar las dos mitades ya ordenadas
	merge(arr, l, r, l+(2*m)+1);
}

void merge(float* arr, int l, int r, int m){
	// Mezcla las dos mitades del segmento de arr entre las posiciones l y r (inclusives)
	// La primera mitda comienza en l y la segunda en m
	int* aar = new int[r-l+1];
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