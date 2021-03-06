// Impplementación de la clase Solicitud
#include "solicitud.h"

Solicitud::Solicitud()
{
    proveedor = Proveedor();
    cliente = Persona();
    tipoPago = "";
    monto = 0;
    fechaPedido = QDate();
    fechaEntrega = QDate();
    direccion = "";
    pedido = vector <producto_cantidad>();
    id = 0;
}

Solicitud::Solicitud(Proveedor proveedor, Persona cliente, string tipoPago, float monto, QDate fechaPedido, QDate fechaEntrega, string direccion, vector <producto_cantidad> pedido)
{
    this->proveedor = proveedor;
    this->cliente = cliente;
    this->tipoPago = tipoPago;
    this->monto = monto;
    this->fechaPedido = fechaPedido;
    this->fechaEntrega = fechaEntrega;
    this->direccion = direccion;
    this->pedido = pedido;
    id = 0;
}

Solicitud::Solicitud(Proveedor proveedor, string tipoPago, QDate fechaPedido, QDate fechaEntrega, string direccion, vector <producto_cantidad> pedido)
{
    this->proveedor = proveedor;
    cliente = Persona();
    this->tipoPago = tipoPago;
    this->fechaPedido = fechaPedido;
    this->fechaEntrega = fechaEntrega;
    this->direccion = direccion;
    this->pedido = pedido;
    calcularMonto();
    id = 0;
}

Solicitud::Solicitud(Persona cliente, string tipoPago, float monto, QDate fechaPedido, QDate fechaEntrega, string direccion, vector <producto_cantidad> pedido)
{
    proveedor = Proveedor();
    this->cliente = cliente;
    this->tipoPago = tipoPago;
    this->monto = monto;
    this->fechaPedido = fechaPedido;
    this->fechaEntrega = fechaEntrega;
    this->direccion = direccion;
    this->pedido = pedido;
    id = 0;
}

// Establece el proveedor
Solicitud &Solicitud::setProveedor(Proveedor proveedor)
{
    this->proveedor = proveedor;

    return *this; // Permite el proceso en cascada
}

// Establece el cliente
Solicitud &Solicitud::setCliente(Persona cliente)
{
    this->cliente = cliente;

    return *this; // Permite el proceso en cascada
}

// Establece el tipo de pago
Solicitud &Solicitud::setTipoPago(string tipoPago)
{
    this->tipoPago = tipoPago;
    
    return  *this; // Permite el proceso en cascada
}

// Establece el monto
Solicitud &Solicitud::setMonto(float monto)
{
    this->monto = monto;
    
    return  *this; // Permite el proceso en cascada
}

// Establece la fecha de realizacion de pedido
Solicitud &Solicitud::setFechaPedido(QDate fechaPedido)
{
    this->fechaPedido = fechaPedido;
    
    return  *this; // Permite el proceso en cascada
}

// Establece la fecha de realizacion de entrega
Solicitud &Solicitud::setFechaEntrega(QDate fechaEntrega)
{
    this->fechaEntrega = fechaEntrega;
    
    return  *this; // Permite el proceso en cascada
}

// Establece el estatus
Solicitud &Solicitud::setEstatus(string estatus)
{
    this->estatus = estatus;
    
    return  *this; // Permite el proceso en cascada
}

// Establece la dirección
Solicitud &Solicitud::setDireccion(string direccion)
{
    this->direccion = direccion;

    return *this; // Permite el proceso en cascada
}

// Establece el pedido
Solicitud &Solicitud::setPedido(vector <producto_cantidad> pedido)
{
    this->pedido = pedido;

    return *this; // Permite el proceso en cascada
}

// Establece el ID
Solicitud &Solicitud::setId(int id)
{
    this->id = id;

    return *this; // Permite el proceso en cascada
}

// Devuelve el proveedor
Proveedor Solicitud::getProveedor() const { return proveedor; }

// Devuelve el cliente
Persona Solicitud::getCliente() const { return cliente; }

// Devuelve el tipo de pago
string Solicitud::getTipoPago() const { return tipoPago; }

// Devuelve el monto
float Solicitud::getMonto() const { return monto; }

// Devuelve la fecha de realización del pedido
QDate Solicitud::getFechaPedido() const { return fechaPedido; }

// Devuelve la fecha de realización de la entrega
QDate Solicitud::getFechaEntrega() const { return fechaEntrega; }

// Devuelve el estatus
string Solicitud::getEstatus() const { return estatus; }

// Devuelve la direccion
string Solicitud::getDireccion() const { return direccion; }

// Devuelve el pedido
vector <producto_cantidad> Solicitud::getPedido() const { return pedido; }

// Devuelve el ID
int Solicitud::getId() const { return id; }

// Agrega un producto con su cantidad al pedido
Solicitud &Solicitud::agregarProducto(producto_cantidad producto)
{
    int i = verificarProducto(producto.producto);
    if (i == -1)
    {
        pedido.push_back(producto);

    }
    else
    {
        pedido[i].cantidad = producto.cantidad;
    }
    calcularMonto();
    return *this;
}

// Elimina un producto del pedido
Solicitud &Solicitud::eliminarProducto(Producto producto)
{
    int i = verificarProducto(producto);

    if (i != -1)
    {
        pedido.erase(pedido.begin() + i);
        calcularMonto();
    }

    return *this;
}

// Calcula el monto total del pedido
Solicitud &Solicitud::calcularMonto()
{
    monto = 0;
    if (!pedido.empty())
        for (std::size_t i = 0; i < pedido.size(); i++)
            monto += pedido[i].producto.getPrecio() * pedido[i].cantidad;

    return *this;
}

int Solicitud::verificarProducto(Producto producto)
{
    for (std::size_t i = 0; i < pedido.size(); i++)
        if (pedido[i].producto.getId() == producto.getId())
            return i;
    return -1;
}

Solicitud &Solicitud::clear()
{
    proveedor = Proveedor();
    cliente = Persona();
    tipoPago = "";
    monto = 0;
    fechaPedido = QDate();
    fechaEntrega = QDate();
    direccion = "";
    pedido = vector <producto_cantidad>();
    id = 0;
    return *this;
}

QString Solicitud::toString()
{
    string resumen = "Solicitud # 00" + std::to_string(id) + " -- " + proveedor.getNombre() + " -- $" + std::to_string(monto) + " -- ";
    return QString(resumen.c_str()) + fechaPedido.toString("dd/MM/yyyy");
}
