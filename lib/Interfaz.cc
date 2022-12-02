#include "../include/Interfaz.h"

Interfaz::Interfaz(Arbol_R_Estrella* A)
: arbol_r(A){
    // Creamos la ventana
    window.create(sf::VideoMode(1000, 720), "R-TREE");
}

Interfaz::Interfaz()
: arbol_r(new Arbol_R_Estrella){
    // Creamos la ventana
    window.create(sf::VideoMode(Constante::N, Constante::N), "R-TREE");
}

void Interfaz::ingresar_coordenada(sf::Event &event)
{
    // Mientras la ventana este abierta
    while (window.isOpen())
    {
        // Consultar eventos
        if (!window.pollEvent(event))
            continue;

        // Cerrar ventana
        if (event.type == sf::Event::Closed){
            window.close();
            return;
        }
        
        // Boton presionado, continuar
        if (event.type != sf::Event::MouseButtonPressed)
            continue;
        // Boton presionado, continuar
        if (event.mouseButton.button != sf::Mouse::Left)
            continue;

        // Agarrar la posicion del mouse, la que indicará la posición del nuevo punto
        sf::Vector2i localPosition = sf::Mouse::getPosition(window);
        // Calculamos la coordenada ya que la posición vertical incrementa a medida que desciende el puntero
        pair<int, int> coordenada = {localPosition.x, int(window.getSize().y) - localPosition.y};
        // Debugear
        printf("arbolito->insertar({{%d, %d}});\n", coordenada.first, coordenada.second);
        // Proceder a insertar la coordenada a la función
        arbol_r->insertar({{coordenada.first, coordenada.second}});
        return;
    }
}

void Interfaz::ingresar_poligono(sf::Event &event)
{
    // Vector que almacenara todos las coordenadas
    vector<Punto> coordenadas;
    // Mientras la ventana siga abierta
    while (window.isOpen())
    {
        // Consultar eventos
        if (!window.pollEvent(event))
            continue;

        // Si se cierra, cerrar ventana
        if (event.type == sf::Event::Closed){
            window.close();
            return;
        }

        // Si presionamos la tecla P (para termianr la inserción de los puntos del polígono)
        if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::P)){
            // Si la cantidad de coordenadas supera a 2, es un poligono, por lo que se inserta al arbol
            if (coordenadas.size() > 2)
                // se llama a la función de insertar
                arbol_r->insertar(coordenadas);
            return;
        }

        // Si clickeamos en una parte del lienzo
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            // Obtenemos la posición relativa del mouse
            sf::Vector2i localPosition = sf::Mouse::getPosition(window);
            // Calculamos la coordenada en el plano cartesiano
            pair<int, int> coordenada = {localPosition.x, int(window.getSize().y) - localPosition.y};

            // Agregamos la coordenada al vector que representa el poligono
            coordenadas.push_back({coordenada.first, coordenada.second});
            // Impresion (DEBUG)
            cout << coordenada.first << '\t' << coordenada.second << endl;

            // Visualizar como se esta creando el poligono
            sf::CircleShape coordenada_vista;
            coordenada_vista.setRadius(2);
            coordenada_vista.setFillColor(sf::Color::Yellow);
            coordenada_vista.setPosition(coordenada.first - 2, window.getSize().y - coordenada.second - 2);
            window.draw(coordenada_vista);
            window.display();
        }
    }
}

void Interfaz::eliminar(sf::Event &event)
{
    // cout << "ELIMINACION" << endl;
    while (1)
    {
        // Mientras no hayan otros eventos, continuar
        if (!window.pollEvent(event))
            continue;

        // COntinuar, si el boton ha sido presionado
        if (event.type != sf::Event::MouseButtonPressed)
            continue;

        // Continuar si no se presiona el boton izquierdo
        if (event.mouseButton.button != sf::Mouse::Left)
            continue;

        // Obtenemos la posicion relativa del mouse en la ventana
        sf::Vector2i localPosition = sf::Mouse::getPosition(window);
        // Calcular la coordenada carteisana
        pair<int, int> coordenada = {localPosition.x, int(window.getSize().y) - localPosition.y};

        // Funcion DEBUG
        printf("arbolito->eliminar({%d, %d});\n", coordenada.first, coordenada.second);
        // Proceder a eliminar
        arbol_r->eliminar({coordenada.first, coordenada.second});

        // terminar función
        return;
    }
}

