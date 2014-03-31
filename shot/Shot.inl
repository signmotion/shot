namespace shot {


template< typename T1, typename T2, typename T3 >
inline Shot::Shot(
    T1 velocity,
    T2 angle,
    T3 g
) :
    mAngle( static_cast< real_t >( angle ) ),
    mGravity( static_cast< real_t >( g ) )
{
    // # Движение начинаем с координатами (0, 0).
    static const coord_t COORD_ZERO = {};
    mCoord = COORD_ZERO;

    // # Сила гравитация направлена вниз.
    const auto v = static_cast< real_t >( velocity );
    const auto a = mAngle * M_PI / 180.0;
    mVelocity.x = static_cast< real_t >( v * cos( a ) );
    mVelocity.y = static_cast< real_t >( v * sin( a ) );
}




inline Shot::~Shot() {
}




inline std::shared_ptr< shot::Shot >
Shot::valueOf( const std::string& line ) {

    // распознаём строку
    real_t velocity = 0.0f;
    real_t angle = 0.0f;
    real_t g = -9.81f;
    std::istringstream ss( line, std::istringstream::in );
    std::string word;
    for (size_t i = 0; ss >> word; ++i) {
        std::istringstream wss( word );
        switch ( i ) {
            // скорость
            case 0:
                wss >> velocity;
                if ( wss.fail() ) {
                    throw Exception( "Velocity is not recognized." );
                }
                break;

            // угол
            case 1:
                wss >> angle;
                if ( wss.fail() ) {
                    throw Exception( "Angle is not recognized." );
                }
                if ( (angle > 90) || (angle < 0) ) {
                    throw Exception( "Angle must have diapason [0; 90]." );
                }
                break;

            // ускорение своб. падения
            case 2:
                wss >> g;
                // # Ускорение всегда отрицательное (направлено вниз).
                if (g > 0.0f) {
                    g = -g;
                }
                if ( wss.fail() ) {
                    throw Exception( "Gravitational acceleration is not recognized." );
                }
                break;

            default:
                throw Exception( "Too many parameters in command line." );
        };
    };


    // создаём модель
    return std::make_shared< Shot >( velocity, angle, g );
}




inline void Shot::pulse( real_t timestep ) {
    mVelocity.y += mGravity * timestep;
    mCoord.x += mVelocity.x * timestep;
    mCoord.y += mVelocity.y * timestep;
}




inline bool Shot::inner( real_t width, real_t height ) const {
    // правый верхний угол
    const auto bx = static_cast< real_t >( width  ) / 2.0f;
    const auto by = static_cast< real_t >( height ) / 2.0f;
    // левый нижний угол
    const auto ax = -bx;
    const auto ay = -by;

    return (mCoord.x > ax)
        && (mCoord.x < bx)
        && (mCoord.y > ay)
        && (mCoord.y < by);
}


} // shot
