#ifndef TEMPERATURE_CONTROL_H
#define TEMPERATURE_CONTROL_H

#include <QByteArray>

typedef enum
{
    SET_FREQ 	 = 0,
    STOP		 = 1,
    SET_P_Params = 2,
    SET_I_Params = 3,
    SET_D_Params = 4,
    SET_TEMP     = 5,
    IDLE         = 6,
    PID_TOGGLE   = 7,
    BED_HEAT     = 8,
    LOCK         = 9,
}Cmd_typedef;

typedef enum
{
    HARDWARE_ERROR = 0,
    COMM_ERROR     = 1,
    COMM_SUCCESS   = 2,
    SET_ERROR      = 3,
    GET_ERROR      = 4,
    SET_SUCCESS    = 5,
    GET_SUCCESS    = 6,
    NON_RECEIVE,
}Status_typedef;

class Temperature_Control
{
public:
    Cmd_typedef Command;
    Status_typedef Response;

    Temperature_Control();
    QByteArray Protocol_transmitt();
    void received_response(QByteArray received_data);

    //get methods
    float get_temp();
    float get_SP();
    float get_P();
    float get_I();
    float get_D();
    float get_speed();
    float get_curspeed();

    //set methods
    void set_temp(float value);
    void set_SP(float value);
    void set_P(float value);
    void set_I(float value);
    void set_D(float value);
    void set_speed(float speed);
    void set_curspeed(float speed);

private:
    float temp;
    float SP;
    float P;
    float I;
    float D;
    float SP_speed;
    float cur_speed;

};

#endif // TEMPERATURE_CONTROL_H
