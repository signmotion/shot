#pragma once

#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>

#include "struct.h"
#include "Exception.h"


namespace shot {


/**
* ������ �������� ��������, ��������� ��� ����� � ���������.
*/
class Shot {
public:
    /**
    * ����������� ����� ��������� ��������� ������ ���� ��� �������
    * �������������� ������������.
    */
    template< typename T1, typename T2, typename T3 >
    Shot(
        T1 velocity,
        T2 angle,
        T3 g
    );




    virtual ~Shot();




    /**
    * @return ������, ��������� ������� ������ ������� ��������.
    *
    * # ��������� ���������� ������� ������ ���������� ����������� ����,
    *   ��� ����������� �� ����������� � ������ �����. ���� �� �������,
    *   ����������� ������ 9.81 �/�^2.
    */
    static std::shared_ptr< shot::Shot >  valueOf( const std::string& );




    /**
    * ��������� ������ � ����. ���������.
    * ������������, ��� �������� ������� �� ���������� ��������� �������.
    * 
    * @param timestep  �����, �.
    */
    void pulse( real_t timestep );




#if 0
    @todo ��� ������������� ��������� ��������� ��� VC11 ����������
          ������� �������� �� ����� ������� ����� ��������������� ����.
          ��������.
    /**
    * �� ��, ��� pulse(), �� ����� ���������� ���������� (����������� �
    * ������).
    */
    ... pulseThread( real_t timestep );
#endif




    /**
    * @return ������� ��������� �������.
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
    * @return ���� ��������� ������ �������� �������.
    *
    * ������� ������� ������� � �������.
    * ���������� ������ ������� ����
    *   ( -width / 2,  -height / 2 )
    * ���������� ������� �������� ����
    *   ( width / 2,  height / 2 )
    * ���������� ������
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
