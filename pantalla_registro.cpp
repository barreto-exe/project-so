#include "pantalla_registro.h"
#include "ui_pantalla_registro.h"
#include <QMessageBox>
#include <QPixmap>
#include <QDateTime>
#include <ctime>
#include "persona.h"
#include "global.h"
#include <pthread.h>

bool cliente;
Persona *datosPersona;

pantalla_registro::pantalla_registro(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::pantalla_registro)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0); //Hace que la pantalla de seleccion sea la predeterminada
}

pantalla_registro::~pantalla_registro()
{
    delete ui;
}

// Estructura con argumentos de la función para registrar clientes
struct argRegCliente
{
    Persona *datos;
    const char *correo;
    const char *password;
    int resul;
};

// Función del hilo para registro de cliente
void *hiloRegCliente(void *args)
{
    argRegCliente *argumentos = (argRegCliente *) args;
    Persona *datos = argumentos->datos;
    const char *correo = argumentos->correo;
    const char *password = argumentos->password;

    argumentos->resul = Global::db.registrarCliente(*datos, correo, password);

    pthread_exit(NULL);
}

void pantalla_registro::on_btnAceptarPersona_clicked() //Boton aceptar al introducir los datos del registro
{
    QMessageBox msgBox;
    
    //Si las contraseñas son iguales
    if(ui->password->text() == ui->repPassword->text())  {
        //Y ningun campo esta vacio, procede a registrar/guardar los datos
        if(!ui->nombre->text().isEmpty() && !ui->apellido->text().isEmpty() && !ui->cedula->text().isEmpty() && !ui->telefono->text().isEmpty() && !ui->direccion->text().isEmpty() && !ui->fechaNac->text().isEmpty() && !ui->password->text().isEmpty()){

            datosPersona = new Persona(ui->nombre->text().toStdString(),
                                                ui->apellido->text().toStdString(),
                                                ui->cedula->text().toStdString(),
                                                ui->telefono->text().toStdString(),
                                                ui->correo->text().toStdString().c_str(),
                                                ui->direccion->text().toStdString(),
                                                ui->fechaNac->date());
            if(cliente){
                //Lo registra si es un cliente

                // Prepara los argumentos
                argRegCliente *args = new argRegCliente;
                args->datos = datosPersona;
                args->correo = ui->correo->text().toStdString().c_str();
                args->password = ui->password->text().toStdString().c_str();

                // Crea un nuevo hilo para el manejo del inicio de sesión
                pthread_t hilo;
                pthread_create(&hilo, NULL, hiloRegCliente, args);
                pthread_join(hilo, NULL);

                // Resultado del registro
                int reg = args->resul;
                delete args;

                if (reg==1){
                    msgBox.setText("Se ha registrado con éxito!");
                    emit inicioSignal();
                } else {
                    msgBox.setText("Chequee sus datos del registro por favor");
                }
            } else {
                //Si es un transportista, pasa a la ventana con los dados adicionales
                ui->stackedWidget->setCurrentIndex(3);
            }
        } else {
            //Si hay algun campo vacio
            msgBox.setText("Por favor rellene todos los campos");
        }
     } else {
        //Si las contraseñas son distintas
        msgBox.setText("Las contraseñas no coinciden, intentelo nuevamente");
    }
    msgBox.exec();
}


/* botones para cambiar de pantallas */
void pantalla_registro::on_atrasProv_clicked() //vuelve a pantalla de registro
{
    ui->stackedWidget->setCurrentIndex(0);
}

void pantalla_registro::on_atrasTransp_clicked() //vuelve al registro de persona
{
    ui->stackedWidget->setCurrentIndex(1);
}

void pantalla_registro::on_atrasRegistroPersona_clicked() //vuelve a la seleccion de usuario
{
    ui->stackedWidget->setCurrentIndex(0);
}

void pantalla_registro::on_btnCliente_clicked()//va al registro de persona
{
    ui->stackedWidget->setCurrentIndex(1);
    cliente = true;
}

void pantalla_registro::on_btnProveedor_clicked()//va al registro de proveedor
{
    ui->stackedWidget->setCurrentIndex(2);

}

void pantalla_registro::on_btnTransp_clicked()//va al registro de persona
{
    ui->stackedWidget->setCurrentIndex(1);
    cliente = false;
}

// Estructura con argumentos de la función para registrar transportista
struct argRegTrans
{
    Persona *datos;
    Vehiculo *vehiculo;
    const char *correo;
    const char *password;
    int resul;
};

