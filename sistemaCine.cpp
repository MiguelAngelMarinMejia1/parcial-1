#include <iostream>
#include <vector>
#include <ctime> // Biblioteca usada para saber fecha actual
#include <limits> // Biblioteca usadada para evitar que se ingrese un dato no pedido
#include <queue> // Biblioteca usada para las colas

using namespace std;

struct peliculas{
    string nombre;
    string descripcion;
    string genero;
    int resEdAd;
};

struct usuarios {
    int id;                      
    string nombre;                  
    vector<pair<int,int>> reservas; // Lista de reservas hechas (fila,columna) 
};


struct funciones {
    string fecha;
    string hora;
    vector<vector<string>> asientos; // cada función tiene sus asientos propios
    queue<usuarios> colaEspera = {}; // Usuarios en espera
};

struct horarios {
    peliculas pelicula;
    vector<funciones> funciones;
};


// Se declaran unas variables globales ya que se usarán en algunas funciones por defecto
vector<peliculas> listaPeliculas;
vector<horarios> listaHorarios;
vector<usuarios> listaUsuarios;

void mostrarMenuPrincipal();
void mostrarPeliculas();
void validarIngresoNumerico(int &opc);
void crearAsientos(vector<vector<string>> &asiento);
void mostrarAsientos(vector<vector<string>> &asiento);
horarios crearHorario(peliculas peli, vector<string> &fechas, vector<string> &horas);
bool reservarAsiento(funciones &func, string asiento, usuarios &usuario);
void cancelarReserva(funciones &func, int usuarioID);

int main(){


    // Se obtiene la fecha actual para luego añadir a funciones
    time_t ahora = time(0);
    tm* tiempoLocal = localtime(&ahora);
    int d = tiempoLocal->tm_mday;
    int m = tiempoLocal->tm_mon + 1;
    int a = tiempoLocal->tm_year + 1900; 
    int contador = 0;
    vector<string> fechasDisponibles;
    vector<string> horariosDisponibles {"14:00", "15:30", "17:00", "19:30"};

    // Luego se muestra las fechas disponibles en ese mismo mes hasta maximo 5 dias despues, se evita el 31 para evitar errores
    do{
        fechasDisponibles.push_back(to_string(a) + "-" + (m < 10? "0" + to_string(m): to_string(m)) + "-" + (d < 10? "0" + to_string(d): to_string(d)));
        d++;
        contador++;
        if(contador == 5){
            break;
        };
    } while (d != 30);

    // Estas son las peliculas disponibles, si se añade una pelicula se debe crear nuevos horario;
    peliculas p1 = {"El conjuro 4 (GRAN ESTRENO)","Los Warren enfrentan el caso mas aterrador de sus carreras, con una entidad que supera todo lo conocido.","Terror",16};
    peliculas p2 = {"Los pitufos 5 (GRAN ESTRENO)","Una nueva aventura lleva a los Pitufos fuera de su aldea para salvar la magia del bosque de una amenaza inesperada.","Animacion/Familiar", 0};
    peliculas p3 = {"F1 la pelicula","Un piloto novato lucha por alcanzar la cima del automovilismo en la Formula 1, enfrentando velocidad, rivalidades y gloria.","Accion/Deportes",7};
    peliculas p4 = {"SuperMan","El ultimo hijo de Krypton debe proteger la Tierra de un enemigo que amenaza la paz mundial, mientras enfrenta su lado humano.","Accion/Fantasia",12};
    peliculas p5 = {"BatMan","Gotham esta sumida en el caos y solo el Caballero Oscuro puede descubrir la verdad detrás de una ola de crimenes.","Accion/Thriller",12};
    peliculas p6 = {"Lilo y Stitch","Una niña hawaiana adopta lo que cree que es un perro, sin saber que en realidad es un experimento extraterrestre fugitivo.","Animacion/Familiar",0};
    
    listaPeliculas = {p1, p2 , p3, p4, p5, p6};

    // Se crean los horarios para cada pelicula

    listaHorarios.push_back(crearHorario(p1, fechasDisponibles, horariosDisponibles));
    listaHorarios.push_back(crearHorario(p2, fechasDisponibles, horariosDisponibles));
    listaHorarios.push_back(crearHorario(p3, fechasDisponibles, horariosDisponibles));
    listaHorarios.push_back(crearHorario(p4, fechasDisponibles, horariosDisponibles));
    listaHorarios.push_back(crearHorario(p5, fechasDisponibles, horariosDisponibles));
    listaHorarios.push_back(crearHorario(p6, fechasDisponibles, horariosDisponibles));

    // Aqui se empieza a ejecutar el codigo que se va a mostrar por pantalla
    cout<<"\n | CINE |\n";
    mostrarMenuPrincipal();

    return 0;
}

