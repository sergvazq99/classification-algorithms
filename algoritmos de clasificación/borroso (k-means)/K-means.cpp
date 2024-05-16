#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

struct flor {
    double v_1;
    double v_2;
    double v_3;
    double v_4;
    string nombre;
};

vector<flor> leerDatos(const string& archivo) {
    ifstream file(archivo);
    vector<flor> flores;
    string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream ss(line);
            flor f;
            string value;

            getline(ss, value, ',');
            f.v_1 = stod(value);
            getline(ss, value, ',');
            f.v_2 = stod(value);
            getline(ss, value, ',');
            f.v_3 = stod(value);
            getline(ss, value, ',');
            f.v_4 = stod(value);
            getline(ss, f.nombre);

            flores.push_back(f);
        }
        file.close();
    } else {
        cout << "Error al abrir el archivo: " << archivo << endl;
    }

    return flores;
}

double distancia(const flor& a, const flor& b) {
    return sqrt(pow(a.v_1 - b.v_1, 2) + pow(a.v_2 - b.v_2, 2) +
                pow(a.v_3 - b.v_3, 2) + pow(a.v_4 - b.v_4, 2));
}

vector<flor>inicializarCentroides(vector<flor>& flores, int k) {
    vector<flor> centroides;
    size_t n = flores.size();

    if (k > n) {
        throw std::runtime_error("El numero de centroides no puede ser mayor que el numero de puntos.");
    }

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    vector<int> indices(n);
    for (int i = 0; i < n; ++i) {
        indices[i] = i;
    }
    //std::random_shuffle(indices.begin(), indices.end());

    for (int i = 0; i < k; ++i) {
        centroides.push_back(flores[indices[i]]);
    }

    return centroides;
}

void borroso(vector<flor>& flores, int k) {
    vector<flor> centroides = inicializarCentroides(flores, k);
    vector<int> asignaciones(flores.size(), -1);
    bool cambio;
    int iteraciones = 0, maxIteraciones = 100;  

    do {
        cambio = false;
        iteraciones++;

     
        for (size_t i = 0; i < flores.size(); ++i) {
            double minDist = numeric_limits<double>::max();
            int idxCentroide = -1;
            for (int j = 0; j < k; ++j) {
                double dist = distancia(flores[i], centroides[j]);
                if (dist < minDist) {
                    minDist = dist;
                    idxCentroide = j;
                }
            }
            if (idxCentroide != asignaciones[i]) {
                asignaciones[i] = idxCentroide;
                cambio = true;
            }
        }

        
        vector<flor> nuevosCentroides(k);
        vector<int> conteos(k, 0);
        for (size_t i = 0; i < flores.size(); ++i) {
            flor& centroide = nuevosCentroides[asignaciones[i]];
            centroide.v_1 += flores[i].v_1;
            centroide.v_2 += flores[i].v_2;
            centroide.v_3 += flores[i].v_3;
            centroide.v_4 += flores[i].v_4;
            conteos[asignaciones[i]]++;
        }
        for (int j = 0; j < k; ++j) {
            if (conteos[j] > 0) {
                centroides[j].v_1 = nuevosCentroides[j].v_1 / conteos[j];
                centroides[j].v_2 = nuevosCentroides[j].v_2 / conteos[j];
                centroides[j].v_3 = nuevosCentroides[j].v_3 / conteos[j];
                centroides[j].v_4 = nuevosCentroides[j].v_4 / conteos[j];
            }
        }

        if (iteraciones >= maxIteraciones) {
            cout << "Se alcanzo el numero maximo de iteraciones." << endl;
            break;
        }
    } while (cambio);

    cout << "Centroides finales despues de " << iteraciones << " iteraciones:" << endl << "--------------------------------------------------------------------\n";
    for (const flor& c : centroides) {
        cout << c.v_1 << ", " << c.v_2 << ", " << c.v_3 << ", " << c.v_4 << endl;
    }
}


vector<flor> combinarDatos(const vector<flor>& datos1, const vector<flor>& datos2) {
    vector<flor> combinados = datos1;
    combinados.insert(combinados.end(), datos2.begin(), datos2.end());
    return combinados;
}

int main() {
    cout << "Nombre del primer archivo (sin extension): ";
    string fileName1;
    cin >> fileName1;
    vector<flor> datos1 = leerDatos(fileName1 + ".txt");

    vector<flor> datos2 = leerDatos("Iris2Clases.txt");

    vector<flor> flores = combinarDatos(datos1, datos2);

    int k;
    cout << "Introduce el numero de clusters: ";
    cin >> k;

    borroso(flores, k);

    return 0;
}