// Función del hilo para registro de cliente
void *hiloRegTrans(void *args)
{
    argRegTrans *argumentos = (argRegTrans *) args;
    Persona *datos = argumentos->datos;
    Vehiculo *vehiculo = argumentos->vehiculo;
    const char *correo = argumentos->correo;
    const char *password = argumentos->password;

    argumentos->resul = Global::db.registrarTransportista(*datos, *vehiculo, correo, password);

    pthread_exit(NULL);
}

void pantalla_registro::on_btnAceptarTransp_clicked()
{
    QMessageBox msgBox;
    if (ui->comboBoxTipo->currentIndex()!= 0 && !ui->modelo->text().isEmpty() && !ui->placa->text().isEmpty()){
        //Si ningun campo esta vacio, registra al transportista
        Vehiculo *vehiculo = new Vehiculo(ui->modelo->text().toStdString(), ui->placa->text().toStdString(), ui->comboBoxTipo->currentText().toStdString());

        // Prepara los argumentos
        argRegTrans *args = new argRegTrans;
        args->datos = datosPersona;
        args->vehiculo = vehiculo;
        args->correo = ui->correo->text().toStdString().c_str();
        args->password = ui->password->text().toStdString().c_str();

        // Crea un nuevo hilo para el manejo del registro
        pthread_t hilo;
        pthread_create(&hilo, NULL, hiloRegTrans, args);
        pthread_join(hilo, NULL);

        // Resultado del registro
        int reg = args->resul;
        delete args;

        if (reg==1){
            msgBox.setText("Se ha registrado con éxito!");
            emit inicioSignal();
        } else {
            msgBox.setText("Chequee sus datos del registro por favor");
        }
    } else {
        msgBox.setText("Por favor rellene todos los campos");
    }
    msgBox.exec();
}

void pantalla_registro::on_atrasSeleccion_clicked()
{
    emit inicioSignal();
}

// Estructura con argumentos de la función para registrar proveedor
struct argRegProv
{
    Proveedor *prov;
    const char *correo;
    const char *password;
    int resul;
};

// Función del hilo para registro de proveedores
void *hiloRegProv(void *args)
{
    argRegProv *argumentos = (argRegProv *) args;
    Proveedor *prov = argumentos->prov;
    const char *correo = argumentos->correo;
    const char *password = argumentos->password;

    argumentos->resul = Global::db.registrarProveedor(*prov, correo, password);

    pthread_exit(NULL);
}

void pantalla_registro::on_btnAceptarProv_clicked()
{
    QMessageBox msgBox;
    //Si las contraseñas coinciden
    if(ui->passwordProv->text() == ui->repPasswordProv->text()){
        //Y ningun campo esta sin llenar, procede a registrar al proveedor
        if (!ui->passwordProv->text().isEmpty() && !ui->nombreProv->text().isEmpty() && !ui->descripcion->text().isEmpty() && !ui->telefonoProv->text().isEmpty() && !ui->direccionProv->text().isEmpty() && !ui->tipoProv->text().isEmpty() && !ui->correoProv->text().isEmpty()){

            Proveedor *proveedor = new Proveedor(ui->nombreProv->text().toStdString(),
                                                 ui->descripcion->text().toStdString(),
                                                 ui->telefonoProv->text().toStdString(),
                                                 ui->correoProv->text().toStdString(),
                                                 ui->direccionProv->text().toStdString(),
                                                 ui->tipoProv->text().toStdString());

            // Prepara los argumentos
            argRegProv *args = new argRegProv;
            args->prov = proveedor;
            args->correo = ui->correoProv->text().toStdString().c_str();
            args->password = ui->passwordProv->text().toStdString().c_str();

            // Crea un nuevo hilo para el manejo del registro
            pthread_t hilo;
            pthread_create(&hilo, NULL, hiloRegProv, args);
            pthread_join(hilo, NULL);

            // Resultado del registro
            int reg = args->resul;
            delete args;

            if (reg==1){
                msgBox.setText("Se ha registrado con éxito!");
                emit inicioSignal();
            } else {
                msgBox.setText("Chequee sus datos del registro por favor");
            }
        } else {
            //Existe algun campo vacio
            msgBox.setText("Por favor rellene todos los campos");   
        }
    } else {
        //Si las contraseñas son distintas
        msgBox.setText("Las contraseñas no coinciden, intentelo nuevamente");

    }
    msgBox.exec();
}
