#pragma once

#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>

#include "struct.h"
#include "Exception.h"


namespace shot {


/**
* Модель движения предмета, брошеного под углом к горизонту.
*/
class Shot {
public:
    /**
    * Конструктор может принимать параметры любого типа без выброса
    * предупреждений компилятором.
    */
    template< typename T1, typename T2, typename T3 >
    Shot(
        T1 velocity,
        T2 angle,
        T3 g
    );




    virtual ~Shot();




    /**
    * @return Модель, параметры которой заданы строкой символов.
    *
    * # Ускорение свободного падения всегда направлено вертикально вниз,
    *   вне зависимости от задаваемого в строке знака. Если не указано,
    *   принимается равным 9.81 м/с^2.
    */
    static std::shared_ptr< shot::Shot >  valueOf( const std::string& );




    /**
    * Переводит модель в след. состояние.
    * Рассчитывает, где окажется предмет по прошествии заданного времени.
    * 
    * @param timestep  Время, с.
    */
    void pulse( real_t timestep );




#if 0
    @todo Без использования сторонних библиотек или VC11 реализация
          потоков повлечёт за собой десятки строк низкоуровневого кода.
          Отложено.
    /**
    * То же, что pulse(), но сразу возвращает управление (запускается в
    * потоке).
    */
    ... pulseThread( real_t timestep );
#endif




    /**
    * @return Текущее состояние системы.
    *
    * @see pulse()
    */
    inline coord_t const& coord() const {
        return mCoord;
    }


    inline vector_t const& velocity() const {
        return mVelocity;
    }


    inline real_t velocityAbs() const {
        return std::sqrt(
            mVelocity.x * mVelocity.x + mVelocity.y * mVelocity.y
        );
    }


    inline real_t angle() const {
        return mAngle;
    }


    inline real_t gravity() const {
        return mGravity;
    }




    /**
    * @return Тело находится внутри заданной области.
    *
    * Область задаётся шириной и высотой.
    * Координаты левого нижнего угла
    *   ( -width / 2,  -height / 2 )
    * Координаты правого верхнего угла
    *   ( width / 2,  height / 2 )
    * Координаты центра
    *   ( 0, 0 )
    */
    bool inner( real_t width, real_t height ) const;




private:
    const real_t mAngle;
    const real_t mGravity;

    coord_t  mCoord;
    vector_t mVelocity;
};


} // shot








#include "Shot.inl"