void validarIngresoNumerico(int &opc, int tam){  
    while (true){
        if (!(cin>>opc) || opc > tam || opc < 1){
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout<<"\n"<<"Opcion no valida, intente de nuevo: ";
            continue;
        }
        break;
    }
}

void mostrarMenuPrincipal(){
    int eleccion;

    cout<<"\nQue deseas hacer?\n";
    cout<<"1: Ver peliculas\n";
    cout<<"2: Cancelar reservas\n";
    cout<<"3: Salir\n";
    
    validarIngresoNumerico(eleccion, 3);

    if (eleccion == 1) {
        mostrarPeliculas();
    } else if(eleccion == 2){
        int id;
        cout << "\nIngresa tu ID para cancelar tu(s) reserva(s): ";
        validarIngresoNumerico(id, 1000000000);

        // Mostrar todas las películas y funciones disponibles
        cout << "\nSelecciona la pelicula y funcion donde deseas cancelar:\n";
        for (int i = 0; i < listaHorarios.size(); i++) {
            cout << i+1 << ". " << listaHorarios[i].pelicula.nombre << "\n";
            for (int j = 0; j < listaHorarios[i].funciones.size(); j++) {
                cout << "   " << j+1 << ") " << listaHorarios[i].funciones[j].hora << "\n";
            }
        }

        int peli, func;
        cout << "\nNumero de pelicula: ";
        validarIngresoNumerico(peli, listaHorarios.size());
        cout << "Numero de funcion: ";
        validarIngresoNumerico(func, listaHorarios[peli-1].funciones.size());

        // Cancelar reserva en esa función
        cancelarReserva(listaHorarios[peli-1].funciones[func-1], id);
    } else{
        cout<<"Hasta luego, vuelve pronto :)";
    }

}

void mostrarPeliculas(){
    int eleccionPelicula;
    int eleccionAccion;
    int cantidadPeliculas = listaPeliculas.size(); 

    cout<<"\n | PELICULAS DISPONIBLES |\n\n";

    for (int i = 0; i < cantidadPeliculas; i++){
        cout<<i + 1<< ": "<<listaPeliculas[i].nombre<<"\n";
    }

    cout<<"\nEscoge la pelicula que te interesa: ";
    validarIngresoNumerico(eleccionPelicula, cantidadPeliculas);

    cout<<"\nQue deseas hacer con esta pelicula?\n";
    cout<<"1: Ver funciones disponibles\n";
    cout<<"2: Ver detalles\n";
    cout<<"3: Volver al menu principal\n";
    validarIngresoNumerico(eleccionAccion, 3);

    if(eleccionAccion == 1){
        horarios h = listaHorarios[eleccionPelicula-1];
        cout<<"\nFunciones disponibles para: "<<h.pelicula.nombre<<"\n";
        for (int i = 0; i < h.funciones.size(); i++) {
            cout<<i+1<<". Fecha: "<<h.funciones[i].fecha<<" | Hora: "<<h.funciones[i].hora<<"\n";
        }
        cout<<"\nSelecciona una funcion para ver los asientos: ";
        int eleccionFuncion;
        validarIngresoNumerico(eleccionFuncion, listaHorarios[eleccionPelicula-1].funciones.size());
        mostrarAsientos(listaHorarios[eleccionPelicula-1].funciones[eleccionFuncion-1].asientos);

        cout << "\nQuieres reservar un asiento?\n";
        cout << "1: Si\n";
        cout << "2: No, volver al menu principal\n";
        int resp;
        validarIngresoNumerico(resp, 2);

        if (resp == 1) {
            string asiento;
            usuarios usuario;

            // El usuario debe poner el identificador numérico
            cout << "Ingresa tu id: ";
            validarIngresoNumerico(usuario.id, 100000000000);
            
            cout << "Ingresa tu nombre: ";
            getline(cin>>ws ,usuario.nombre);

            // Guardar usuario si no existe

            bool verificador = true;
            while(verificador){
                
                cout << "Ingresa asiento (ejemplo 2-3): ";
                cin >> asiento;

                verificador = reservarAsiento(listaHorarios[eleccionPelicula-1].funciones[eleccionFuncion-1], asiento, usuario);
            }

            bool existe = false;
            for (auto &u : listaUsuarios) {
                if (u.id == usuario.id) {
                    existe = true;
                    break;
                }
            }
            if (!existe) listaUsuarios.push_back(usuario);
        }
        
        cout << "\nVolviendo al menu principal...\n";
        mostrarMenuPrincipal();
        
    } else if(eleccionAccion == 2){
        peliculas p = listaPeliculas[eleccionPelicula-1];
        cout<<"\n | DETALLES | \n\n";
        cout<<"Titulo: "<<p.nombre<<"\n";
        cout<<"Descripcion: "<<p.descripcion<<"\n";
        cout<<"Genero: "<<p.genero<<"\n";
        cout<<"Restriccion de edad: "<<p.resEdAd<<"+\n";

        cout << "\nDesea volver al menu principal?\n";
        cout << "1: Si\n";
        cout << "2: No, salir completamente\n ";
        int resp;
        validarIngresoNumerico(resp, 2);

        if (resp == 1){
            mostrarMenuPrincipal();
        } else {
            cout<<"\nHasta luego, vuelve pronto :)";
        }
        

    } else if(eleccionAccion == 3){
        mostrarMenuPrincipal();
    }
    
}