void Interfaz::eventos()
{
    sf::Event event;
    // COnsultar los enventos
    while (window.pollEvent(event))
    {
        // Si se cierra la vevntana, cerrar
        if (event.type == sf::Event::Closed)
            window.close();
        // La interfaz solo acepta teclas como entradas para las diferentes operaciones
        if (event.type != sf::Event::KeyPressed)
            continue;

        // Insercion de PUNTO
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
            this->ingresar_coordenada(event);
        // Insercion de  POLIGONO
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
            this->ingresar_poligono(event);
        // Eliminación del más cercano
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            this->eliminar(event);
        // Busqueda de los K vecinos más cercanos
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
            this->buscar_k_coordenadas(event);
    }
}

void Interfaz::buscar_k_coordenadas(sf::Event& event){
    // Variable que define la cantidad de vecinos a hallar
    int k {};
    // DEBUG
    cout<<"BUSQUEDA DE "<<k<<" VECINOS:"<<endl;
    // Mientras la ventana siga abierta
    while (window.isOpen())
    {
        // Continuar si no hay eventos
        if (!window.pollEvent(event))
            continue;

        // Si se llama al cierre de la ventana, cerrar la ventana
        if (event.type == sf::Event::Closed){
            window.close();
            return;
        }

        // Si se ingresa un texto, capturar cada caracter
        if(event.type == sf::Event::TextEntered){
            char numero = static_cast<char>(event.text.unicode);
            // Si el caracter es de un digito
            if(numero >= '0' && numero <= '9'){
                // Multiplicar por 10 el K actual
                k*=10;
                // Agregar el digito entrado a A
                k+=(numero-'0');
            }
            // DEBUG
            std::cout<<k<<std::endl;
        }
        
        // Si k== 0 continuar
        if(k<1) continue;

        // Mientras no se presiona el mouse, continuar
        if (event.type != sf::Event::MouseButtonPressed)
            continue;

        // Mientras no se presiona el mouse izquierda, continuar
        if (event.mouseButton.button != sf::Mouse::Left)
            continue;

        // Obtener la posición relativa del puntero para hallar los vecinos
        sf::Vector2i localPosition = sf::Mouse::getPosition(window);
        // Obtenemos la coordenada cartesiana
        pair<int, int> coordenada = {localPosition.x, int(window.getSize().y) - localPosition.y};
        // Definir la linea a dibujar para el vecino
        sf::Vertex linea[2]{ 
            sf::Vector2f(localPosition.x, localPosition.y)
        };
        linea[0].color = sf::Color::White;

        // DEBUG
        cout << coordenada.first << '\t' << coordenada.second << endl;
        // Llamada a la busqueda de vecinos cercanos por la función de busqueda del arbol
        vector<Arbol_R_Estrella::Distante> k_vecinos = arbol_r->buscar({coordenada.first, coordenada.second}, k);
        // Iterar por cada vecino en el vector
        for(auto i: k_vecinos){
            // Definir el punto medio del objeto vecino
            double pm_x=0, pm_y=0;
            // facilitar el acceso al objeto
            Entrada* iEH = i.entrada;
            // Adicionar al punto medio los valores de cada punto
            for(auto p: iEH->objeto){
                pm_x+=p.x;
                pm_y+=p.y;
            }
            // Dividir y obtener el centro por media
            pm_x/=iEH->objeto.size();
            pm_y/=iEH->objeto.size();
            
            // Definir las posiciones de la linea
            linea[1] = sf::Vector2f(static_cast<int>(pm_x), window.getSize().y - static_cast<int>(pm_y));
            linea[1].color = sf::Color::White;
            cout<<linea[1].position.x<<"  "<<linea[1].position.y<<endl;
            // Debigujar la linea de donde se hizo click a los diferentes vecinos cercanos
            window.draw(linea, 2, sf::Lines);
            window.display();
        }
        // Desdibujar las lineas a los vecinos cuando se vuelva a hacer click
        while(window.isOpen()){
            // Continuar si no hay eventos
            if (!window.pollEvent(event))
            continue;
            
            // Cerrar ventana
            if (event.type == sf::Event::Closed){
                window.close();
                return;
            }

            // Continaur si no se presiona el mouse
            if (event.type != sf::Event::MouseButtonPressed)
                continue;

            // Continaur si no se presiona el click izqs
            if (event.mouseButton.button != sf::Mouse::Left)
                continue;
            // Terminar funciony desdibujar lineas
            return;
        }
    }
}

