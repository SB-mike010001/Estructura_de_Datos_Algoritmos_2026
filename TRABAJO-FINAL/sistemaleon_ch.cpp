
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <cctype>
#include <limits>
#include <map>

using namespace std;

static string trim(const string &s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

static string lowerText(string s) {
    for (char &c : s) c = (char)tolower((unsigned char)c);
    return s;
}

static string nowText() {
    time_t t = time(nullptr);
    tm *lt = localtime(&t);
    char buf[40];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", lt);
    return string(buf);
}

static bool isExitText(string s) {
    s = lowerText(trim(s));
    return s == "0" || s == "salir" || s == "volver" || s == "cancelar" || s == "cancel";
}

static bool parseInt(const string &s, int &out) {
    try {
        string t = trim(s);
        if (t.empty()) return false;
        size_t pos = 0;
        int v = stoi(t, &pos);
        if (pos != t.size()) return false;
        out = v;
        return true;
    } catch (...) { return false; }
}

static bool parseDouble(const string &s, double &out) {
    try {
        string t = trim(s);
        replace(t.begin(), t.end(), ',', '.');
        if (t.empty()) return false;
        size_t pos = 0;
        double v = stod(t, &pos);
        if (pos != t.size()) return false;
        out = v;
        return true;
    } catch (...) { return false; }
}

static string money(double v) {
    stringstream ss;
    ss << fixed << setprecision(2) << "S/ " << v;
    return ss.str();
}


struct LocalInfo {
    string nombre;
    string zona;
};

struct Usuario {
    string usuario;
    string clave;
    string rol;
    string local;
    string zona;
    string nombrePersona;
};

struct Producto {
    int id;
    string codigo;
    string codigoBarras;
    string nombre;
    string marca;
    string categoria;
    string presentacion;
    double costo;
    double precioMenor;
    double precioMayor;
    int stockMinimo;
    vector<int> stock;
    string fechaVencimiento;
    bool activo;
};

struct Cliente {
    int id;
    string nombre;
    string tipoDoc;
    string numeroDoc;
    string telefono;
};

struct Pago {
    string metodo;
    double monto;
};

struct DetalleVenta {
    int productoId;
    string codigo;
    string nombre;
    int cantidad;
    double precioUnitario;
    double subtotal;
};

struct Venta {
    int id;
    string fecha;
    int localIndex;
    string local;
    string cajero;
    Cliente cliente;
    vector<DetalleVenta> detalles;
    vector<Pago> pagos;
    double subtotal;
    double descuento;
    bool conIGV;
    double igv;
    double total;
};

struct Auditoria {
    int id;
    string fecha;
    string usuario;
    string rol;
    string local;
    string zona;
    string accion;
    string detalle;
    string datoAnterior;
    string datoNuevo;
    string comentario;
    string estado;
};

struct SolicitudProducto {
    int id;
    string fecha;
    string local;
    string cliente;
    string producto;
    string cantidad;
    string observacion;
};

struct Combo {
    int id;
    string nombre;
    double precio;
    vector<int> productoIds;
    bool activo;
};

class SistemaVentas {
private:
    vector<LocalInfo> locales;
    vector<Usuario> usuarios;
    vector<Producto> productos;
    vector<Cliente> clientes;
    vector<Venta> ventas;
    vector<Auditoria> auditorias;
    vector<SolicitudProducto> solicitudes;
    vector<Combo> combos;
    Usuario sesion;
    bool logged = false;
    int nextCliente = 1;
    int nextVenta = 1;
    int nextAuditoria = 1;
    int nextSolicitud = 1;
    int nextCombo = 1;

public:
    SistemaVentas() {
        cargarLocales();
        cargarUsuarios();
        cargarProductosBase();
        cargarCombosBase();
    }

    void ejecutar() {
        cout << endl;
        cout << "==============================================" << endl;
        cout << "  SISTEMA INTEGRAL LICORERIA LEON - C++17" << endl;
        cout << "  Ventas | Inventario | Multilocal | Auditoria" << endl;
        cout << "==============================================" << endl;
        cout << "Pantalla de acceso cargada correctamente." << endl << endl;
        cout.flush();
        if (!login()) return;
        menuPrincipal();
        cout << endl << "Gracias por usar el sistema Licoreria Leon." << endl;
    }

private:
    void cargarLocales() {
        locales.push_back({"Tingo Maria", "Zona Alto Huallaga"});
        locales.push_back({"Aucayacu", "Zona Alto Huallaga"});
        locales.push_back({"Tocache", "Zona Alto Huallaga"});
        locales.push_back({"Aguaytia", "Zona Federico Basadre"});
        locales.push_back({"San Alejandro", "Zona Federico Basadre"});
        locales.push_back({"Km 86", "Zona Federico Basadre"});
    }

    void addUser(const string &u, const string &c, const string &rol, const string &local, const string &zona, const string &nombrePersona = "") {
        string nombre = nombrePersona;
        if (nombre.empty()) {
            if (rol == "Gerente general") nombre = "Gerente General";
            else if (rol == "Supervisor zonal") nombre = "Supervisor de " + zona;
            else nombre = rol + " - " + local;
        }
        usuarios.push_back({u, c, rol, local, zona, nombre});
    }

    void cargarUsuarios() {
        addUser("gerente", "731", "Gerente general", "TODOS", "TODAS");
        addUser("supervisor1", "428", "Supervisor zonal", "ZONA", "Zona Alto Huallaga");
        addUser("supervisor2", "596", "Supervisor zonal", "ZONA", "Zona Federico Basadre");

        addUser("admin_tingo", "314", "Administrador", "Tingo Maria", "Zona Alto Huallaga");
        addUser("cajero_tingo1", "647", "Cajero", "Tingo Maria", "Zona Alto Huallaga");
        addUser("cajero_tingo2", "618", "Cajero", "Tingo Maria", "Zona Alto Huallaga");
        addUser("cajero_tingo3", "729", "Cajero", "Tingo Maria", "Zona Alto Huallaga");
        addUser("cajero_tingo_aux", "540", "Cajero auxiliar", "Tingo Maria", "Zona Alto Huallaga");
        addUser("almacenero", "209", "Almacenero", "Tingo Maria", "Zona Alto Huallaga");

        addUser("admin_aucayacu", "852", "Administrador", "Aucayacu", "Zona Alto Huallaga");
        addUser("cajero_aucayacu1", "371", "Cajero", "Aucayacu", "Zona Alto Huallaga");
        addUser("cajero_aucayacu2", "482", "Cajero", "Aucayacu", "Zona Alto Huallaga");
        addUser("cajero_aucayacu3", "593", "Cajero", "Aucayacu", "Zona Alto Huallaga");
        addUser("cajero_aucayacu_aux", "604", "Cajero auxiliar", "Aucayacu", "Zona Alto Huallaga");
        addUser("almac_aucayacu", "715", "Almacenero", "Aucayacu", "Zona Alto Huallaga");

        addUser("admin_tocache", "763", "Administrador", "Tocache", "Zona Alto Huallaga");
        addUser("cajero_tocache1", "826", "Cajero", "Tocache", "Zona Alto Huallaga");
        addUser("cajero_tocache2", "937", "Cajero", "Tocache", "Zona Alto Huallaga");
        addUser("cajero_tocache3", "148", "Cajero", "Tocache", "Zona Alto Huallaga");
        addUser("cajero_tocache_aux", "259", "Cajero auxiliar", "Tocache", "Zona Alto Huallaga");
        addUser("almac_tocache", "360", "Almacenero", "Tocache", "Zona Alto Huallaga");

        addUser("admin_aguaytia", "491", "Administrador", "Aguaytia", "Zona Federico Basadre");
        addUser("cajero_aguaytia1", "471", "Cajero", "Aguaytia", "Zona Federico Basadre");
        addUser("cajero_aguaytia2", "582", "Cajero", "Aguaytia", "Zona Federico Basadre");
        addUser("cajero_aguaytia3", "693", "Cajero", "Aguaytia", "Zona Federico Basadre");
        addUser("cajero_aguaytia_aux", "704", "Cajero auxiliar", "Aguaytia", "Zona Federico Basadre");
        addUser("almac_aguaytia", "815", "Almacenero", "Aguaytia", "Zona Federico Basadre");

        addUser("admin_sanalejandro", "286", "Administrador", "San Alejandro", "Zona Federico Basadre");
        addUser("cajero_sanalejandro1", "926", "Cajero", "San Alejandro", "Zona Federico Basadre");
        addUser("cajero_sanalejandro2", "137", "Cajero", "San Alejandro", "Zona Federico Basadre");
        addUser("cajero_sanalejandro3", "248", "Cajero", "San Alejandro", "Zona Federico Basadre");
        addUser("cajero_sanalejandro_aux", "359", "Cajero auxiliar", "San Alejandro", "Zona Federico Basadre");
        addUser("almac_sanalejandro", "460", "Almacenero", "San Alejandro", "Zona Federico Basadre");

        addUser("admin_km86", "934", "Administrador", "Km 86", "Zona Federico Basadre");
        addUser("cajero_km861", "571", "Cajero", "Km 86", "Zona Federico Basadre");
        addUser("cajero_km862", "682", "Cajero", "Km 86", "Zona Federico Basadre");
        addUser("cajero_km863", "793", "Cajero", "Km 86", "Zona Federico Basadre");
        addUser("cajero_km86_aux", "804", "Cajero auxiliar", "Km 86", "Zona Federico Basadre");
        addUser("almac_km86", "915", "Almacenero", "Km 86", "Zona Federico Basadre");
    }

    bool validarSeguridadUsuarios() {
        for (size_t i = 0; i < usuarios.size(); ++i) {
            if (usuarios[i].usuario == usuarios[i].clave) return false;
            for (size_t j = i + 1; j < usuarios.size(); ++j) {
                if (usuarios[i].usuario == usuarios[j].usuario) return false;
                if (usuarios[i].clave == usuarios[j].clave) return false;
            }
        }
        return true;
    }

    bool login() {
        if (!validarSeguridadUsuarios()) {
            cout << "ERROR DE SEGURIDAD: hay usuarios o claves repetidas.\n";
            return false;
        }
        cout << "==============================================\n";
        cout << "              ACCESO AL SISTEMA\n";
        cout << "==============================================\n";
        cout << "Ingrese sus credenciales autorizadas.\n";
        cout << "Escriba 0 en usuario para salir." << endl << endl;
        cout.flush();
        for (int intento = 1; intento <= 3; ++intento) {
            string u, c;
            cout << "Usuario: " << flush;
            getline(cin, u);
            u = trim(u);
            if (isExitText(u)) return false;
            if (u.empty()) { cout << "Usuario obligatorio.\n"; continue; }
            cout << "Clave: " << flush;
            getline(cin, c);
            c = trim(c);
            if (isExitText(c)) return false;
            for (const auto &usr : usuarios) {
                if (usr.usuario == u && usr.clave == c) {
                    sesion = usr;
                    logged = true;
                    cout << "\nIngreso correcto. Bienvenido: " << sesion.usuario << " | " << sesion.rol << " | " << sesion.local << "\n";
                    pausar();
                    return true;
                }
            }
            cout << "Credenciales incorrectas. Intento " << intento << " de 3.\n";
        }
        cout << "Acceso bloqueado por intentos fallidos.\n";
        return false;
    }

    void pausar() {
        cout << "\nPresione ENTER para continuar...";
        string tmp;
        getline(cin, tmp);
    }

    int pedirOpcion(int minv, int maxv) {
        while (true) {
            string s;
            cout << "Opcion: ";
            getline(cin, s);
            s = trim(s);
            if (s.empty()) return 0;
            int v;
            if (parseInt(s, v) && v >= minv && v <= maxv) return v;
            cout << "Entrada invalida. Ingrese un numero entre " << minv << " y " << maxv << ".\n";
        }
    }

    string pedirTexto(const string &msg, bool obligatorio = false) {
        while (true) {
            string s;
            cout << msg;
            getline(cin, s);
            s = trim(s);
            if (!obligatorio || !s.empty()) return s;
            cout << "Este dato es obligatorio. Puede escribir 0 para cancelar.\n";
        }
    }

    double pedirDouble(const string &msg, double minv, bool permitirCeroSalir = true) {
        while (true) {
            string s = pedirTexto(msg, false);
            if (permitirCeroSalir && isExitText(s)) return -1.0;
            double v;
            if (parseDouble(s, v) && v >= minv) return v;
            cout << "Numero invalido. Intente nuevamente.\n";
        }
    }

    int pedirEntero(const string &msg, int minv, int maxv, bool permitirCeroSalir = true) {
        while (true) {
            string s = pedirTexto(msg, false);
            if (permitirCeroSalir && isExitText(s)) return 0;
            int v;
            if (parseInt(s, v) && v >= minv && v <= maxv) return v;
            cout << "Numero invalido. Rango permitido: " << minv << " a " << maxv << ".\n";
        }
    }

    bool puedeVerLocal(int idx) const {
        if (idx < 0 || idx >= (int)locales.size()) return false;
        if (sesion.rol == "Gerente general") return true;
        if (sesion.rol == "Supervisor zonal") return sesion.zona == locales[idx].zona;
        return sesion.local == locales[idx].nombre;
    }

    int localIndexPorNombre(const string &nom) const {
        for (int i = 0; i < (int)locales.size(); ++i) if (locales[i].nombre == nom) return i;
        return -1;
    }

    int seleccionarLocalAccesible() {
        vector<int> opciones;
        cout << "\nLocales disponibles:\n";
        for (int i = 0; i < (int)locales.size(); ++i) {
            if (puedeVerLocal(i)) {
                opciones.push_back(i);
                cout << "  " << (int)opciones.size() << ". " << locales[i].nombre << " | " << locales[i].zona << "\n";
            }
        }
        cout << "  0. Volver\n";
        int op = pedirOpcion(0, (int)opciones.size());
        if (op == 0) return -1;
        return opciones[op - 1];
    }

    void registrarAuditoria(const string &accion, const string &detalle, const string &ant, const string &nuevo, const string &comentario, const string &estado = "Pendiente de revision") {
        Auditoria a;
        a.id = nextAuditoria++;
        a.fecha = nowText();
        a.usuario = sesion.usuario;
        a.rol = sesion.rol;
        a.local = sesion.local;
        a.zona = sesion.zona;
        a.accion = accion;
        a.detalle = detalle;
        a.datoAnterior = ant;
        a.datoNuevo = nuevo;
        a.comentario = comentario;
        a.estado = estado;
        auditorias.push_back(a);
    }

    bool pedirComentarioCritico(const string &accion, string &comentario) {
        cout << "\nACCION CRITICA: " << accion << "\n";
        cout << "Antes de continuar debe escribir el motivo.\n";
        cout << "Aviso: esta accion sera registrada, enviada al gerente general y al supervisor zonal.\n";
        cout << "Responsabilidad: la accion es de completa responsabilidad del administrador o usuario que la ejecuta.\n";
        while (true) {
            comentario = pedirTexto("Comentario obligatorio (0 para cancelar): ", true);
            if (isExitText(comentario)) return false;
            if (comentario.size() >= 8) return true;
            cout << "El comentario debe ser mas claro. Minimo 8 caracteres.\n";
        }
    }

    void menuPrincipal() {
        while (true) {
            cout << "\n==============================================\n";
            cout << " SISTEMA LICORERIA LEON - MENU PRINCIPAL\n";
            cout << " Usuario: " << sesion.usuario << " | Rol: " << sesion.rol << "\n";
            cout << "==============================================\n";
            cout << "1. Productos\n";
            cout << "2. Clientes\n";
            cout << "3. Ventas\n";
            cout << "4. Comprobantes\n";
            cout << "5. Stock por local\n";
            cout << "6. Stock general\n";
            cout << "7. Transferencias entre locales\n";
            cout << "8. Combos\n";
            cout << "9. Productos solicitados por clientes\n";
            cout << "10. Compras y proveedores\n";
            cout << "11. Facturas de compra con IA simulada\n";
            cout << "12. Codigos de barras\n";
            cout << "13. Reportes y Excel CSV\n";
            cout << "14. Control de vencimientos FEFO\n";
            cout << "15. Usuarios y permisos\n";
            cout << "16. Auditoria de acciones\n";
            cout << "17. Inteligencia artificial basica\n";
            cout << "18. Comandos por voz simulados\n";
            cout << "19. Camaras y control visual\n";
            cout << "20. Configuracion y credenciales\n";
            cout << "21. Guardar informacion\n";
            cout << "22. Recuperar informacion\n";
            cout << "0. Salir\n";
            int op = pedirOpcion(0, 22);
            if (op == 0) break;
            switch (op) {
                case 1: menuProductos(); break;
                case 2: menuClientes(); break;
                case 3: menuVentas(); break;
                case 4: menuComprobantes(); break;
                case 5: reporteStockPorLocal(); break;
                case 6: reporteStockGeneral(); break;
                case 7: menuTransferencias(); break;
                case 8: menuCombos(); break;
                case 9: menuSolicitudes(); break;
                case 10: menuComprasProveedores(); break;
                case 11: menuFacturasIA(); break;
                case 12: menuCodigos(); break;
                case 13: menuReportesCSV(); break;
                case 14: menuVencimientos(); break;
                case 15: menuUsuarios(); break;
                case 16: menuAuditoria(); break;
                case 17: menuIA(); break;
                case 18: menuVoz(); break;
                case 19: menuCamaras(); break;
                case 20: menuConfiguracion(); break;
                case 21: guardarInformacion(); break;
                case 22: recuperarInformacion(); break;
            }
        }
    }

    void agregarProductoBase(const string &codigo, const string &barra, const string &nombre, const string &marca, const string &categoria, const string &presentacion, double costo, double menor, double mayor) {
        Producto p;
        p.id = (int)productos.size() + 1;
        p.codigo = codigo;
        p.codigoBarras = barra;
        p.nombre = nombre;
        p.marca = marca;
        p.categoria = categoria;
        p.presentacion = presentacion;
        p.costo = costo;
        p.precioMenor = menor;
        p.precioMayor = mayor;
        p.stockMinimo = 5;
        p.stock.assign(locales.size(), 0);
        for (int i = 0; i < (int)locales.size(); ++i) {
            p.stock[i] = 8 + ((p.id * (i + 3)) % 25);
        }
        p.fechaVencimiento = "2027-12-31";
        p.activo = true;
        productos.push_back(p);
    }

    void cargarProductosBase() {
        agregarProductoBase("P0001", "775100000001", "Johnnie Walker Red Label", "Johnnie", "Whisky", "750 ml", 35.50, 43.00, 37.40);
        agregarProductoBase("P0002", "775100000002", "Johnnie Walker Black Label", "Johnnie", "Whisky", "750 ml", 82.50, 99.00, 86.90);
        agregarProductoBase("P0003", "775100000003", "Johnnie Walker Double Black", "Johnnie", "Whisky", "750 ml", 108.50, 126.00, 114.30);
        agregarProductoBase("P0004", "775100000004", "Johnnie Walker Gold Label / Dorado", "Johnnie", "Whisky", "750 ml", 165.50, 191.00, 174.30);
        agregarProductoBase("P0005", "775100000005", "Johnnie Walker Green Label", "Johnnie", "Whisky premium", "750 ml", 220.50, 254.00, 232.20);
        agregarProductoBase("P0006", "775100000006", "Johnnie Walker 18 anos", "Johnnie", "Whisky premium", "750 ml", 0.00, 0.00, 0.00);
        agregarProductoBase("P0007", "775100000007", "Buchanan's 12 anos", "Buchanan's", "Whisky", "750 ml", 92.50, 113.00, 97.40);
        agregarProductoBase("P0008", "775100000008", "Chivas Regal 12 anos", "Chivas", "Whisky", "750 ml", 65.50, 80.00, 69.00);
        agregarProductoBase("P0009", "775100000009", "Chivas Regal 13 anos", "Chivas", "Whisky", "750 ml", 72.50, 87.00, 76.40);
        agregarProductoBase("P0010", "775100000010", "Chivas Regal 15 anos", "Chivas", "Whisky premium", "750 ml", 178.50, 211.00, 187.90);
        agregarProductoBase("P0011", "775100000011", "Chivas Regal 18 anos", "Chivas", "Whisky premium", "750 ml", 185.50, 219.00, 195.30);
        agregarProductoBase("P0012", "775100000012", "Chivas Royal Salute 21 anos", "Chivas", "Whisky premium", "700/750 ml", 650.50, 768.00, 684.80);
        agregarProductoBase("P0013", "775100000013", "Old Parr 12 anos", "Old", "Whisky", "750 ml", 76.50, 92.00, 80.60);
        agregarProductoBase("P0014", "775100000014", "Ballantine's Finest", "Ballantine's", "Whisky", "700/750 ml", 37.00, 47.00, 39.00);
        agregarProductoBase("P0015", "775100000015", "Something Special", "Something", "Whisky", "750 ml", 42.50, 54.00, 44.80);
        agregarProductoBase("P0016", "775100000016", "Swing", "Swing", "Whisky premium", "750 ml", 170.50, 202.00, 179.50);
        agregarProductoBase("P0017", "775100000017", "Passport Scotch", "Passport", "Whisky", "750 ml", 32.00, 40.00, 33.70);
        agregarProductoBase("P0018", "775100000018", "Flor de Cana 4 anos", "Flor", "Rones", "750 ml", 30.50, 40.00, 32.20);
        agregarProductoBase("P0019", "775100000019", "Flor de Cana 5 anos", "Flor", "Rones", "750 ml", 37.50, 48.00, 39.50);
        agregarProductoBase("P0020", "775100000020", "Flor de Cana 5 anos", "Flor", "Rones", "1 L", 0.00, 0.00, 0.00);
        agregarProductoBase("P0021", "775100000021", "Flor de Cana 7 anos", "Flor", "Rones", "750 ml", 52.50, 66.00, 55.30);
        agregarProductoBase("P0022", "775100000022", "Flor de Cana 12 anos", "Flor", "Rones", "750 ml", 84.50, 104.00, 89.00);
        agregarProductoBase("P0023", "775100000023", "Flor de Cana 18 anos", "Flor", "Ron premium", "750 ml", 128.50, 157.00, 135.30);
        agregarProductoBase("P0024", "775100000024", "Bacardi Blanco", "Bacardi", "Rones", "1 L", 39.50, 50.00, 41.60);
        agregarProductoBase("P0025", "775100000025", "Bacardi", "Bacardi", "Rones", "1.75 L", 0.00, 0.00, 0.00);
        agregarProductoBase("P0026", "775100000026", "Barcelo Dorado", "Barcelo", "Rones", "750 ml", 26.50, 34.00, 27.90);
        agregarProductoBase("P0027", "775100000027", "Barcelo Anejo", "Barcelo", "Rones", "750 ml", 32.50, 42.00, 34.30);
        agregarProductoBase("P0028", "775100000028", "Barcelo Gran Anejo", "Barcelo", "Rones", "750 ml", 36.50, 47.00, 38.50);
        agregarProductoBase("P0029", "775100000029", "Barcelo Anejo", "Barcelo", "Rones", "1.75 L", 65.50, 82.00, 69.00);
        agregarProductoBase("P0030", "775100000030", "Barcelo Gran Anejo", "Barcelo", "Rones", "1.75 L", 67.50, 85.00, 71.10);
        agregarProductoBase("P0031", "775100000031", "Barcelo Blanco", "Barcelo", "Rones", "750 ml", 28.50, 37.00, 30.00);
        agregarProductoBase("P0032", "775100000032", "Barcelo Blanco", "Barcelo", "Rones", "1 L", 41.00, 52.00, 43.20);
        agregarProductoBase("P0033", "775100000033", "Cartavio Blanco", "Cartavio", "Rones", "750 ml", 22.50, 30.00, 23.70);
        agregarProductoBase("P0034", "775100000034", "Cartavio 5 anos", "Cartavio", "Rones", "750 ml", 25.50, 33.00, 26.90);
        agregarProductoBase("P0035", "775100000035", "Cartavio Black", "Cartavio", "Rones", "750 ml", 22.25, 29.00, 23.50);
        agregarProductoBase("P0036", "775100000036", "Cartavio Black", "Cartavio", "Rones", "1 L", 23.35, 30.00, 24.60);
        agregarProductoBase("P0037", "775100000037", "Cartavio Blanco", "Cartavio", "Rones", "1.75 L", 0.00, 0.00, 0.00);
        agregarProductoBase("P0038", "775100000038", "Ron Medellin", "Ron", "Rones", "750 ml", 22.50, 29.00, 23.70);
        agregarProductoBase("P0039", "775100000039", "Ron Medellin", "Ron", "Rones", "1 L", 30.50, 39.00, 32.20);
        agregarProductoBase("P0040", "775100000040", "Havana Rubio", "Havana", "Rones", "750 ml", 28.50, 37.00, 30.00);
        agregarProductoBase("P0041", "775100000041", "Zacapa XO", "Zacapa", "Ron premium", "750 ml", 395.50, 455.00, 416.40);
        agregarProductoBase("P0042", "775100000042", "Zacapa 23 anos", "Zacapa", "Ron premium", "750 ml", 0.00, 0.00, 0.00);
        agregarProductoBase("P0043", "775100000043", "Pisco Vargas", "Pisco", "Piscos", "750 ml", 16.00, 22.00, 16.90);
        agregarProductoBase("P0044", "775100000044", "Pisco Vargas", "Pisco", "Piscos", "4 L", 69.50, 87.00, 73.20);
        agregarProductoBase("P0045", "775100000045", "Pisco Sello de Oro / Sello Rojo", "Pisco", "Piscos", "4 L", 38.50, 52.00, 40.60);
        agregarProductoBase("P0046", "775100000046", "Pisco Santiago Queirolo Acholado", "Pisco", "Piscos", "750 ml", 22.50, 29.00, 23.70);
        agregarProductoBase("P0047", "775100000047", "Pisco Santiago Queirolo Quebranta", "Pisco", "Piscos", "750 ml", 22.50, 29.00, 23.70);
        agregarProductoBase("P0048", "775100000048", "Pisco Santiago Queirolo Acholado", "Pisco", "Piscos", "2 L", 52.00, 65.00, 54.80);
        agregarProductoBase("P0049", "775100000049", "Pisco Tabernero", "Pisco", "Piscos", "2 L", 52.00, 65.00, 54.80);
        agregarProductoBase("P0050", "775100000050", "Pisco La Botija Quebranta", "Pisco", "Piscos", "750 ml", 22.50, 29.00, 23.70);
        agregarProductoBase("P0051", "775100000051", "Pisco La Botija Acholado", "Pisco", "Piscos", "700/750 ml", 25.40, 32.00, 26.80);
        agregarProductoBase("P0052", "775100000052", "Pisco Porton surtido", "Pisco", "Piscos", "750 ml", 65.50, 80.00, 69.00);
        agregarProductoBase("P0053", "775100000053", "Piscano de Maracuya", "Piscano", "Piscos", "750 ml", 14.00, 19.00, 14.80);
        agregarProductoBase("P0054", "775100000054", "Pisco Tabernero Pisco Sour Maracuya", "Pisco", "Piscos", "700 ml", 15.40, 21.00, 16.30);
        agregarProductoBase("P0055", "775100000055", "Cartavio Chata Black", "Cartavio", "Chatas/Petacas", "250 ml", 8.75, 12.00, 9.30);
        agregarProductoBase("P0056", "775100000056", "Cartavio Chata surtido", "Cartavio", "Chatas/Petacas", "250 ml", 8.75, 12.00, 9.30);
        agregarProductoBase("P0057", "775100000057", "Cartavio Petaca Black", "Cartavio", "Chatas/Petacas", "125 ml", 5.67, 8.00, 6.00);
        agregarProductoBase("P0058", "775100000058", "Cartavio Petaca surtido", "Cartavio", "Chatas/Petacas", "125 ml", 5.67, 8.00, 6.00);
        agregarProductoBase("P0059", "775100000059", "Conac Poblete chata", "Conac", "Chatas/Petacas", "250 ml", 6.25, 9.00, 6.60);
        agregarProductoBase("P0060", "775100000060", "Anis Najar Seco Especial petaca", "Anis", "Chatas/Petacas", "125 ml", 11.17, 15.00, 11.80);
        agregarProductoBase("P0061", "775100000061", "Anis Najar Semi Dulce Especial petaca", "Anis", "Chatas/Petacas", "125 ml", 11.17, 15.00, 11.80);
        agregarProductoBase("P0062", "775100000062", "Anis Najar Crema Especial petaca", "Anis", "Chatas/Petacas", "125 ml", 11.17, 15.00, 11.80);
        agregarProductoBase("P0063", "775100000063", "Tabernero Borgona", "Tabernero", "Vinos", "750 ml", 11.17, 16.00, 11.80);
        agregarProductoBase("P0064", "775100000064", "Tabernero Rose", "Tabernero", "Vinos", "750 ml", 11.17, 16.00, 11.80);
        agregarProductoBase("P0065", "775100000065", "Tabernero Gran Seleccion Borgona", "Tabernero", "Vinos", "750 ml", 14.00, 19.00, 14.80);
        agregarProductoBase("P0066", "775100000066", "Tabernero Gran Seleccion Rose", "Tabernero", "Vinos", "750 ml", 14.00, 19.00, 14.80);
        agregarProductoBase("P0067", "775100000067", "Espumante Tabernero Especial Rose", "Espumante", "Vinos", "750 ml", 12.17, 17.00, 12.90);
        agregarProductoBase("P0068", "775100000068", "Vino Tabernero 2 L surtido", "Vino", "Vinos", "2 L", 30.67, 39.00, 32.30);
        agregarProductoBase("P0069", "775100000069", "Tabernero Rose 2 L", "Tabernero", "Vinos", "2 L", 29.00, 37.00, 30.60);
        agregarProductoBase("P0070", "775100000070", "Santiago Queirolo Borgona", "Santiago", "Vinos", "750 ml", 11.50, 16.00, 12.20);
        agregarProductoBase("P0071", "775100000071", "Santiago Queirolo Rose", "Santiago", "Vinos", "750 ml", 11.50, 16.00, 12.20);
        agregarProductoBase("P0072", "775100000072", "Santiago Queirolo Blanco", "Santiago", "Vinos", "750 ml", 11.50, 16.00, 12.20);
        agregarProductoBase("P0073", "775100000073", "Santiago Queirolo Sangria", "Santiago", "Vinos", "750 ml", 12.25, 17.00, 12.90);
        agregarProductoBase("P0074", "775100000074", "Santiago Queirolo Borgona", "Santiago", "Vinos", "2 L", 31.00, 39.00, 32.70);
        agregarProductoBase("P0075", "775100000075", "Santiago Queirolo Rose", "Santiago", "Vinos", "2 L", 31.00, 39.00, 32.70);
        agregarProductoBase("P0076", "775100000076", "Santiago Queirolo Magdalena", "Santiago", "Vinos", "2 L", 31.00, 39.00, 32.70);
        agregarProductoBase("P0077", "775100000077", "Santiago Queirolo Borgona", "Santiago", "Vinos", "4 L", 61.50, 76.00, 64.80);
        agregarProductoBase("P0078", "775100000078", "Santiago Queirolo Rose", "Santiago", "Vinos", "4 L", 61.50, 76.00, 64.80);
        agregarProductoBase("P0079", "775100000079", "Santiago Queirolo Magdalena", "Santiago", "Vinos", "4 L", 61.50, 76.00, 64.80);
        agregarProductoBase("P0080", "775100000080", "Tacama Rose", "Tacama", "Vinos", "750 ml", 12.50, 17.00, 13.20);
        agregarProductoBase("P0081", "775100000081", "Tacama tinto semiseco", "Tacama", "Vinos", "750 ml", 12.50, 17.00, 13.20);
        agregarProductoBase("P0082", "775100000082", "Tacama Blanco Semiseco", "Tacama", "Vinos", "750 ml", 12.50, 17.00, 13.20);
        agregarProductoBase("P0083", "775100000083", "Tacama Gran Tinto", "Tacama", "Vinos", "750 ml", 18.50, 25.00, 19.50);
        agregarProductoBase("P0084", "775100000084", "Tacama Gran Blanco", "Tacama", "Vinos", "750 ml", 18.50, 25.00, 19.50);
        agregarProductoBase("P0085", "775100000085", "Casillero del Diablo surtido", "Casillero", "Vinos", "750 ml", 28.50, 36.00, 30.00);
        agregarProductoBase("P0086", "775100000086", "Vina Vieja lata surtido", "Vina", "Vinos", "lata x24", 6.75, 10.00, 7.20);
        agregarProductoBase("P0087", "775100000087", "Vina Vieja surtido", "Vina", "Vinos", "750 ml/caja", 108.00, 130.00, 113.70);
        agregarProductoBase("P0088", "775100000088", "Frontera surtido", "Frontera", "Vinos", "750 ml", 15.50, 21.00, 16.40);
        agregarProductoBase("P0089", "775100000089", "Gato Blanco", "Gato", "Vinos", "750 ml", 12.16, 17.00, 12.80);
        agregarProductoBase("P0090", "775100000090", "Primado Santiago Queirolo", "Primado", "Vinos", "750 ml", 12.50, 17.00, 13.20);
        agregarProductoBase("P0091", "775100000091", "Riccadonna surtido", "Riccadonna", "Vinos", "750 ml", 45.50, 57.00, 47.90);
        agregarProductoBase("P0092", "775100000092", "Nuvo", "Nuvo", "Vodka/Licores premium", "750 ml", 86.50, 109.00, 91.10);
        agregarProductoBase("P0093", "775100000093", "Baileys", "Baileys", "Vodka/Licores premium", "750 ml", 52.50, 66.00, 55.30);
        agregarProductoBase("P0094", "775100000094", "Baileys", "Baileys", "Vodka/Licores premium", "375 ml", 28.50, 38.00, 30.00);
        agregarProductoBase("P0095", "775100000095", "Jagermeister", "Jagermeister", "Vodka/Licores premium", "750 ml", 46.50, 59.00, 49.00);
        agregarProductoBase("P0096", "775100000096", "Jagermeister", "Jagermeister", "Vodka/Licores premium", "1 L", 59.50, 74.00, 63.70);
        agregarProductoBase("P0097", "775100000097", "Vodka Absolut Azul", "Vodka", "Vodka/Licores premium", "750 ml", 38.00, 50.00, 40.00);
        agregarProductoBase("P0098", "775100000098", "Gin Bombay", "Gin", "Gin", "750 ml", 64.50, 81.00, 67.90);
        agregarProductoBase("P0099", "775100000099", "Beefeater Pink", "Beefeater", "Gin", "750 ml", 62.50, 79.00, 65.80);
        agregarProductoBase("P0100", "775100000100", "Beefeater Classic", "Beefeater", "Gin", "750 ml", 54.50, 69.00, 57.40);
        agregarProductoBase("P0101", "775100000101", "Don Julio", "Don", "Tequila", "750 ml", 185.50, 219.00, 195.30);
        agregarProductoBase("P0102", "775100000102", "Olmeca", "Olmeca", "Tequila", "750 ml", 74.50, 91.00, 78.50);
        agregarProductoBase("P0103", "775100000103", "Tequila Cuervo Blanco", "Tequila", "Tequila", "750 ml", 52.50, 66.00, 55.30);
        agregarProductoBase("P0104", "775100000104", "Tequila Cuervo Reposado", "Tequila", "Tequila", "750 ml", 52.50, 66.00, 55.30);
        agregarProductoBase("P0105", "775100000105", "Blue Curacao licor", "Blue", "Licores", "unidad/caja x12", 16.00, 20.00, 16.90);
        agregarProductoBase("P0106", "775100000106", "Jarabe Curacao Wislan", "Jarabe", "Licores", "unidad caja x12", 8.00, 11.00, 8.50);
        agregarProductoBase("P0107", "775100000107", "Crema de coco Barclan", "Crema", "Licores", "unidad", 5.25, 8.00, 5.60);
        agregarProductoBase("P0108", "775100000108", "Anis Tonic Blanco", "Anis", "Licores", "750 ml", 12.50, 17.00, 13.20);
        agregarProductoBase("P0109", "775100000109", "Anis Najar Verde", "Anis", "Licores", "750 ml", 32.50, 42.00, 34.30);
        agregarProductoBase("P0110", "775100000110", "Noche Permanente", "Noche", "Licores", "pendiente", 8.00, 11.00, 8.50);
        agregarProductoBase("P0111", "775100000111", "Tritadona / Tridona S/Q", "Tritadona", "Licores", "750 ml", 18.50, 25.00, 19.50);
        agregarProductoBase("P0112", "775100000112", "Aguardiente Antioqueno Azul", "Aguardiente", "Aguardiente", "750 ml", 24.50, 32.00, 25.80);
        agregarProductoBase("P0113", "775100000113", "Aguardiente Antioqueno Rojo", "Aguardiente", "Aguardiente", "750 ml", 24.50, 32.00, 25.80);
        agregarProductoBase("P0114", "775100000114", "Aguardiente Antioqueno Verde", "Aguardiente", "Aguardiente", "750 ml", 24.50, 32.00, 25.80);
        agregarProductoBase("P0115", "775100000115", "Aguardiente Antioqueno presentacion chica", "Aguardiente", "Aguardiente", "pendiente", 4.00, 6.00, 4.30);
        agregarProductoBase("P0116", "775100000116", "Four Loko surtido", "Four", "Cervezas/RTD", "473 ml unidad", 9.14, 11.00, 9.70);
        agregarProductoBase("P0117", "775100000117", "Mike's Limon six pack", "Mike's", "Cervezas/RTD", "six pack", 24.50, 27.00, 25.80);
        agregarProductoBase("P0118", "775100000118", "Mike's Maracuya six pack", "Mike's", "Cervezas/RTD", "six pack", 24.50, 27.00, 25.80);
        agregarProductoBase("P0119", "775100000119", "Mike's Manzana six pack", "Mike's", "Cervezas/RTD", "six pack", 24.50, 27.00, 25.80);
        agregarProductoBase("P0120", "775100000120", "Mike's Arandano six pack", "Mike's", "Cervezas/RTD", "six pack", 24.50, 27.00, 25.80);
        agregarProductoBase("P0121", "775100000121", "San Juan lata six pack", "San", "Cervezas/RTD", "six pack", 21.50, 24.00, 22.70);
        agregarProductoBase("P0122", "775100000122", "San Juan cerveza caja", "San", "Cervezas/RTD", "caja", 68.00, 75.00, 71.60);
        agregarProductoBase("P0123", "775100000123", "Heineken lata six pack", "Heineken", "Cervezas/RTD", "six pack", 17.50, 20.00, 18.50);
        agregarProductoBase("P0124", "775100000124", "Heineken botella caja x24", "Heineken", "Cervezas/RTD", "caja x24", 98.00, 108.00, 103.20);
        agregarProductoBase("P0125", "775100000125", "Corona caja x24", "Corona", "Cervezas/RTD", "caja x24", 95.00, 105.00, 100.00);
        agregarProductoBase("P0126", "775100000126", "Coronita caja x24", "Coronita", "Cervezas/RTD", "caja x24", 65.00, 72.00, 68.50);
        agregarProductoBase("P0127", "775100000127", "Pilsen normal six pack", "Pilsen", "Cervezas/RTD", "six pack", 23.50, 26.00, 24.80);
        agregarProductoBase("P0128", "775100000128", "Pilsen laton six pack", "Pilsen", "Cervezas/RTD", "six pack", 28.50, 32.00, 30.00);
        agregarProductoBase("P0129", "775100000129", "Golden laton six pack", "Golden", "Cervezas/RTD", "six pack", 20.50, 23.00, 21.60);
        agregarProductoBase("P0130", "775100000130", "Cusquena Trigo six pack", "Cusquena", "Cervezas/RTD", "six pack", 28.50, 32.00, 30.00);
        agregarProductoBase("P0131", "775100000131", "Cusquena Trigo laton six pack", "Cusquena", "Cervezas/RTD", "six pack", 30.00, 33.00, 31.60);
        agregarProductoBase("P0132", "775100000132", "Cusquena Trigo caja", "Cusquena", "Cervezas/RTD", "caja", 78.00, 86.00, 82.20);
        agregarProductoBase("P0133", "775100000133", "Wild lata caja x24", "Wild", "Cervezas/RTD", "caja x24", 68.00, 75.00, 71.60);
        agregarProductoBase("P0134", "775100000134", "Hit lata caja x24", "Hit", "Cervezas/RTD", "caja x24", 68.00, 75.00, 71.60);
        agregarProductoBase("P0135", "775100000135", "Red Bull", "Red", "Bebidas", "250 ml unidad", 5.83, 7.00, 6.20);
        agregarProductoBase("P0136", "775100000136", "Coca Cola", "Coca", "Bebidas", "600 ml paquete x12", 32.00, 36.00, 33.70);
        agregarProductoBase("P0137", "775100000137", "Inca Kola", "Inca", "Bebidas", "600 ml paquete x12", 32.00, 36.00, 33.70);
        agregarProductoBase("P0138", "775100000138", "Coca Cola", "Coca", "Bebidas", "1.5 L paquete x6", 39.00, 43.00, 41.10);
        agregarProductoBase("P0139", "775100000139", "Inca Kola", "Inca", "Bebidas", "1.5 L paquete x6", 39.00, 43.00, 41.10);
        agregarProductoBase("P0140", "775100000140", "Guaranita", "Guaranita", "Bebidas", "paquete", 11.50, 14.00, 12.20);
        agregarProductoBase("P0141", "775100000141", "Pepsi Pirana", "Pepsi", "Bebidas", "paquete", 12.50, 15.00, 13.20);
        agregarProductoBase("P0142", "775100000142", "Agua San Carlos", "Agua", "Bebidas", "paquete", 11.00, 13.00, 11.60);
        agregarProductoBase("P0143", "775100000143", "Agua Cielo", "Agua", "Bebidas", "paquete", 12.00, 14.00, 12.70);
        agregarProductoBase("P0144", "775100000144", "Sporade", "Sporade", "Bebidas", "paquete", 21.50, 25.00, 22.70);
        agregarProductoBase("P0145", "775100000145", "Ginger Ale", "Ginger", "Bebidas", "paquete", 27.00, 30.00, 28.50);
        agregarProductoBase("P0146", "775100000146", "Tampico grande", "Tampico", "Bebidas", "paquete", 11.80, 13.00, 12.50);
        agregarProductoBase("P0147", "775100000147", "Hielo", "Hielo", "Bebidas", "bolsa 2 kg", 2.50, 3.00, 2.70);
        agregarProductoBase("P0148", "775100000148", "Jarabe de goma Wislan", "Jarabe", "Jarabes", "unidad", 6.67, 10.00, 7.10);
        agregarProductoBase("P0149", "775100000149", "Jarabe de granadina Wislan", "Jarabe", "Jarabes", "unidad", 6.67, 10.00, 7.10);
        agregarProductoBase("P0150", "775100000150", "Jarabe de goma Wislan", "Jarabe", "Jarabes", "caja x12", 80.00, 100.00, 84.30);
        agregarProductoBase("P0151", "775100000151", "Jarabe de granadina Wislan", "Jarabe", "Jarabes", "caja x12", 80.00, 100.00, 84.30);
        agregarProductoBase("P0152", "775100000152", "Golden rojo", "Golden", "Cigarros", "paquete x10 cajetillas", 19.80, 23.00, 20.90);
        agregarProductoBase("P0153", "775100000153", "Golden verde", "Golden", "Cigarros", "paquete x10 cajetillas", 19.80, 23.00, 20.90);
        agregarProductoBase("P0154", "775100000154", "Golden plomo", "Golden", "Cigarros", "paquete x10 cajetillas", 19.80, 23.00, 20.90);
        agregarProductoBase("P0155", "775100000155", "Carlyle rojo", "Carlyle", "Cigarros", "paquete x10 cajetillas", 20.50, 24.00, 21.60);
        agregarProductoBase("P0156", "775100000156", "Carlyle verde", "Carlyle", "Cigarros", "paquete x10 cajetillas", 20.50, 24.00, 21.60);
        agregarProductoBase("P0157", "775100000157", "Carnival Mora", "Carnival", "Cigarros", "paquete", 45.00, 52.00, 47.40);
        agregarProductoBase("P0158", "775100000158", "Carnival Sandia", "Carnival", "Cigarros", "paquete", 45.00, 52.00, 47.40);
        agregarProductoBase("P0159", "775100000159", "Carnival Mango", "Carnival", "Cigarros", "paquete", 58.00, 67.00, 61.10);
        agregarProductoBase("P0160", "775100000160", "Lucky Mora", "Lucky", "Cigarros", "paquete", 55.00, 64.00, 57.90);
        agregarProductoBase("P0161", "775100000161", "Pine / Pint", "Pine", "Cigarros", "paquete", 55.00, 64.00, 57.90);
        agregarProductoBase("P0162", "775100000999", "Hielo en bolsa", "Hielo", "Complemento", "Bolsa", 3.50, 5.00, 4.00);
    }

    void cargarCombosBase() {
        // Los combos se generan como ejemplo dentro del menu de combos.
    }

    void imprimirProductoCabecera() {
        cout << left << setw(5) << "ID" << setw(8) << "Codigo" << setw(15) << "Barra" << setw(36) << "Producto" << setw(16) << "Marca" << setw(18) << "Categoria" << setw(12) << "Precio" << "StockG" << "\n";
        cout << string(120, '-') << "\n";
    }

    int stockGeneral(const Producto &p) const {
        int s = 0;
        for (int v : p.stock) s += v;
        return s;
    }

    vector<int> buscarProductosIndices(const string &q) {
        vector<int> res;
        string t = lowerText(trim(q));
        if (t.empty()) return res;
        int idq = -1;
        parseInt(t, idq);
        for (int i = 0; i < (int)productos.size(); ++i) {
            const Producto &p = productos[i];
            string base = lowerText(p.codigo + " " + p.codigoBarras + " " + p.nombre + " " + p.marca + " " + p.categoria + " " + p.presentacion);
            if ((idq > 0 && p.id == idq) || base.find(t) != string::npos) res.push_back(i);
        }
        return res;
    }

    void mostrarProductoLinea(const Producto &p) {
        cout << left << setw(5) << p.id << setw(8) << p.codigo << setw(15) << p.codigoBarras.substr(0,14) << setw(36) << p.nombre.substr(0,35) << setw(16) << p.marca.substr(0,15) << setw(18) << p.categoria.substr(0,17) << setw(12) << money(p.precioMenor) << stockGeneral(p) << "\n";
    }

    void listarProductos(bool pausarPaginas = true) {
        imprimirProductoCabecera();
        int count = 0;
        for (const auto &p : productos) {
            if (!p.activo) continue;
            mostrarProductoLinea(p);
            count++;
            if (pausarPaginas && count % 25 == 0) {
                cout << "-- Mostrando " << count << " productos. ENTER continua, 0 vuelve: ";
                string s; getline(cin, s);
                if (isExitText(s)) return;
            }
        }
        cout << "Total productos activos: " << count << "\n";
    }

    int escogerProductoPorBusqueda() {
        while (true) {
            string q = pedirTexto("Buscar producto por ID, codigo, barra, nombre o marca (0 para volver): ", false);
            if (isExitText(q) || q.empty()) return -1;
            auto res = buscarProductosIndices(q);
            if (res.empty()) { cout << "No se encontraron productos.\n"; continue; }
            imprimirProductoCabecera();
            int limite = min((int)res.size(), 20);
            for (int i = 0; i < limite; ++i) mostrarProductoLinea(productos[res[i]]);
            if ((int)res.size() > limite) cout << "Hay mas coincidencias. Escriba una busqueda mas precisa.\n";
            int id = pedirEntero("Ingrese ID exacto del producto (0 para buscar de nuevo): ", 0, (int)productos.size());
            if (id == 0) continue;
            for (int idx : res) if (productos[idx].id == id) return idx;
            cout << "El ID elegido no esta en la lista mostrada.\n";
        }
    }

    void menuProductos() {
        while (true) {
            cout << "\n--- PRODUCTOS ---\n";
            cout << "1. Listar productos\n";
            cout << "2. Buscar producto\n";
            cout << "3. Registrar producto nuevo\n";
            cout << "4. Modificar precio\n";
            cout << "5. Ajustar stock\n";
            cout << "0. Volver\n";
            int op = pedirOpcion(0, 5);
            if (op == 0) return;
            if (op == 1) { listarProductos(); pausar(); }
            else if (op == 2) { int idx = escogerProductoPorBusqueda(); if (idx >= 0) { mostrarDetalleProducto(productos[idx]); pausar(); } }
            else if (op == 3) registrarProductoNuevo();
            else if (op == 4) modificarPrecioProducto();
            else if (op == 5) ajustarStockProducto();
        }
    }

    void mostrarDetalleProducto(const Producto &p) {
        cout << "\nProducto seleccionado\n";
        cout << "ID: " << p.id << "\n";
        cout << "Codigo: " << p.codigo << "\n";
        cout << "Codigo de barras: " << p.codigoBarras << "\n";
        cout << "Nombre: " << p.nombre << "\n";
        cout << "Marca: " << p.marca << "\n";
        cout << "Categoria: " << p.categoria << "\n";
        cout << "Presentacion: " << p.presentacion << "\n";
        cout << "Costo: " << money(p.costo) << "\n";
        cout << "Precio menor: " << money(p.precioMenor) << "\n";
        cout << "Precio mayor: " << money(p.precioMayor) << "\n";
        cout << "Vencimiento: " << p.fechaVencimiento << "\n";
        cout << "Stock por local:\n";
        for (int i = 0; i < (int)locales.size(); ++i) cout << "  " << locales[i].nombre << ": " << p.stock[i] << "\n";
        cout << "Stock general: " << stockGeneral(p) << "\n";
    }

    void registrarProductoNuevo() {
        string comentario;
        if (!pedirComentarioCritico("Registrar producto nuevo", comentario)) return;
        string nombre = pedirTexto("Nombre del producto (0 cancela): ", true);
        if (isExitText(nombre)) return;
        string marca = pedirTexto("Marca: ", false);
        string categoria = pedirTexto("Categoria: ", false);
        string presentacion = pedirTexto("Presentacion: ", false);
        double costo = pedirDouble("Costo: ", 0.01); if (costo < 0) return;
        double menor = pedirDouble("Precio venta menor: ", 0.01); if (menor < 0) return;
        double mayor = pedirDouble("Precio venta mayor: ", 0.01); if (mayor < 0) return;
        string code = "P" + string(4 - to_string((int)productos.size()+1).length(), '0') + to_string((int)productos.size()+1);
        string bar = "775" + to_string(200000000 + (int)productos.size() + 1);
        agregarProductoBase(code, bar, nombre, marca.empty()?"General":marca, categoria.empty()?"General":categoria, presentacion.empty()?"Unidad":presentacion, costo, menor, mayor);
        registrarAuditoria("Registro de producto", nombre, "No existia", code + " / " + bar, comentario, "Ejecutado y pendiente de revision");
        cout << "Producto registrado con codigo " << code << " y barra " << bar << ".\n";
        pausar();
    }

    void modificarPrecioProducto() {
        int idx = escogerProductoPorBusqueda(); if (idx < 0) return;
        mostrarDetalleProducto(productos[idx]);
        string comentario;
        if (!pedirComentarioCritico("Modificar precio", comentario)) return;
        double nuevo = pedirDouble("Nuevo precio por menor (0 cancela): ", 0.01); if (nuevo < 0) return;
        string ant = money(productos[idx].precioMenor);
        productos[idx].precioMenor = nuevo;
        registrarAuditoria("Modificacion de precio", productos[idx].nombre, ant, money(nuevo), comentario, "Ejecutado y pendiente de revision");
        cout << "Precio actualizado. La accion fue enviada a gerente general y supervisor zonal.\n";
        pausar();
    }

    void ajustarStockProducto() {
        int idx = escogerProductoPorBusqueda(); if (idx < 0) return;
        int loc = seleccionarLocalAccesible(); if (loc < 0) return;
        string comentario;
        if (!pedirComentarioCritico("Ajuste de stock", comentario)) return;
        cout << "Stock actual en " << locales[loc].nombre << ": " << productos[idx].stock[loc] << "\n";
        int nuevo = pedirEntero("Nuevo stock (0 permitido, - no permitido): ", 0, 100000, false);
        string ant = to_string(productos[idx].stock[loc]);
        productos[idx].stock[loc] = nuevo;
        registrarAuditoria("Ajuste de stock", productos[idx].nombre + " | " + locales[loc].nombre, ant, to_string(nuevo), comentario, "Ejecutado y pendiente de revision");
        cout << "Stock actualizado. La accion fue enviada a gerente general y supervisor zonal.\n";
        pausar();
    }

    Cliente crearClienteRapido() {
        Cliente c;
        c.id = nextCliente++;
        c.nombre = pedirTexto("Cliente o razon social (ENTER = Cliente no registrado): ", false);
        if (c.nombre.empty()) c.nombre = "Cliente no registrado";
        cout << "Tipo de documento:\n";
        cout << "1. DNI\n2. RUC\n3. Carnet de extranjeria\n4. Pasaporte\n5. Sin documento\n";
        int op = pedirOpcion(1, 5);
        vector<string> docs = {"DNI", "RUC", "Carnet de extranjeria", "Pasaporte", "Sin documento"};
        c.tipoDoc = docs[op-1];
        if (op == 5) c.numeroDoc = "-";
        else c.numeroDoc = pedirTexto("Numero de documento: ", true);
        c.telefono = pedirTexto("Telefono (opcional): ", false);
        clientes.push_back(c);
        return c;
    }

    void menuClientes() {
        while (true) {
            cout << "\n--- CLIENTES ---\n";
            cout << "1. Registrar cliente\n";
            cout << "2. Listar clientes\n";
            cout << "0. Volver\n";
            int op = pedirOpcion(0, 2);
            if (op == 0) return;
            if (op == 1) { Cliente c = crearClienteRapido(); cout << "Cliente registrado ID " << c.id << ".\n"; pausar(); }
            if (op == 2) {
                cout << left << setw(5) << "ID" << setw(30) << "Cliente" << setw(20) << "Documento" << "Telefono\n";
                for (const auto &c : clientes) cout << setw(5) << c.id << setw(30) << c.nombre.substr(0,29) << setw(20) << (c.tipoDoc + ":" + c.numeroDoc).substr(0,19) << c.telefono << "\n";
                pausar();
            }
        }
    }

    void menuVentas() {
        while (true) {
            cout << "\n--- VENTAS ---\n";
            cout << "1. Registrar venta\n";
            cout << "2. Ver ventas registradas\n";
            cout << "0. Volver\n";
            int op = pedirOpcion(0, 2);
            if (op == 0) return;
            if (op == 1) registrarVenta();
            if (op == 2) { listarVentasDisponibles(); pausar(); }
        }
    }

    void registrarVenta() {
        int loc = (sesion.rol == "Cajero" || sesion.rol == "Cajero auxiliar" || sesion.rol == "Almacenero" || sesion.rol == "Administrador") ? localIndexPorNombre(sesion.local) : seleccionarLocalAccesible();
        if (loc < 0) return;
        Cliente cliente = crearClienteRapido();
        vector<DetalleVenta> detalles;
        double subtotal = 0.0;
        while (true) {
            cout << "\nAgregar producto a venta. Escriba 0 para terminar.\n";
            int idx = escogerProductoPorBusqueda();
            if (idx < 0) break;
            Producto &p = productos[idx];
            cout << "Stock en " << locales[loc].nombre << ": " << p.stock[loc] << "\n";
            if (p.stock[loc] <= 0) { cout << "No hay stock en este local.\n"; continue; }
            int cant = pedirEntero("Cantidad: ", 1, p.stock[loc]);
            if (cant == 0) break;
            double precio = p.precioMenor;
            string usarMayor = lowerText(pedirTexto("Usar precio mayor? (s/n): ", false));
            if (usarMayor == "s" || usarMayor == "si") precio = p.precioMayor;
            DetalleVenta d{p.id, p.codigo, p.nombre, cant, precio, precio * cant};
            detalles.push_back(d);
            subtotal += d.subtotal;
            cout << "Agregado: " << p.nombre << " x" << cant << " = " << money(d.subtotal) << "\n";
        }
        if (detalles.empty()) { cout << "Venta cancelada: no hay productos.\n"; pausar(); return; }
        cout << "Subtotal: " << money(subtotal) << "\n";
        double descuento = 0.0;
        cout << "Descuento: 1 porcentaje, 2 monto fijo, 0 sin descuento\n";
        int od = pedirOpcion(0, 2);
        if (od == 1) {
            double pct = pedirDouble("Porcentaje de descuento: ", 0.0, false);
            descuento = subtotal * (pct / 100.0);
        } else if (od == 2) {
            descuento = pedirDouble("Monto de descuento: ", 0.0, false);
            if (descuento > subtotal) descuento = subtotal;
        }
        string sigv = lowerText(pedirTexto("Aplicar IGV 18%? (s/n): ", false));
        bool conIGV = (sigv == "s" || sigv == "si");
        double base = subtotal - descuento;
        double igv = conIGV ? base * 0.18 : 0.0;
        double total = base + igv;
        vector<Pago> pagos = registrarPagos(total);
        if (pagos.empty()) { cout << "Venta cancelada por pago incompleto.\n"; pausar(); return; }
        for (const auto &d : detalles) {
            int pi = d.productoId - 1;
            if (pi >= 0 && pi < (int)productos.size()) productos[pi].stock[loc] -= d.cantidad;
        }
        Venta v;
        v.id = nextVenta++;
        v.fecha = nowText();
        v.localIndex = loc;
        v.local = locales[loc].nombre;
        v.cajero = sesion.usuario;
        v.cliente = cliente;
        v.detalles = detalles;
        v.pagos = pagos;
        v.subtotal = subtotal;
        v.descuento = descuento;
        v.conIGV = conIGV;
        v.igv = igv;
        v.total = total;
        ventas.push_back(v);
        cout << "\nVenta registrada correctamente. ID venta: " << v.id << "\n";
        mostrarTicketPantalla(v);
        string g = lowerText(pedirTexto("Guardar ticket en archivo TXT? (s/n): ", false));
        if (g == "s" || g == "si") guardarTicketTXT(v, false);
        pausar();
    }

    vector<Pago> registrarPagos(double total) {
        vector<Pago> pagos;
        double acumulado = 0.0;
        while (acumulado + 0.001 < total) {
            cout << "\nTotal: " << money(total) << " | Falta: " << money(total - acumulado) << "\n";
            cout << "Metodos: efectivo, yape, plin, tarjeta, transferencia, credito\n";
            string metodo = pedirTexto("Metodo de pago (0 cancela): ", true);
            if (isExitText(metodo)) return {};
            double monto = pedirDouble("Monto: ", 0.01); if (monto < 0) return {};
            if (acumulado + monto > total + 0.01) {
                cout << "El monto supera el total. Ingrese un monto menor.\n";
                continue;
            }
            pagos.push_back({metodo, monto});
            acumulado += monto;
        }
        return pagos;
    }

    void listarVentasDisponibles() {
        if (ventas.empty()) { cout << "No hay ventas registradas. Primero registre una venta.\n"; return; }
        cout << left << setw(6) << "ID" << setw(22) << "Fecha" << setw(18) << "Local" << setw(28) << "Cliente" << setw(12) << "Total" << "Cajero\n";
        cout << string(100, '-') << "\n";
        for (const auto &v : ventas) {
            if (!puedeVerLocal(v.localIndex)) continue;
            cout << setw(6) << v.id << setw(22) << v.fecha << setw(18) << v.local.substr(0,17) << setw(28) << v.cliente.nombre.substr(0,27) << setw(12) << money(v.total) << v.cajero << "\n";
        }
    }

    Venta* buscarVentaPorId(int id) {
        for (auto &v : ventas) if (v.id == id && puedeVerLocal(v.localIndex)) return &v;
        return nullptr;
    }

    void menuComprobantes() {
        while (true) {
            cout << "\n--- COMPROBANTES ---\n";
            cout << "1. Ver ticket en pantalla y guardar TXT 80mm\n";
            cout << "2. Ver comprobante PDF simulado en pantalla y guardar TXT\n";
            cout << "3. Ver ventas disponibles\n";
            cout << "0. Volver\n";
            int op = pedirOpcion(0, 3);
            if (op == 0) return;
            if (op == 3) { listarVentasDisponibles(); pausar(); continue; }
            if (ventas.empty()) { cout << "No hay ventas registradas.\n"; pausar(); continue; }
            listarVentasDisponibles();
            int id = pedirEntero("ID venta (0 vuelve): ", 0, 1000000);
            if (id == 0) continue;
            Venta *v = buscarVentaPorId(id);
            if (!v) { cout << "ID no encontrado o no tiene permiso para verlo.\n"; pausar(); continue; }
            if (op == 1) { mostrarTicketPantalla(*v); guardarTicketTXT(*v, true); }
            if (op == 2) { mostrarPDFSimuladoPantalla(*v); guardarPDFSimulado(*v, true); }
            pausar();
        }
    }

    string ticketComoTexto(const Venta &v, bool ancho80) {
        stringstream ss;
        int w = ancho80 ? 42 : 80;
        ss << string(w, '=') << "\n";
        ss << "LICORERIA LEON\n";
        ss << "RUC: 00000000000\n";
        ss << "Local: " << v.local << "\n";
        ss << "Fecha: " << v.fecha << "\n";
        ss << "Cajero: " << v.cajero << "\n";
        ss << string(w, '-') << "\n";
        ss << "Cliente: " << v.cliente.nombre << "\n";
        ss << "Documento: " << v.cliente.tipoDoc << " " << v.cliente.numeroDoc << "\n";
        ss << string(w, '-') << "\n";
        for (const auto &d : v.detalles) {
            ss << d.nombre << "\n";
            ss << "  " << d.cantidad << " x " << money(d.precioUnitario) << " = " << money(d.subtotal) << "\n";
        }
        ss << string(w, '-') << "\n";
        ss << "Subtotal:  " << money(v.subtotal) << "\n";
        ss << "Descuento: " << money(v.descuento) << "\n";
        ss << "IGV:       " << money(v.igv) << (v.conIGV ? " (18%)" : " (No aplicado)") << "\n";
        ss << "TOTAL:     " << money(v.total) << "\n";
        ss << string(w, '-') << "\n";
        ss << "Pagos:\n";
        for (const auto &p : v.pagos) ss << "  " << p.metodo << ": " << money(p.monto) << "\n";
        ss << string(w, '-') << "\n";
        ss << "ID venta: " << v.id << "\n";
        ss << "Gracias por su compra.\n";
        ss << string(w, '=') << "\n";
        return ss.str();
    }

    void mostrarTicketPantalla(const Venta &v) {
        cout << "\n--- TICKET 80MM SIMULADO EN PANTALLA ---\n";
        cout << ticketComoTexto(v, true);
        cout << "--- Opcion imprimir: use el archivo TXT generado o configure impresora termica. ---\n";
    }

    void mostrarPDFSimuladoPantalla(const Venta &v) {
        cout << "\n--- COMPROBANTE PDF SIMULADO EN PANTALLA ---\n";
        cout << ticketComoTexto(v, false);
        cout << "--- Este formato se guarda como TXT simulando PDF para el trabajo academico. ---\n";
    }

    void guardarTicketTXT(const Venta &v, bool avisar) {
        string file = "ticket_venta_" + to_string(v.id) + ".txt";
        ofstream out(file);
        out << ticketComoTexto(v, true);
        out.close();
        if (avisar) cout << "Ticket guardado: " << file << "\n";
    }

    void guardarPDFSimulado(const Venta &v, bool avisar) {
        string file = "comprobante_pdf_simulado_venta_" + to_string(v.id) + ".txt";
        ofstream out(file);
        out << ticketComoTexto(v, false);
        out.close();
        if (avisar) cout << "PDF simulado guardado: " << file << "\n";
    }

    void reporteStockPorLocal() {
        int loc = seleccionarLocalAccesible(); if (loc < 0) return;
        cout << "\n--- STOCK POR LOCAL: " << locales[loc].nombre << " ---\n";
        cout << left << setw(5) << "ID" << setw(35) << "Producto" << setw(15) << "Marca" << setw(12) << "Stock" << "Precio\n";
        for (const auto &p : productos) cout << setw(5) << p.id << setw(35) << p.nombre.substr(0,34) << setw(15) << p.marca.substr(0,14) << setw(12) << p.stock[loc] << money(p.precioMenor) << "\n";
        pausar();
    }

    void reporteStockGeneral() {
        cout << "\n--- STOCK GENERAL ENTRE TODAS LAS TIENDAS ---\n";
        cout << left << setw(5) << "ID" << setw(38) << "Producto" << setw(16) << "Marca" << setw(12) << "StockG" << "Detalle\n";
        for (const auto &p : productos) {
            cout << setw(5) << p.id << setw(38) << p.nombre.substr(0,37) << setw(16) << p.marca.substr(0,15) << setw(12) << stockGeneral(p);
            for (int i = 0; i < (int)locales.size(); ++i) cout << locales[i].nombre << ":" << p.stock[i] << " ";
            cout << "\n";
        }
        pausar();
    }

    void menuTransferencias() {
        cout << "\n--- TRANSFERENCIAS ENTRE LOCALES ---\n";
        int idx = escogerProductoPorBusqueda(); if (idx < 0) return;
        cout << "Local origen:\n"; int origen = seleccionarLocalAccesible(); if (origen < 0) return;
        cout << "Local destino:\n"; int destino = seleccionarLocalAccesible(); if (destino < 0) return;
        if (origen == destino) { cout << "Origen y destino no pueden ser iguales.\n"; pausar(); return; }
        cout << "Stock disponible en origen: " << productos[idx].stock[origen] << "\n";
        int cant = pedirEntero("Cantidad a transferir: ", 1, productos[idx].stock[origen]); if (cant == 0) return;
        string comentario; if (!pedirComentarioCritico("Transferencia de mercaderia", comentario)) return;
        productos[idx].stock[origen] -= cant;
        productos[idx].stock[destino] += cant;
        registrarAuditoria("Transferencia de stock", productos[idx].nombre, locales[origen].nombre + " -" + to_string(cant), locales[destino].nombre + " +" + to_string(cant), comentario, "Ejecutado y pendiente de revision");
        cout << "Transferencia realizada y notificada.\n";
        pausar();
    }

    void menuCombos() {
        while (true) {
            cout << "\n--- GESTION DE COMBOS ---\n";
            cout << "1. Crear nuevo combo\n";
            cout << "2. Listar combos\n";
            cout << "3. Ver detalle de un combo\n";
            cout << "0. Volver\n";

            int op = pedirOpcion(0, 3);
            if (op == 0) return;

            if (op == 1) {
                string nombre = pedirTexto("Nombre del combo (0 para cancelar): ", true);
                if (isExitText(nombre)) continue;

                double precio = pedirDouble("Precio de venta del combo: ", 0.01);
                if (precio < 0) continue;

                int cantidadProductos = pedirEntero(
                    "Cuantos productos formaran el combo? (minimo 2): ",
                    2, 20, false
                );

                Combo nuevo;
                nuevo.id = nextCombo++;
                nuevo.nombre = nombre;
                nuevo.precio = precio;
                nuevo.activo = true;

                cout << "\nSeleccione los " << cantidadProductos
                     << " productos que formaran el combo.\n";

                bool cancelado = false;

                for (int i = 1; i <= cantidadProductos; ++i) {
                    while (true) {
                        cout << "\n--- ELEGIR PRODUCTO " << i
                             << " DE " << cantidadProductos << " ---\n";

                        int idx = escogerProductoPorBusqueda();

                        if (idx < 0) {
                            cout << "Creacion del combo cancelada.\n";
                            cancelado = true;
                            break;
                        }

                        int productoId = productos[idx].id;

                        if (find(nuevo.productoIds.begin(),
                                 nuevo.productoIds.end(),
                                 productoId) != nuevo.productoIds.end()) {
                            cout << "Ese producto ya fue agregado al combo. Elija otro producto.\n";
                            continue;
                        }

                        nuevo.productoIds.push_back(productoId);

                        cout << "Producto " << i << " agregado: "
                             << productos[idx].nombre
                             << " | Precio individual: "
                             << money(productos[idx].precioMenor) << "\n";
                        break;
                    }

                    if (cancelado) break;
                }

                if (cancelado) continue;

                combos.push_back(nuevo);

                cout << "\nCombo creado correctamente.\n";
                cout << "ID del combo: " << nuevo.id << "\n";
                cout << "Nombre: " << nuevo.nombre << "\n";
                cout << "Precio del combo: " << money(nuevo.precio) << "\n";
                cout << "Productos incluidos:\n";

                double sumaIndividual = 0.0;

                for (size_t i = 0; i < nuevo.productoIds.size(); ++i) {
                    for (const auto &p : productos) {
                        if (p.id == nuevo.productoIds[i]) {
                            cout << "  " << (i + 1) << ". "
                                 << p.nombre << " | "
                                 << money(p.precioMenor) << "\n";
                            sumaIndividual += p.precioMenor;
                            break;
                        }
                    }
                }

                cout << "Precio total por separado: " << money(sumaIndividual) << "\n";
                cout << "Precio promocional del combo: " << money(nuevo.precio) << "\n";

                if (sumaIndividual > nuevo.precio) {
                    cout << "Ahorro para el cliente: "
                         << money(sumaIndividual - nuevo.precio) << "\n";
                }

                registrarAuditoria(
                    "Creacion de combo",
                    nuevo.nombre,
                    "No existia",
                    "Combo ID " + to_string(nuevo.id) + " | " + money(nuevo.precio),
                    "Combo creado con " + to_string(cantidadProductos) + " productos",
                    "Ejecutado"
                );

                pausar();
            }

            else if (op == 2) {
                cout << "\n--- COMBOS REGISTRADOS ---\n";

                if (combos.empty()) {
                    cout << "No hay combos registrados.\n";
                    pausar();
                    continue;
                }

                cout << left
                     << setw(6) << "ID"
                     << setw(35) << "Nombre"
                     << setw(15) << "Precio"
                     << "Productos\n";
                cout << string(75, '-') << "\n";

                for (const auto &c : combos) {
                    if (!c.activo) continue;

                    cout << left
                         << setw(6) << c.id
                         << setw(35) << c.nombre.substr(0, 34)
                         << setw(15) << money(c.precio)
                         << c.productoIds.size() << "\n";
                }

                pausar();
            }

            else if (op == 3) {
                if (combos.empty()) {
                    cout << "No hay combos registrados.\n";
                    pausar();
                    continue;
                }

                cout << "\nCombos disponibles:\n";
                for (const auto &c : combos) {
                    if (c.activo) {
                        cout << c.id << ". " << c.nombre
                             << " | " << money(c.precio)
                             << " | " << c.productoIds.size()
                             << " productos\n";
                    }
                }

                int idCombo = pedirEntero(
                    "Ingrese el ID del combo (0 para volver): ",
                    0, 100000
                );

                if (idCombo == 0) continue;

                bool encontrado = false;

                for (const auto &c : combos) {
                    if (c.id != idCombo || !c.activo) continue;

                    encontrado = true;

                    cout << "\n--- DETALLE DEL COMBO ---\n";
                    cout << "ID: " << c.id << "\n";
                    cout << "Nombre: " << c.nombre << "\n";
                    cout << "Precio del combo: " << money(c.precio) << "\n";
                    cout << "Cantidad de productos: "
                         << c.productoIds.size() << "\n";
                    cout << "Productos incluidos:\n";

                    double sumaIndividual = 0.0;

                    for (size_t i = 0; i < c.productoIds.size(); ++i) {
                        bool productoEncontrado = false;

                        for (const auto &p : productos) {
                            if (p.id == c.productoIds[i]) {
                                cout << "  " << (i + 1) << ". "
                                     << p.nombre
                                     << " | Codigo: " << p.codigo
                                     << " | Precio: "
                                     << money(p.precioMenor) << "\n";

                                sumaIndividual += p.precioMenor;
                                productoEncontrado = true;
                                break;
                            }
                        }

                        if (!productoEncontrado) {
                            cout << "  " << (i + 1)
                                 << ". Producto no disponible en el catalogo actual.\n";
                        }
                    }

                    cout << "Total comprando por separado: "
                         << money(sumaIndividual) << "\n";
                    cout << "Precio del combo: "
                         << money(c.precio) << "\n";

                    if (sumaIndividual > c.precio) {
                        cout << "Ahorro: "
                             << money(sumaIndividual - c.precio) << "\n";
                    }

                    break;
                }

                if (!encontrado) {
                    cout << "No se encontro un combo activo con ese ID.\n";
                }

                pausar();
            }
        }
    }

    void menuSolicitudes() {
        while (true) {
            cout << "\n--- PRODUCTOS SOLICITADOS POR CLIENTES ---\n";
            cout << "1. Registrar solicitud\n2. Ver solicitudes\n0. Volver\n";
            int op = pedirOpcion(0, 2); if (op == 0) return;
            if (op == 1) {
                SolicitudProducto s;
                s.id = nextSolicitud++; s.fecha = nowText();
                int loc = seleccionarLocalAccesible(); if (loc < 0) continue;
                s.local = locales[loc].nombre;
                s.cliente = pedirTexto("Cliente: ", false);
                s.producto = pedirTexto("Producto solicitado: ", true);
                s.cantidad = pedirTexto("Cantidad solicitada: ", false);
                s.observacion = pedirTexto("Observacion: ", false);
                solicitudes.push_back(s);
                cout << "Solicitud registrada.\n";
            } else {
                for (const auto &s : solicitudes) cout << s.id << " | " << s.fecha << " | " << s.local << " | " << s.producto << " | " << s.cantidad << " | " << s.cliente << "\n";
            }
            pausar();
        }
    }

    void menuComprasProveedores() {
        cout << "\n--- COMPRAS Y PROVEEDORES ---\n";
        cout << "Modulo de compra unificada.\n";
        cout << "El gerente puede usar stock general, rotacion y productos solicitados para comprar por volumen.\n";
        cout << "Proveedores ejemplo: Cartavio, Coca-Cola, Backus, Distribuidora Lima, KR.\n";
        pausar();
    }

    void menuFacturasIA() {
        cout << "\n--- FACTURAS DE COMPRA CON IA SIMULADA ---\n";
        string archivo = pedirTexto("Nombre/imagen de factura simulada (0 vuelve): ", false);
        if (isExitText(archivo) || archivo.empty()) return;
        cout << "IA simulada leyendo factura: " << archivo << "\n";
        cout << "Resultado sugerido: proveedor detectado, productos, cantidades, descuentos e IGV.\n";
        int idx = escogerProductoPorBusqueda(); if (idx < 0) return;
        int loc = seleccionarLocalAccesible(); if (loc < 0) return;
        int cant = pedirEntero("Cantidad detectada para cargar al stock: ", 1, 100000); if (cant == 0) return;
        string comentario; if (!pedirComentarioCritico("Carga de stock desde factura IA", comentario)) return;
        productos[idx].stock[loc] += cant;
        registrarAuditoria("Carga de stock por factura IA", productos[idx].nombre, "Factura imagen: " + archivo, "+" + to_string(cant) + " en " + locales[loc].nombre, comentario, "Ejecutado y pendiente de revision");
        cout << "Stock actualizado previa confirmacion del usuario.\n";
        pausar();
    }

    void menuCodigos() {
        while (true) {
            cout << "\n--- CODIGOS DE BARRAS ---\n";
            cout << "1. Ver codigo de barras de producto\n";
            cout << "2. Generar nuevo codigo interno\n";
            cout << "3. Imprimir etiqueta simulada\n";
            cout << "0. Volver\n";
            int op = pedirOpcion(0, 3); if (op == 0) return;
            int idx = escogerProductoPorBusqueda(); if (idx < 0) continue;
            if (op == 1) { cout << "Codigo de barras: " << productos[idx].codigoBarras << "\n"; }
            if (op == 2) {
                string comentario; if (!pedirComentarioCritico("Generar codigo de barras interno", comentario)) continue;
                string ant = productos[idx].codigoBarras;
                productos[idx].codigoBarras = "775" + to_string(300000000 + productos[idx].id);
                registrarAuditoria("Generacion de codigo de barras", productos[idx].nombre, ant, productos[idx].codigoBarras, comentario, "Ejecutado y pendiente de revision");
                cout << "Nuevo codigo: " << productos[idx].codigoBarras << "\n";
            }
            if (op == 3) {
                cout << "\nETIQUETA SIMULADA\n";
                cout << productos[idx].nombre << "\n";
                cout << "Precio: " << money(productos[idx].precioMenor) << "\n";
                cout << "Barra: ||| " << productos[idx].codigoBarras << " |||\n";
                cout << "Puede imprimirse en 1, 2 o 3 columnas segun impresora.\n";
            }
            pausar();
        }
    }

    void menuReportesCSV() {
        while (true) {
            cout << "\n--- REPORTES Y EXCEL CSV ---\n";
            cout << "1. Exportar stock general CSV\n";
            cout << "2. Exportar ventas CSV\n";
            cout << "3. Exportar auditoria CSV\n";
            cout << "0. Volver\n";
            int op = pedirOpcion(0, 3); if (op == 0) return;
            if (op == 1) exportarStockCSV();
            if (op == 2) exportarVentasCSV();
            if (op == 3) exportarAuditoriaCSV();
            pausar();
        }
    }

    void exportarStockCSV() {
        ofstream out("reporte_stock_general.csv");
        out << "ID,Codigo,Barra,Producto,Marca,Categoria,Precio,StockGeneral";
        for (auto &l : locales) out << "," << l.nombre;
        out << "\n";
        for (auto &p : productos) {
            out << p.id << "," << p.codigo << "," << p.codigoBarras << ",\"" << p.nombre << "\",\"" << p.marca << "\",\"" << p.categoria << "\"," << p.precioMenor << "," << stockGeneral(p);
            for (int s : p.stock) out << "," << s;
            out << "\n";
        }
        cout << "Archivo generado: reporte_stock_general.csv\n";
    }

    void exportarVentasCSV() {
        ofstream out("reporte_ventas.csv");
        out << "ID,Fecha,Local,Cajero,Cliente,Documento,Subtotal,Descuento,IGV,Total\n";
        for (auto &v : ventas) out << v.id << "," << v.fecha << "," << v.local << "," << v.cajero << ",\"" << v.cliente.nombre << "\",\"" << v.cliente.tipoDoc << " " << v.cliente.numeroDoc << "\"," << v.subtotal << "," << v.descuento << "," << v.igv << "," << v.total << "\n";
        cout << "Archivo generado: reporte_ventas.csv\n";
    }

    void exportarAuditoriaCSV() {
        ofstream out("reporte_auditoria.csv");
        out << "ID,Fecha,Usuario,Rol,Local,Zona,Accion,Detalle,Anterior,Nuevo,Comentario,Estado\n";
        for (auto &a : auditorias) out << a.id << "," << a.fecha << "," << a.usuario << "," << a.rol << "," << a.local << "," << a.zona << ",\"" << a.accion << "\",\"" << a.detalle << "\",\"" << a.datoAnterior << "\",\"" << a.datoNuevo << "\",\"" << a.comentario << "\",\"" << a.estado << "\"\n";
        cout << "Archivo generado: reporte_auditoria.csv\n";
    }

    void menuVencimientos() {
        cout << "\n--- CONTROL DE VENCIMIENTOS FEFO ---\n";
        vector<Producto> copia = productos;
        sort(copia.begin(), copia.end(), [](const Producto &a, const Producto &b){ return a.fechaVencimiento < b.fechaVencimiento; });
        cout << left << setw(5) << "ID" << setw(35) << "Producto" << setw(15) << "Marca" << setw(15) << "Vence" << "StockG\n";
        for (int i = 0; i < min(30, (int)copia.size()); ++i) cout << setw(5) << copia[i].id << setw(35) << copia[i].nombre.substr(0,34) << setw(15) << copia[i].marca.substr(0,14) << setw(15) << copia[i].fechaVencimiento << stockGeneral(copia[i]) << "\n";
        cout << "FEFO: First Expired, First Out. Sale primero lo que vence mas pronto.\n";
        pausar();
    }

    void mostrarCuadroCredenciales() {
        cout << "\n--- CUADRO DE CREDENCIALES ---\n";
        cout << left << setw(4) << "#" << setw(28) << "Usuario" << setw(8) << "Clave" << setw(26) << "Persona" << setw(20) << "Rol" << setw(18) << "Local" << "Zona\n";
        cout << string(130, '-') << "\n";
        for (size_t i = 0; i < usuarios.size(); ++i) {
            const auto &u = usuarios[i];
            cout << left << setw(4) << (int)i + 1
                 << setw(28) << u.usuario.substr(0,27)
                 << setw(8) << u.clave
                 << setw(26) << u.nombrePersona.substr(0,25)
                 << setw(20) << u.rol.substr(0,19)
                 << setw(18) << u.local.substr(0,17)
                 << u.zona << "\n";
        }
    }

    bool esGerenteGeneral() const {
        return sesion.rol == "Gerente general";
    }

    bool claveValida(const string &clave) const {
        if (clave.size() != 3) return false;
        for (char c : clave) if (!isdigit((unsigned char)c)) return false;
        return true;
    }

    bool usuarioExiste(const string &usuario, int ignorar = -1) const {
        for (int i = 0; i < (int)usuarios.size(); ++i) {
            if (i == ignorar) continue;
            if (usuarios[i].usuario == usuario) return true;
        }
        return false;
    }

    bool claveExiste(const string &clave, int ignorar = -1) const {
        for (int i = 0; i < (int)usuarios.size(); ++i) {
            if (i == ignorar) continue;
            if (usuarios[i].clave == clave) return true;
        }
        return false;
    }

    string pedirUsuarioNuevo(int ignorar = -1) {
        while (true) {
            string u = lowerText(pedirTexto("Usuario (0 cancela): ", true));
            if (isExitText(u)) return "";
            if (u.size() < 4) { cout << "El usuario debe tener minimo 4 caracteres.\n"; continue; }
            bool ok = true;
            for (char c : u) {
                if (!isalnum((unsigned char)c) && c != '_') { ok = false; break; }
            }
            if (!ok) { cout << "Use solo letras, numeros o guion bajo.\n"; continue; }
            if (usuarioExiste(u, ignorar)) { cout << "Ese usuario ya existe. Ingrese otro.\n"; continue; }
            return u;
        }
    }

    string pedirClaveNueva(int ignorar = -1) {
        while (true) {
            string c = pedirTexto("Clave de 3 digitos (0 cancela): ", true);
            if (isExitText(c)) return "";
            if (!claveValida(c)) { cout << "La clave debe tener exactamente 3 digitos numericos.\n"; continue; }
            if (claveExiste(c, ignorar)) { cout << "Esa clave ya esta usada por otro usuario. Ingrese otra.\n"; continue; }
            return c;
        }
    }

    int seleccionarRolUsuario() {
        cout << "\nSeleccione rol:\n";
        cout << "1. Administrador\n";
        cout << "2. Cajero\n";
        cout << "3. Cajero auxiliar\n";
        cout << "4. Almacenero\n";
        cout << "5. Supervisor zonal\n";
        cout << "6. Gerente general\n";
        cout << "0. Cancelar\n";
        return pedirOpcion(0, 6);
    }

    string rolPorOpcion(int op) const {
        if (op == 1) return "Administrador";
        if (op == 2) return "Cajero";
        if (op == 3) return "Cajero auxiliar";
        if (op == 4) return "Almacenero";
        if (op == 5) return "Supervisor zonal";
        return "Gerente general";
    }

    int seleccionarLocalGlobal() {
        cout << "\nSeleccione local:\n";
        for (int i = 0; i < (int)locales.size(); ++i) cout << i + 1 << ". " << locales[i].nombre << " | " << locales[i].zona << "\n";
        cout << "0. Cancelar\n";
        int op = pedirOpcion(0, (int)locales.size());
        if (op == 0) return -1;
        return op - 1;
    }

    int seleccionarZonaSupervisor() {
        cout << "\nSeleccione zona para supervisor:\n";
        cout << "1. Zona Alto Huallaga\n";
        cout << "2. Zona Federico Basadre\n";
        cout << "0. Cancelar\n";
        int op = pedirOpcion(0, 2);
        return op;
    }

    bool completarRolLocalZona(int rolOp, string &rol, string &local, string &zona) {
        rol = rolPorOpcion(rolOp);
        if (rol == "Gerente general") {
            local = "TODOS";
            zona = "TODAS";
            return true;
        }
        if (rol == "Supervisor zonal") {
            int z = seleccionarZonaSupervisor();
            if (z == 0) return false;
            local = "ZONA";
            zona = (z == 1 ? "Zona Alto Huallaga" : "Zona Federico Basadre");
            return true;
        }
        int loc = seleccionarLocalGlobal();
        if (loc < 0) return false;
        local = locales[loc].nombre;
        zona = locales[loc].zona;
        return true;
    }

    void crearCredencialGerente() {
        if (!esGerenteGeneral()) { cout << "Solo el gerente general puede crear credenciales.\n"; pausar(); return; }
        cout << "\n--- CREAR CREDENCIAL ---\n";
        string usuario = pedirUsuarioNuevo(); if (usuario.empty()) return;
        string clave = pedirClaveNueva(); if (clave.empty()) return;
        string nombre = pedirTexto("Nombre de la persona designada: ", true); if (isExitText(nombre)) return;
        int rolOp = seleccionarRolUsuario(); if (rolOp == 0) return;
        string rol, local, zona;
        if (!completarRolLocalZona(rolOp, rol, local, zona)) return;
        string comentario;
        if (!pedirComentarioCritico("crear credencial de usuario", comentario)) return;
        addUser(usuario, clave, rol, local, zona, nombre);
        registrarAuditoria("Crear credencial", "Usuario: " + usuario + " | Persona: " + nombre, "No existia", "Creado", comentario, "Registrado por gerente");
        cout << "Credencial creada correctamente.\n";
        pausar();
    }

    void editarCredencialGerente() {
        if (!esGerenteGeneral()) { cout << "Solo el gerente general puede editar credenciales.\n"; pausar(); return; }
        mostrarCuadroCredenciales();
        int id = pedirEntero("Numero de credencial a editar (0 cancela): ", 0, (int)usuarios.size());
        if (id == 0) return;
        int idx = id - 1;
        Usuario anterior = usuarios[idx];
        while (true) {
            cout << "\n--- EDITAR CREDENCIAL: " << usuarios[idx].usuario << " ---\n";
            cout << "1. Cambiar usuario\n";
            cout << "2. Cambiar clave\n";
            cout << "3. Cambiar nombre de persona\n";
            cout << "4. Cambiar rol/local/zona\n";
            cout << "5. Guardar y volver\n";
            cout << "0. Cancelar cambios\n";
            int op = pedirOpcion(0, 5);
            if (op == 0) { usuarios[idx] = anterior; cout << "Cambios cancelados.\n"; pausar(); return; }
            if (op == 5) break;
            if (op == 1) { string u = pedirUsuarioNuevo(idx); if (!u.empty()) usuarios[idx].usuario = u; }
            if (op == 2) { string c = pedirClaveNueva(idx); if (!c.empty()) usuarios[idx].clave = c; }
            if (op == 3) { string n = pedirTexto("Nuevo nombre de persona: ", true); if (!isExitText(n)) usuarios[idx].nombrePersona = n; }
            if (op == 4) {
                int rolOp = seleccionarRolUsuario();
                if (rolOp != 0) {
                    string r,l,z;
                    if (completarRolLocalZona(rolOp, r, l, z)) { usuarios[idx].rol = r; usuarios[idx].local = l; usuarios[idx].zona = z; }
                }
            }
        }
        string comentario;
        if (!pedirComentarioCritico("editar credencial", comentario)) { usuarios[idx] = anterior; cout << "Cambios cancelados.\n"; pausar(); return; }
        string antes = anterior.usuario + " / " + anterior.clave + " / " + anterior.nombrePersona + " / " + anterior.rol + " / " + anterior.local + " / " + anterior.zona;
        string despues = usuarios[idx].usuario + " / " + usuarios[idx].clave + " / " + usuarios[idx].nombrePersona + " / " + usuarios[idx].rol + " / " + usuarios[idx].local + " / " + usuarios[idx].zona;
        registrarAuditoria("Editar credencial", "Edicion de usuario", antes, despues, comentario, "Registrado por gerente");
        if (anterior.usuario == sesion.usuario) sesion = usuarios[idx];
        cout << "Credencial editada correctamente.\n";
        pausar();
    }

    void eliminarCredencialGerente() {
        if (!esGerenteGeneral()) { cout << "Solo el gerente general puede eliminar credenciales.\n"; pausar(); return; }
        mostrarCuadroCredenciales();
        int id = pedirEntero("Numero de credencial a eliminar (0 cancela): ", 0, (int)usuarios.size());
        if (id == 0) return;
        int idx = id - 1;
        if (usuarios[idx].usuario == sesion.usuario) { cout << "No puede eliminar la credencial con la que inicio sesion.\n"; pausar(); return; }
        if (usuarios[idx].usuario == "gerente") { cout << "No se puede eliminar la credencial principal del gerente.\n"; pausar(); return; }
        string comentario;
        if (!pedirComentarioCritico("eliminar credencial", comentario)) return;
        Usuario borrado = usuarios[idx];
        string antes = borrado.usuario + " / " + borrado.clave + " / " + borrado.nombrePersona + " / " + borrado.rol + " / " + borrado.local + " / " + borrado.zona;
        usuarios.erase(usuarios.begin() + idx);
        registrarAuditoria("Eliminar credencial", "Usuario eliminado: " + borrado.usuario, antes, "Eliminado", comentario, "Registrado por gerente");
        cout << "Credencial eliminada correctamente.\n";
        pausar();
    }

    void exportarCredencialesCSV() {
        ofstream out("reporte_credenciales.csv");
        out << "Usuario,Clave,Persona,Rol,Local,Zona\n";
        for (const auto &u : usuarios) {
            out << u.usuario << "," << u.clave << ","" << u.nombrePersona << "","" << u.rol << "","" << u.local << "","" << u.zona << ""\n";
        }
        cout << "Archivo generado: reporte_credenciales.csv\n";
        pausar();
    }

    void menuUsuarios() {
        while (true) {
            cout << "\n--- USUARIOS, PERMISOS Y CREDENCIALES ---\n";
            cout << "1. Ver cuadro de credenciales\n";
            cout << "2. Crear credencial (solo gerente)\n";
            cout << "3. Editar credencial (solo gerente)\n";
            cout << "4. Eliminar credencial (solo gerente)\n";
            cout << "5. Exportar credenciales CSV\n";
            cout << "0. Volver\n";
            int op = pedirOpcion(0, 5);
            if (op == 0) return;
            if (op == 1) { mostrarCuadroCredenciales(); pausar(); }
            if (op == 2) crearCredencialGerente();
            if (op == 3) editarCredencialGerente();
            if (op == 4) eliminarCredencialGerente();
            if (op == 5) exportarCredencialesCSV();
        }
    }

    void menuAuditoria() {
        while (true) {
            cout << "\n--- AUDITORIA DE ACCIONES ---\n";
            cout << "1. Ver auditoria\n";
            cout << "2. Cambiar estado de auditoria\n";
            cout << "3. Registrar accion simulada para prueba\n";
            cout << "0. Volver\n";
            int op = pedirOpcion(0, 3); if (op == 0) return;
            if (op == 1) {
                if (auditorias.empty()) cout << "No hay auditorias registradas.\n";
                for (const auto &a : auditorias) {
                    cout << "\nID " << a.id << " | " << a.fecha << " | " << a.usuario << " | " << a.local << " | " << a.zona << "\n";
                    cout << "Accion: " << a.accion << "\nDetalle: " << a.detalle << "\nAntes: " << a.datoAnterior << "\nNuevo: " << a.datoNuevo << "\nComentario: " << a.comentario << "\nEstado: " << a.estado << "\n";
                }
                pausar();
            } else if (op == 2) {
                if (sesion.rol != "Gerente general" && sesion.rol != "Supervisor zonal") { cout << "Solo gerente o supervisor puede revisar auditorias.\n"; pausar(); continue; }
                int id = pedirEntero("ID auditoria: ", 1, 100000); if (id == 0) continue;
                bool ok = false;
                for (auto &a : auditorias) {
                    if (a.id == id) {
                        if (sesion.rol == "Supervisor zonal" && sesion.zona != a.zona) { cout << "No pertenece a su zona.\n"; ok = true; break; }
                        cout << "1. Revisado y aprobado\n2. Revisado con observacion\n3. Revertido\n4. Requiere explicacion adicional\n";
                        int e = pedirOpcion(1, 4);
                        a.estado = (e == 1 ? "Revisado y aprobado" : (e == 2 ? "Revisado con observacion" : (e == 3 ? "Revertido" : "Requiere explicacion adicional")));
                        cout << "Estado actualizado.\n"; ok = true; break;
                    }
                }
                if (!ok) cout << "No encontrado.\n";
                pausar();
            } else {
                string accion = pedirTexto("Accion realizada: ", true); if (isExitText(accion)) continue;
                string detalle = pedirTexto("Detalle: ", true); if (isExitText(detalle)) continue;
                string ant = pedirTexto("Dato anterior: ", false);
                string nue = pedirTexto("Dato nuevo: ", false);
                string comentario;
                if (!pedirComentarioCritico(accion, comentario)) continue;
                registrarAuditoria(accion, detalle, ant, nue, comentario);
                cout << "Accion registrada en auditoria.\n";
                pausar();
            }
        }
    }

    void menuIA() {
        while (true) {
            cout << "\n--- INTELIGENCIA ARTIFICIAL BASICA SIMULADA ---\n";
            cout << "1. Sugerir plan de compra semanal\n";
            cout << "2. Sugerir distribucion por local\n";
            cout << "3. Reconocer producto por imagen simulada\n";
            cout << "0. Volver\n";
            int op = pedirOpcion(0, 3); if (op == 0) return;
            if (op == 1) sugerirCompraIA();
            if (op == 2) sugerirDistribucionIA();
            if (op == 3) {
                string img = lowerText(pedirTexto("Nombre de imagen, por ejemplo red_label.jpg (0 vuelve): ", false));
                if (!isExitText(img) && !img.empty()) {
                    cout << "\nIA simulada revisa la imagen: " << img << "\n";
                    int encontrados = 0;
                    for (const auto &p : productos) {
                        string data = lowerText(p.nombre + " " + p.marca + " " + p.categoria + " " + p.codigo);
                        string limpio = img;
                        replace(limpio.begin(), limpio.end(), '_', ' ');
                        replace(limpio.begin(), limpio.end(), '-', ' ');
                        if (data.find(limpio.substr(0, min<size_t>(limpio.size(), 8))) != string::npos || limpio.find(lowerText(p.marca)) != string::npos) {
                            cout << "Posible producto: ID " << p.id << " | " << p.nombre << " | " << p.marca << " | " << money(p.precioMenor) << "\n";
                            if (++encontrados >= 8) break;
                        }
                    }
                    if (encontrados == 0) cout << "No se encontro coincidencia clara. Registre manualmente o busque por marca/nombre.\n";
                }
            }
            pausar();
        }
    }

    void sugerirCompraIA() {
        cout << "\nPLAN BASICO DE COMPRA SEMANAL (SIMULADO)\n";
        cout << left << setw(5) << "ID" << setw(35) << "Producto" << setw(12) << "StockG" << setw(15) << "CompraSug" << "Motivo\n";
        int mostrados = 0;
        for (const auto &p : productos) {
            int sg = stockGeneral(p);
            if (sg < 60 && mostrados < 20) {
                int sug = max(24, 90 - sg);
                cout << setw(5) << p.id << setw(35) << p.nombre.substr(0,34) << setw(12) << sg << setw(15) << sug << "Stock bajo y rotacion estimada\n";
                mostrados++;
            }
        }
    }

    void sugerirDistribucionIA() {
        int idx = escogerProductoPorBusqueda(); if (idx < 0) return;
        int compra = pedirEntero("Cantidad comprada para distribuir: ", 1, 100000); if (compra == 0) return;
        vector<int> pesos = {40, 20, 15, 10, 10, 5};
        int asignado = 0;
        cout << "\nDistribucion sugerida para " << productos[idx].nombre << ":\n";
        for (int i = 0; i < (int)locales.size(); ++i) {
            int cant = (i == (int)locales.size()-1) ? (compra - asignado) : (compra * pesos[i] / 100);
            asignado += cant;
            cout << "  " << locales[i].nombre << ": " << cant << " unidades\n";
        }
        cout << "Criterio: ventas promedio simuladas y distancia por zona.\n";
    }

    void ejecutarComandoVoz(const string &cmd) {
        string c = lowerText(cmd);
        if (c.find("producto") != string::npos) menuProductos();
        else if (c.find("cliente") != string::npos) menuClientes();
        else if (c.find("venta") != string::npos) menuVentas();
        else if (c.find("comprobante") != string::npos || c.find("ticket") != string::npos) menuComprobantes();
        else if (c.find("stock por local") != string::npos || c.find("stock local") != string::npos) reporteStockPorLocal();
        else if (c.find("stock general") != string::npos || c == "stock") reporteStockGeneral();
        else if (c.find("transferencia") != string::npos) menuTransferencias();
        else if (c.find("combo") != string::npos) menuCombos();
        else if (c.find("solicitado") != string::npos || c.find("pedido") != string::npos) menuSolicitudes();
        else if (c.find("compra sugerida") != string::npos || c.find("plan de compra") != string::npos) { sugerirCompraIA(); pausar(); }
        else if (c.find("distribucion sugerida") != string::npos || c.find("distribuir") != string::npos) { sugerirDistribucionIA(); pausar(); }
        else if (c.find("compra") != string::npos || c.find("proveedor") != string::npos) menuComprasProveedores();
        else if (c.find("factura") != string::npos) menuFacturasIA();
        else if (c.find("codigo") != string::npos || c.find("barra") != string::npos) menuCodigos();
        else if (c.find("reporte") != string::npos || c.find("excel") != string::npos) menuReportesCSV();
        else if (c.find("vencimiento") != string::npos || c.find("fefo") != string::npos) menuVencimientos();
        else if (c.find("usuario") != string::npos || c.find("credencial") != string::npos || c.find("clave") != string::npos) menuUsuarios();
        else if (c.find("auditoria") != string::npos) menuAuditoria();
        else if (c == "ia" || c.find("inteligencia") != string::npos) menuIA();
        else if (c.find("camara") != string::npos || c.find("visual") != string::npos) menuCamaras();
        else if (c.find("config") != string::npos) menuConfiguracion();
        else if (c.find("guardar") != string::npos) guardarInformacion();
        else if (c.find("recuperar") != string::npos) recuperarInformacion();
        else {
            cout << "Comando no reconocido. Use ayuda para ver comandos disponibles.\n";
            pausar();
        }
    }

    void mostrarAyudaVoz() {
        cout << "\nComandos disponibles:\n";
        cout << "productos, clientes, ventas, comprobantes, ticket, stock, stock general, stock por local\n";
        cout << "transferencias, combos, productos solicitados, compras, proveedores, facturas\n";
        cout << "codigos, barras, reportes, excel, vencimientos, fefo, usuarios, credenciales\n";
        cout << "auditoria, ia, inteligencia artificial, compra sugerida, distribucion sugerida\n";
        cout << "camaras, control visual, configuracion, guardar, recuperar, salir\n";
    }

    void menuVoz() {
        while (true) {
            cout << "\n--- COMANDOS POR VOZ SIMULADOS ---\n";
            cout << "Escriba una orden como si fuera voz. Escriba ayuda para ver comandos.\n";
            string cmd = pedirTexto("Comando (0 vuelve): ", false);
            if (isExitText(cmd) || cmd.empty()) return;
            string c = lowerText(cmd);
            if (c == "ayuda" || c == "comandos") { mostrarAyudaVoz(); pausar(); continue; }
            ejecutarComandoVoz(c);
        }
    }

    void menuCamaras() {
        while (true) {
            cout << "\n--- CAMARAS Y CONTROL VISUAL ---\n";
            cout << "1. Simular diferencia de stock funcional\n";
            cout << "2. Ver niveles de alerta\n";
            cout << "0. Volver\n";
            int op = pedirOpcion(0, 2); if (op == 0) return;
            if (op == 2) {
                cout << "Nivel 1: Producto retirado de vitrina o estante\n";
                cout << "Nivel 2: Diferencia visual entre camara y stock del sistema\n";
                cout << "Nivel 3: Verificacion de venta o movimiento autorizado\n";
                cout << "Nivel 4: Producto no visible o posible ocultamiento\n";
                cout << "Nivel 5: Cliente se dirige a salida sin pago registrado\n";
                cout << "Nivel 6: Alerta critica o posible perdida\n";
            } else {
                int idx = escogerProductoPorBusqueda(); if (idx < 0) continue;
                int loc = seleccionarLocalAccesible(); if (loc < 0) continue;
                string zonaInterna = pedirTexto("Zona observada (vitrina, estante, caja, salida): ", false);
                int detectado = pedirEntero("Cantidad detectada por camara: ", 0, 100000, false);
                int sistema = productos[idx].stock[loc];
                cout << "\nProducto: " << productos[idx].nombre << "\nLocal: " << locales[loc].nombre << "\nZona: " << zonaInterna << "\n";
                cout << "Stock sistema: " << sistema << " | Camara detecta: " << detectado << "\n";
                if (detectado >= sistema) {
                    cout << "Estado: Sin inconsistencia. No se genera alerta.\n";
                } else {
                    int dif = sistema - detectado;
                    cout << "Diferencia visual: " << dif << " unidad(es).\n";
                    string venta = lowerText(pedirTexto("Hubo venta o movimiento autorizado en ese lapso? (s/n): ", false));
                    if (venta == "s" || venta == "si") {
                        cout << "Estado: diferencia justificada. No se emite alerta.\n";
                    } else {
                        cout << "Revision DVR simulada: producto retirado sin venta registrada.\n";
                        cout << "1. Cliente sigue comprando o va a caja\n";
                        cout << "2. Producto no visible o posible ocultamiento\n";
                        cout << "3. Cliente se dirige a zona de salida sin pago\n";
                        int ruta = pedirOpcion(1, 3);
                        if (ruta == 1) cout << "Resultado: observacion silenciosa. Supervisar sin alarmar.\n";
                        if (ruta == 2) cout << "Resultado: ALERTA SILENCIOSA al encargado y supervisor.\n";
                        if (ruta == 3) cout << "Resultado: ALERTA CRITICA. Intervencion inmediata antes de salida.\n";
                    }
                }
            }
            pausar();
        }
    }

    void menuConfiguracion() {
        while (true) {
            cout << "\n--- CONFIGURACION Y CREDENCIALES ---\n";
            cout << "1. Ver zonas y locales\n";
            cout << "2. Gestionar credenciales de usuarios (solo gerente)\n";
            cout << "3. Ver politicas de seguridad\n";
            cout << "0. Volver\n";
            int op = pedirOpcion(0, 3);
            if (op == 0) return;
            if (op == 1) {
                cout << "Zonas:\n";
                cout << "  Zona Alto Huallaga: Tingo Maria, Aucayacu, Tocache\n";
                cout << "  Zona Federico Basadre: Aguaytia, San Alejandro, Km 86\n";
                pausar();
            }
            if (op == 2) menuUsuarios();
            if (op == 3) {
                cout << "Seguridad activa:\n";
                cout << "- Usuario y clave obligatorios.\n";
                cout << "- Claves unicas de 3 digitos.\n";
                cout << "- No se permite clave igual al usuario.\n";
                cout << "- Gerente puede crear, editar y eliminar credenciales.\n";
                cout << "- Acciones criticas quedan en auditoria.\n";
                cout << "Usuarios cargados: " << usuarios.size() << "\n";
                pausar();
            }
        }
    }

    void guardarInformacion() {
        exportarStockCSV();
        exportarVentasCSV();
        exportarAuditoriaCSV();

        ofstream out("sistemaleon_datos.dat");
        if (!out) {
            cout << "No se pudo crear el archivo de respaldo.\n";
            pausar();
            return;
        }

        out << "SISTEMA_LEON_BACKUP_V1\n";
        out << quoted(nowText()) << "\n";

        out << productos.size() << "\n";
        for (const auto &p : productos) {
            out << p.id << " "
                << quoted(p.codigo) << " "
                << quoted(p.codigoBarras) << " "
                << quoted(p.nombre) << " "
                << quoted(p.marca) << " "
                << quoted(p.categoria) << " "
                << quoted(p.presentacion) << " "
                << p.costo << " "
                << p.precioMenor << " "
                << p.precioMayor << " "
                << p.stockMinimo << " "
                << quoted(p.fechaVencimiento) << " "
                << p.activo << " "
                << p.stock.size();
            for (int s : p.stock) out << " " << s;
            out << "\n";
        }

        out << clientes.size() << "\n";
        for (const auto &c : clientes) {
            out << c.id << " "
                << quoted(c.nombre) << " "
                << quoted(c.tipoDoc) << " "
                << quoted(c.numeroDoc) << " "
                << quoted(c.telefono) << "\n";
        }

        out << ventas.size() << "\n";
        for (const auto &v : ventas) {
            out << v.id << " "
                << quoted(v.fecha) << " "
                << v.localIndex << " "
                << quoted(v.local) << " "
                << quoted(v.cajero) << " "
                << v.cliente.id << " "
                << quoted(v.cliente.nombre) << " "
                << quoted(v.cliente.tipoDoc) << " "
                << quoted(v.cliente.numeroDoc) << " "
                << quoted(v.cliente.telefono) << " "
                << v.subtotal << " "
                << v.descuento << " "
                << v.conIGV << " "
                << v.igv << " "
                << v.total << " "
                << v.detalles.size() << " "
                << v.pagos.size() << "\n";

            for (const auto &d : v.detalles) {
                out << d.productoId << " "
                    << quoted(d.codigo) << " "
                    << quoted(d.nombre) << " "
                    << d.cantidad << " "
                    << d.precioUnitario << " "
                    << d.subtotal << "\n";
            }

            for (const auto &pago : v.pagos) {
                out << quoted(pago.metodo) << " " << pago.monto << "\n";
            }
        }

        out << auditorias.size() << "\n";
        for (const auto &a : auditorias) {
            out << a.id << " "
                << quoted(a.fecha) << " "
                << quoted(a.usuario) << " "
                << quoted(a.rol) << " "
                << quoted(a.local) << " "
                << quoted(a.zona) << " "
                << quoted(a.accion) << " "
                << quoted(a.detalle) << " "
                << quoted(a.datoAnterior) << " "
                << quoted(a.datoNuevo) << " "
                << quoted(a.comentario) << " "
                << quoted(a.estado) << "\n";
        }

        out << solicitudes.size() << "\n";
        for (const auto &s : solicitudes) {
            out << s.id << " "
                << quoted(s.fecha) << " "
                << quoted(s.local) << " "
                << quoted(s.cliente) << " "
                << quoted(s.producto) << " "
                << quoted(s.cantidad) << " "
                << quoted(s.observacion) << "\n";
        }

        out.close();

        cout << "\nRespaldo guardado correctamente.\n";
        cout << "Archivo principal de recuperacion: sistemaleon_datos.dat\n";
        cout << "Tambien se actualizaron los reportes CSV de stock, ventas y auditoria.\n";
        pausar();
    }

    void recuperarInformacion() {
        cout << "\n--- RECUPERAR INFORMACION ---\n";

        ifstream verificar("sistemaleon_datos.dat");
        if (!verificar) {
            cout << "No se encontraron respaldos disponibles para recuperar.\n";
            cout << "Primero debe usar la opcion 'Guardar informacion' para generar un respaldo.\n";
            pausar();
            return;
        }

        string firma, fechaRespaldo;
        getline(verificar, firma);
        verificar >> quoted(fechaRespaldo);
        verificar.close();

        if (firma != "SISTEMA_LEON_BACKUP_V1") {
            cout << "Se encontro un archivo de respaldo, pero su formato no es compatible.\n";
            pausar();
            return;
        }

        cout << "Respaldos disponibles:\n";
        cout << "1. sistemaleon_datos.dat";
        if (!fechaRespaldo.empty()) cout << " | Generado: " << fechaRespaldo;
        cout << "\n";
        cout << "0. Volver\n";

        int op = pedirOpcion(0, 1);
        if (op == 0) return;

        cout << "\nDatos disponibles para recuperar:\n";
        cout << "- Productos y stock por local\n";
        cout << "- Clientes\n";
        cout << "- Ventas y formas de pago\n";
        cout << "- Auditoria de acciones\n";
        cout << "- Solicitudes de productos\n";

        string confirmar = lowerText(pedirTexto("\nDesea recuperar este respaldo? (s/n): ", false));
        if (confirmar != "s" && confirmar != "si") {
            cout << "Recuperacion cancelada.\n";
            pausar();
            return;
        }

        ifstream in("sistemaleon_datos.dat");
        if (!in) {
            cout << "No se pudo abrir el respaldo seleccionado.\n";
            pausar();
            return;
        }

        string encabezado, fecha;
        getline(in, encabezado);
        in >> quoted(fecha);

        vector<Producto> productosRec;
        vector<Cliente> clientesRec;
        vector<Venta> ventasRec;
        vector<Auditoria> auditoriasRec;
        vector<SolicitudProducto> solicitudesRec;

        size_t n = 0;

        in >> n;
        for (size_t i = 0; i < n; ++i) {
            Producto p;
            size_t stockCount = 0;
            in >> p.id
               >> quoted(p.codigo)
               >> quoted(p.codigoBarras)
               >> quoted(p.nombre)
               >> quoted(p.marca)
               >> quoted(p.categoria)
               >> quoted(p.presentacion)
               >> p.costo
               >> p.precioMenor
               >> p.precioMayor
               >> p.stockMinimo
               >> quoted(p.fechaVencimiento)
               >> p.activo
               >> stockCount;
            p.stock.resize(stockCount);
            for (size_t j = 0; j < stockCount; ++j) in >> p.stock[j];
            productosRec.push_back(p);
        }

        in >> n;
        for (size_t i = 0; i < n; ++i) {
            Cliente c;
            in >> c.id
               >> quoted(c.nombre)
               >> quoted(c.tipoDoc)
               >> quoted(c.numeroDoc)
               >> quoted(c.telefono);
            clientesRec.push_back(c);
        }

        in >> n;
        for (size_t i = 0; i < n; ++i) {
            Venta v;
            size_t nd = 0, np = 0;
            in >> v.id
               >> quoted(v.fecha)
               >> v.localIndex
               >> quoted(v.local)
               >> quoted(v.cajero)
               >> v.cliente.id
               >> quoted(v.cliente.nombre)
               >> quoted(v.cliente.tipoDoc)
               >> quoted(v.cliente.numeroDoc)
               >> quoted(v.cliente.telefono)
               >> v.subtotal
               >> v.descuento
               >> v.conIGV
               >> v.igv
               >> v.total
               >> nd
               >> np;

            for (size_t j = 0; j < nd; ++j) {
                DetalleVenta d;
                in >> d.productoId
                   >> quoted(d.codigo)
                   >> quoted(d.nombre)
                   >> d.cantidad
                   >> d.precioUnitario
                   >> d.subtotal;
                v.detalles.push_back(d);
            }

            for (size_t j = 0; j < np; ++j) {
                Pago pago;
                in >> quoted(pago.metodo) >> pago.monto;
                v.pagos.push_back(pago);
            }
            ventasRec.push_back(v);
        }

        in >> n;
        for (size_t i = 0; i < n; ++i) {
            Auditoria a;
            in >> a.id
               >> quoted(a.fecha)
               >> quoted(a.usuario)
               >> quoted(a.rol)
               >> quoted(a.local)
               >> quoted(a.zona)
               >> quoted(a.accion)
               >> quoted(a.detalle)
               >> quoted(a.datoAnterior)
               >> quoted(a.datoNuevo)
               >> quoted(a.comentario)
               >> quoted(a.estado);
            auditoriasRec.push_back(a);
        }

        in >> n;
        for (size_t i = 0; i < n; ++i) {
            SolicitudProducto s;
            in >> s.id
               >> quoted(s.fecha)
               >> quoted(s.local)
               >> quoted(s.cliente)
               >> quoted(s.producto)
               >> quoted(s.cantidad)
               >> quoted(s.observacion);
            solicitudesRec.push_back(s);
        }

        if (!in.good() && !in.eof()) {
            cout << "El respaldo contiene datos incompletos o dañados. No se realizaron cambios.\n";
            pausar();
            return;
        }

        productos = productosRec;
        clientes = clientesRec;
        ventas = ventasRec;
        auditorias = auditoriasRec;
        solicitudes = solicitudesRec;

        nextCliente = 1;
        for (const auto &c : clientes) nextCliente = max(nextCliente, c.id + 1);

        nextVenta = 1;
        for (const auto &v : ventas) nextVenta = max(nextVenta, v.id + 1);

        nextAuditoria = 1;
        for (const auto &a : auditorias) nextAuditoria = max(nextAuditoria, a.id + 1);

        nextSolicitud = 1;
        for (const auto &s : solicitudes) nextSolicitud = max(nextSolicitud, s.id + 1);

        cout << "\nInformacion recuperada correctamente.\n";
        cout << "Respaldo restaurado: " << fecha << "\n";
        cout << "Productos: " << productos.size() << "\n";
        cout << "Clientes: " << clientes.size() << "\n";
        cout << "Ventas: " << ventas.size() << "\n";
        cout << "Registros de auditoria: " << auditorias.size() << "\n";
        cout << "Solicitudes: " << solicitudes.size() << "\n";
        pausar();
    }
};

int main() {
    ios::sync_with_stdio(true);
    cin.tie(nullptr);

    cout << endl;
    cout << "ARRANQUE OK: sistemaleon_ch.cpp se esta ejecutando." << endl;
    cout << "Cargando datos iniciales..." << endl;
    cout.flush();

    try {
        SistemaVentas sistema;
        cout << "Datos cargados correctamente." << endl;
        cout << "Entrando al sistema..." << endl;
        cout.flush();
        sistema.ejecutar();
    } catch (const exception &e) {
        cout << "Error controlado: " << e.what() << "\n";
        cout << "El sistema no se cerro de forma brusca.\n";
    } catch (...) {
        cout << "Error inesperado controlado.\n";
    }
    return 0;
}