void crearAsientos(vector<vector<string>> &asientos){
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 8; j++){
            asientos[i][j] = to_string(i + 1) + "-" + to_string(j + 1);
        }   
    }
}

void mostrarAsientos(vector<vector<string>> &asiento){

    cout<<"\n | Asientos disponibles | \n\n";
    cout<<"Los asientos marcados con X no estan disponibles \n\n";

    
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 8;j++){
            std::cout<<asiento[i][j]<<" | ";
        }
        std::cout<<"\n";
    }
};

horarios crearHorario(peliculas peli, vector<string> &fechas, vector<string> &horas) {
    horarios horario;
    horario.pelicula = peli;

    for (string f : fechas) {
        for (string h : horas) {
            funciones func;
            func.fecha = f;
            func.hora = h;

            func.asientos = vector<vector<string>>(5, vector<string>(8));
            crearAsientos(func.asientos);

            horario.funciones.push_back(func);
        }
    }
    return horario;
}

bool reservarAsiento(funciones &func, string asiento, usuarios &usuario) {
    for (int i = 0; i < func.asientos.size(); i++) {
        for (int j = 0; j < func.asientos[i].size(); j++) {
            string nombreAsiento = to_string(i+1) + "-" + to_string(j+1);

            if (nombreAsiento == asiento) {
                if (func.asientos[i][j] != "X") {
                    func.asientos[i][j] = "X";  
                    usuario.reservas.push_back({i,j}); // Se guarda la reserva del usuario

                    cout << "\nReserva confirmada para en asiento " << asiento << "\n";

                    return false; // Para que el bucle en el que está pare
                } else {
                    cout << "\nAsiento ocupado!\n";
                    cout << "\nQuieres estar en lista de espera?\n";
                    cout << "1: Si\n";
                    cout << "2: No, volver al menu principal\n ";
                    int resp;
                    validarIngresoNumerico(resp, 2);

                    if (resp == 1) {
                        func.colaEspera.push(usuario);
                        cout << "\nListo, estas en lista de espera!\n";
                    }
                    return false; // Para que el bucle en el que está pare
                }
            }
        }
    }
    cout << "El asiento no existe.\n";
    return true; // Para que el bucle continue y el usuario pueda ingresar algo
}


void cancelarReserva(funciones &func, int usuarioID) {

    bool usuarioEncontrado = false;

    // Buscar usuario en la lista
    for (usuarios &u : listaUsuarios) {
        if (u.id == usuarioID) {

            usuarioEncontrado = true;

            if (u.reservas.empty()) {

                cout << "El usuario " << u.nombre << " no tiene reservas activas.\n";
                break;
            }

            // Liberar los asientos que tenía reservados
            for (auto &res : u.reservas) {
                int i = res.first;
                int j = res.second;

                cout<<i<<endl;
                cout<<j<<endl;

                func.asientos[i][j] = to_string(i + 1) + "-" + to_string(j + 1); // En caso de que no haya cola de espera
                
                // Ver si hay alguien en la cola de espera
                if (!func.colaEspera.empty()) {
                    usuarios espera = func.colaEspera.front();
                    func.colaEspera.pop();

                    func.asientos[i][j] = "X"; // ahora ocupado por alguien de la cola
                    espera.reservas.push_back({i,j});

                    // Actualizar listaUsuarios
                    bool existe = false;
                    for (auto &usr : listaUsuarios) {
                        if (usr.id == espera.id) {
                            usr.reservas.push_back({i,j});
                            existe = true;
                            break;
                        }
                    }

                    if (!existe) {
                        listaUsuarios.push_back(espera);
                    }

                    cout << "El asiento " << i + 1 << "-" << j + 1 << " fue asignado a alguien que estaba en cola de espera\n";
                }
                break;
            }

            cout << "Se cancelaron " << u.reservas.size() << " reservas del usuario " << u.nombre << " (ID: " << u.id << ").\n";

            // Vaciar reservas del usuario
            u.reservas.clear();

        }
    }

    if (!usuarioEncontrado){
        cout << "No se encontro ningun usuario con ID " << usuarioID << ".\n";
    }
    
    mostrarMenuPrincipal();
}