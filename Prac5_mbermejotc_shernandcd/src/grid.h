//
// Created by salabeta on 24/11/20.
//

#ifndef GOTOXY_GRID_H
#define GOTOXY_GRID_H

#include <QGraphicsItem>

template<typename HMIN, HMIN hmin, typename WIDTH, WIDTH width, typename TILE, TILE tile>
class Grid
{
    public:
        Grid()
        {
            array.resize((int)(width/tile));
            for (auto &row : array)
                row.resize((int)(width/tile));
            int k=0;
            for (int i = hmin; i < width/2; i += tile, k++)
            {
                int l=0;
                for (int j = hmin; j < width/2; j += tile, l++)
                {
                    array[k][l] = Value{false, nullptr, nullptr,i, j,k,l, -1};
                }
            }

        };

        struct Value
        {
            bool occupied = false;
            QGraphicsRectItem * paint_cell = nullptr;
            QGraphicsTextItem * text_cell = nullptr;
            int cx, cy;
            int k,l;
            int dist; //dist vecinos
        };

        std::vector<std::vector<Value>> array;

        void create_graphic_items(QGraphicsScene &scene)
        {
            auto fondo = QColor("LightGreen"); fondo.setAlpha(40);
            QFont font("Bavaria");
            font.setPointSize(50);
            font.setWeight(QFont::TypeWriter);
            for (auto &row : array)
                for (auto &elem : row)
                {
                    elem.paint_cell = scene.addRect(-tile / 2, -tile / 2, tile, tile, QPen(QColor("DarkGreen")), QBrush(fondo));
                    elem.paint_cell->setPos(elem.cx, elem.cy);
                    elem.text_cell = scene.addText("-1", font);
                    elem.text_cell->setPos(elem.cx-tile/2, elem.cy-tile/2);
                    // Get the current transform
                    QTransform transform(elem.text_cell->transform());
                    qreal m11 = transform.m11();    // Horizontal scaling
                    qreal m12 = transform.m12();    // Vertical shearing
                    qreal m13 = transform.m13();    // Horizontal Projection
                    qreal m21 = transform.m21();    // Horizontal shearing
                    qreal m22 = transform.m22();    // vertical scaling
                    qreal m23 = transform.m23();    // Vertical Projection
                    qreal m31 = transform.m31();    // Horizontal Position (DX)
                    qreal m32 = transform.m32();    // Vertical Position (DY)
                    qreal m33 = transform.m33();    // Addtional Projection Factor
                    // Vertical flip
                    m22 = -m22;
                    // Write back to the matrix
                    transform.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);
                    // Set the items transformation
                    elem.text_cell->setTransform(transform);
                }

        }
    /*
 * Inicializamos el array a false, osea, no ocupades.
 */
    void inicializate()
    {
        for (int i = 0; i < this->tam; ++i) {
            for (int j = 0; j < this->tam; ++j) {
                this->array[i][j] = false;
            }
        }
    }

public:

    /**
     * modificamos en funcion de v la coordenada x,z
     * @param x
     * @param z
     * @param v
     */
    void set_Value(int x, int z, bool v)
    {
       auto [i, j] = transformar(x,z);

       std::cout << i<< "  " <<j<<endl;
       array[i][j].occupied = v;
       if(v) {
           array[i][j].paint_cell->setBrush(QColor("Red"));
           array[i][j].dist = -1;
       }

    }
    /**
     * devolvemos el valor de la coordenada x,z
     * @param x
     * @param z
     * @return
     */
    Value get_value(int x, int z)
    {
        auto [i, j] = transformar(x,z);
        return  this->array[i][j];
    }


    std::tuple<int, int> transformar(int x, int y){
        int i = x/tile + (width/tile)/2;
        int k = y/tile + (width/tile)/2;

        return std::make_tuple(i, k);
        
    }

    bool is_in_limits(int k, int l){
        bool is = false;

        if (k >= 0 and k < 50 and l >= 0 and l < 50 )
            is = true;

        return is;
    }

    std::vector<Value> vecinos(Value v, int dist){
        std::vector<std::tuple<int, int>> lista_coor_de_vecinos{ {-1,-1}, {0,-1}, {1,-1},{-1,0},
                                                                 {1,0}, {-1,1}, {0,1}, {-1,1} };
        std::vector<Value> listA;
        for(auto [dk, dl] : lista_coor_de_vecinos)
        {
            int k = v.k + dk;        
            int l = v.l + dl;

            if(is_in_limits(k,l) and not this->array[k][l].occupied and this->array[k][l].dist == -1)
            {
                this->array[k][l].dist = dist;
                this->array[k][l].text_cell->setPlainText(QString::number(dist));
                listA.insert(listA.end(),this->array[k][l]);
            }
        }


        return listA;

    }
    void reset_cell_distances() {
        for (auto &row : array) {
            for(auto &elem : row){
                elem.dist = -1;
            }
        }
    }


};



#endif //GOTOXY_GRID_H