void Interfaz::ejecutar()
{
    // Mientras la ventana este abierta
    while (window.isOpen())
    {   
        // Definir que funcion del arbol se llamará
        this->eventos();

        // Limpiar ventana
        window.clear();
        // Dibujar arbol 
        imprimir_arbol_r();
        // Mostrar dibujos
        window.display();
    }
}

void Interfaz::imprimir_arbol_r(){
    int espacio{10};
    // Dibujamos empezando en la raiz
    imprimir_arbol_r_recursivo(arbol_r->raiz, espacio, 30);
}

// FUNCION DEPRECADA
void Interfaz::imprimir_arbol_r_recursivo(Nodo* nodo, int &espacio, int color){
    // Si es un nodo hoja
    if(nodo->hoja){
        // por todas las entradas
        for(auto i: nodo->entradas){
            Entrada* eh = i;
            // si es solo un punto
            if(eh->objeto.size() == 1){
                // Dibujamos un circulo en la posición del Punto
                sf::CircleShape coordenada;
                coordenada.setRadius(4);
                coordenada.setFillColor(sf::Color::Red);
                coordenada.setPosition(eh->objeto[0].x - 2, window.getSize().y - eh->objeto[0].y - 2);
                window.draw(coordenada);

                // Imprimir coordendas
                /* sf::Text text;
                text.setFont(font);
                text.setString("Punto: "+tupla_string(eh->objeto[0]));
                text.setCharacterSize(10);
                text.setFillColor(sf::Color::White);
                text.setPosition(sf::Vector2f(window.getSize().x*7.f/10.f, window.getSize().y/10 + espacio));
                window.draw(text);
                espacio += 13; */
            }
            // Si es un poligono
            else
            {
                // Agregamos las coordenadas a la figura compleja
                sf::ConvexShape convex;
                string polygon_coords;
                convex.setPointCount(eh->objeto.size());
                for (int j = 0; j < eh->objeto.size(); j++)
                {
                    convex.setPoint(j, sf::Vector2f(eh->objeto[j].x, window.getSize().y - eh->objeto[j].y));
                    polygon_coords = polygon_coords + tupla_string(eh->objeto[j]) + '\t';
                }
                convex.setFillColor(sf::Color::Blue);
                window.draw(convex);

                /* // sdfsfs
                sf::Text text;
                text.setFont(font);
                text.setString("Poligono: "+polygon_coords);
                text.setCharacterSize(10);
                text.setFillColor(sf::Color::White);
                text.setPosition(sf::Vector2f(window.getSize().x*7.f/10.f, window.getSize().y/10 + espacio));
                window.draw(text);
                espacio += 13; */
            }
        }
    }
    // SI es un nodo interno
    else
    {
        // Pasamos por las entradas
        for (auto i : nodo->entradas)
        {
            // Obtenemos las distancias del MBR
            int dx = i->rectangulo[0].mayor - i->rectangulo[0].menor;
            int dy = i->rectangulo[1].mayor - i->rectangulo[1].menor;
            // Cremaos un rectangulo con las distancias
            sf::RectangleShape rectangulo(sf::Vector2f(dx, dy));
            rectangulo.setOutlineColor(sf::Color((40-color)%256, (2*color  + 10)%256, color%256));
            rectangulo.setOutlineThickness(2);
            // Posicionamos el rectangulo de acuerdo a su posición superior izquierda
            rectangulo.setPosition(i->rectangulo[0].menor, window.getSize().y - i->rectangulo[1].mayor);
            rectangulo.setFillColor(sf::Color::Transparent);

            window.draw(rectangulo);
            // llamamos a la misma funión para al nodo hijo de la entrada interna
            imprimir_arbol_r_recursivo(i->puntero_hijo, espacio, color+30);
        }
    }
}

// FUNCION DEPRECADA
string Interfaz::tupla_string(Punto llave_tupla){
    return "("+to_string(llave_tupla.x)+", "+to_string(llave_tupla.y)+")";
}