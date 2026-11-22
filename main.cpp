#include <iostream>
#include <ctime>
#include <string>
#include <utility>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>

using std::cout;
using std::cin;
using std::string;
using std::vector;
using std::pair;

/*
    Descripcion: Lee el log ordenado por IP destino (resultado.txt), cuenta los
    accesos de cada IP en un vector de pair<ip, int>, lo convierte en heap con
    make_heap y despliega las N IPs con mas accesos usando pop_heap.

    Este codigo estará detallladamente comentado para una mejor colaboración
    de equipo.
    
    Miembros Del Equipo:
    José Angel Gabriel Carrillo Gamboa A01802775
    José Maria Avilés Díaz  A01803596

    Fecha de modificacion: 21/11/2026

    Link En Línea: https://www.jdoodle.com/ga/uygy-y39e-mvyb
*/

//Estructuras Pre Hechas De Las Anteriores Actividades

// Struct representing an IP address
struct ip {
    int o1;
    int o2;
    int o3;
    int o4;
};

// Struct representing a log event (connection between two nodes)
// ts: Date-time of the event as ctime's tm (check docs: https://cplusplus.com/reference/ctime/tm/)
// ip_o: IP of the origin node
// port_o: Port of the origin node
// domain_o: Domain of the origin node
// ip_d: IP of the destination node
// port_d: Port of the destination node
// domain_d: Domain of the destination node
struct event {
  struct std::tm ts;
  struct ip ip_o;
  string port_o;
  string domain_o;
  struct ip ip_d;
  string port_d;
  string domain_d;
};

// Overload of << operator for struct ip
// If IP starts with 0 (o1 == 0) prints '-' instead
std::ostream& operator<<(std::ostream &os, const ip &i) {
    if(i.o1 == 0) {
        os << "-";
    } else {
       os << i.o1 << "." << i.o2 << "." << i.o3 << "." << i.o4;   
    }
    return os;
}

// Overload of << operator for struct event
// Prints event using same format as log
std::ostream& operator<<(std::ostream &os, const event &e) {
    char date_output[20];
    strftime(date_output, 20, "%d-%m-%Y,%T", &e.ts);
    os << date_output << "," << e.ip_o << "," << e.port_o << "," 
		<< e.domain_o << "," << e.ip_d << "," << e.port_d << "," 
		<< e.domain_d;
    return os;
}

// Overload of << operator for pair<ip, int>
std::ostream& operator<<(std::ostream &os, const pair<ip, int> &p) {
    os << p.first << ": " << p.second; // ip: count
    return os;
}

/*
    Primer Apartado Para Trabajar: Sobre Carga De Operador

*/
// Overload of < operator for pair<ip, int>
// Will be used by make_heap to determine max value
// Should compare pairs based on their counts
bool operator<(const pair<ip, int> &p1, const pair<ip, int> &p2) {
    return p1.second < p2.second;
}

/*
    Segundo Apartado Para Trabajar: El Proyecto Es Directamente En El Main

*/
int main() {
    // Declare vector of pairs of <ip, int>
    vector<pair<ip, int>> v{};
    // Open the log
    std::ifstream archivo("resultado.txt");
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo.\n";
        return 1;
    }

    string linea = "";
    while (getline(archivo, linea)) {
        if (!linea.empty()) {
            // Saltamos los primeros 5 campos; 'campo' termina con el 6to: la IP destino
            std::stringstream ss(linea);
            string campo = "";
            for (int i = 0; i < 6; ++i) {
                getline(ss, campo, ',');
            }

            // Convertimos el texto "o1.o2.o3.o4" a struct ip ('sep' se traga los puntos)
            ip ipDestino{0, 0, 0, 0};
            if (campo != "-") {
                char sep = ' ';
                std::stringstream ssIp(campo);
                ssIp >> ipDestino.o1 >> sep >> ipDestino.o2 >> sep >> ipDestino.o3 >> sep >> ipDestino.o4;
            }

            // For each unique destination ip create a pair<ip, int>
                // First element of pair will store destination ip
                // Second element of pair will store counter for that ip
                // Remember, log is already sorted by ip, so counting should be easy
            // Store each pair in a vector
            // Si es la misma IP que la anterior sumamos 1; si es nueva, agregamos un par (ip, 1)
            if (!v.empty() &&
                v.back().first.o1 == ipDestino.o1 &&
                v.back().first.o2 == ipDestino.o2 &&
                v.back().first.o3 == ipDestino.o3 &&
                v.back().first.o4 == ipDestino.o4) {
                v.back().second++;
            } else {
                v.push_back({ipDestino, 1});
            }
        }
    }
    archivo.close();

    // Rearrange the vector into a heap
    std::make_heap(v.begin(), v.end());

    // Ask the user for a number N
    int n = 0;
    cout << "Ingresa cuantas IPs quieres ver: ";
    while (!(cin >> n) || n <= 0) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Valor invalido, ingresa un numero mayor a 0: ";
    }
    if (n > (int)v.size()) {
        n = v.size();
    }

    // Show the N ips with the highest counter
    // front lee la raiz (mas accesos), pop_heap la manda al final y pop_back la elimina
    cout << "\nTop " << n << " IPs con mas accesos:\n";
    for (int i = 0; i < n; ++i) {
        cout << i + 1 << ". " << v.front() << "\n";
        std::pop_heap(v.begin(), v.end());
        v.pop_back();
    }

    return 0;
}