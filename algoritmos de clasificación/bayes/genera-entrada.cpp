#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <unordered_map>
using namespace std;

const int DIMENSIONES_X = 4;

int main() {
	ifstream in("Iris2Clases.txt");
	auto cinbuf = std::cin.rdbuf(in.rdbuf());

	set<string> clases;

	//La primera componente es x, la segunda la clase a la que pertenece
	vector<pair<vector<double>, string>> x;

	double d;
	char c;
	string s;
	int dimension_actual = 1, n = 0;
	int num_ejemplo = 0;
	bool primer_ejemplo = true;
	string primera_clase; //Usado para obtener n.

	while (cin) {

		//Crear nuevo espacio en el vector si es un nuevo ejemplo.
		if (dimension_actual == 1) {
			pair<vector<double>, string> p;
			x.push_back(p);
		}

		if (dimension_actual <= DIMENSIONES_X) {
			//Leer el valor
			cin >> d;

			//Meter el valor en x
			x.at(num_ejemplo).first.push_back(d);

			//Pasar la coma
			cin >> c;

			dimension_actual++;
		}
		//Leemos la clase a la que pertenece
		else if (dimension_actual == DIMENSIONES_X + 1) {
			//Leer la clase
			cin >> s;

			//Meter el valor en x
			x.at(num_ejemplo).second = s;

			//Obtener primera_clase si es el primer ejemplo
			if (primer_ejemplo) {
				primer_ejemplo = false;
				primera_clase = s;
			}

			//No admite repetidos
			clases.insert(s);

			//Si la clase es primera_clase, aumentar el valor de n
			if (s == primera_clase) n++;

			//Limpiar la entrada y pasar a siguiente ejemplo
			cin.getline(&c, 1);
			num_ejemplo++;
			dimension_actual = 1;
		}
		else break;
	}

	for (string s : clases) { cout << s << endl; }
	cout << "FIN-CLASES" << endl;

	unordered_map<string, vector<vector<double>>> mapa;

	for (string clase : clases) {
		vector<vector<double>> v;
		mapa[clase] = v;
	}

	for (pair<vector<double>, string> par : x) {
		string clase = par.second;
		mapa[clase].push_back(par.first);
	}

	for (auto entrada : mapa) {
		cout << entrada.first << endl;

		for (vector<double> v : entrada.second) {
			for (int i = 0; i < v.size(); i++) {
				cout << v.at(i);
				if (i != v.size() - 1) cout << ", ";
			}
			cout << ";" << endl;
		}

	}

	return 0;
}