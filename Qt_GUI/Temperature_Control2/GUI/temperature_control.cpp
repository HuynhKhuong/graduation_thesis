#include "temperature_control.h"
#include <QDebug>
#include <QString>
Temperature_Control::Temperature_Control()
{
    this->D = 0;
    this->P = 0;
    this->I = 0;
    this->SP = 0;
    this->temp = 0;
    this->SP_speed = 0;
    this->cur_speed = 0;
}

QByteArray Temperature_Control::Protocol_transmitt()
{
   QByteArray data_transmitt;
   //Header
   data_transmitt.append('(');
   data_transmitt.append('{');
   //Data length (leave blank)
   data_transmitt.append('.');
   switch(this->Command)
   {
        case(STOP):
        case(SET_FREQ):
        {
           data_transmitt.append('S');
           data_transmitt.append(':');
           data_transmitt.append((char)(((qint16)(this->SP_speed*100) >> 8)& 0xFF));
           data_transmitt.append((char)((qint16)(this->SP_speed*100 )& 0xFF));
           break;
        }
        case(SET_P_Params):
        {
            data_transmitt.append('P');
            data_transmitt.append(':');
            data_transmitt.append((char)(((qint16)(this->P*100) >> 8)& 0xFF));
            data_transmitt.append((char)((qint16)(this->P*100)& 0xFF));
            break;
        }
       case(SET_I_Params):
       {
           data_transmitt.append('I');
           data_transmitt.append(':');
           data_transmitt.append((char)(((qint16)(this->I*100) >> 8)& 0xFF));
           data_transmitt.append((char)((qint16)(this->I*100)& 0xFF));
           break;
       }
       case(SET_D_Params):
       {
           data_transmitt.append('D');
           data_transmitt.append(':');
           data_transmitt.append((char)(((qint16)(this->D*100) >> 8)& 0xFF));
           data_transmitt.append((char)((qint16)(this->D*100)& 0xFF));
           break;
       }
       case(SET_TEMP):
       {
           data_transmitt.append('T');
           data_transmitt.append(':');
           data_transmitt.append((char)(((qint16)(this->SP*100) >> 8)& 0xFF));
           data_transmitt.append((char)((qint16)(this->SP*100)& 0xFF));
           break;
       }
       case(PID_TOGGLE):
       {
           data_transmitt.append('R');
           data_transmitt.append(':');
           data_transmitt.append((char)(((qint16)(this->SP*100) >> 8)& 0xFF)); //Unused values
           data_transmitt.append((char)((qint16)(this->SP*100)& 0xFF));
           break;
       }
       case(BED_HEAT):
       {
           data_transmitt.append('B');
           data_transmitt.append(':');
           data_transmitt.append((char)(((qint16)(this->SP*100) >> 8)& 0xFF)); //Unused values
           data_transmitt.append((char)((qint16)(this->SP*100)& 0xFF));
           break;
       }
       case(LOCK):
       {
           data_transmitt.append('L');
           data_transmitt.append(':');
           data_transmitt.append((char)(((qint16)(this->SP*100) >> 8)& 0xFF)); //Unused values
           data_transmitt.append((char)((qint16)(this->SP*100)& 0xFF));
           break;
       }
   }

   data_transmitt.append('}');
   data_transmitt.append(')');
   data_transmitt[2] = char(data_transmitt.length() - 5);
   return data_transmitt;
}

void Temperature_Control::received_response(QByteArray received_data)
{
    //remove header

    received_data.remove(0,2);
    received_data.remove(received_data.length()-2,2);

    //get  Status

    switch(received_data[0])
    {
        case 0:
        {
            this->Response = HARDWARE_ERROR;
            break;
        }
        case 1:
        {
            this->Response = COMM_ERROR;
            break;
        }
        case 2:
        {
            this->Response = COMM_SUCCESS ;
            break;
        }
        case 3:
        {
            this->Response = SET_ERROR;
            break;
        }
        case 4:
        {
            this->Response = GET_ERROR;
            break;
        }
        case 5:
        {
            this->Response = SET_SUCCESS ;
            break;
        }
        case 6:
        {
            this->Response = GET_SUCCESS;
            break;
        }
        default:    this->Response = NON_RECEIVE;
    };

    //get temperature
    if(received_data[received_data.indexOf('T')+1] != 'x') //if temperature can't be read, keep old value
    {
        //QString b;
        uint16_t buffer = (uint16_t)((uint8_t)received_data[received_data.indexOf('T')+1]);
        buffer = (buffer << 8) | (uint16_t)((uint8_t)received_data[received_data.indexOf('T')+2]);
        //qDebug() << (buffer);
        this->temp = (float)(buffer)/100;
    }
    else this->temp = this->get_temp();

    //get speed
    switch(this->Response) //if speed can't be read, keep old value
    {
        case SET_SUCCESS:
        {
            uint16_t buffer = (uint16_t)((uint8_t)received_data[received_data.indexOf('S')+1]);
            buffer = (buffer << 8) | (uint16_t)((uint8_t)received_data[received_data.indexOf('S')+2]);
            this->cur_speed = (float)(buffer)/100;
            break;
        }

        case GET_SUCCESS:
        {

            uint16_t buffer = (uint16_t)((uint8_t)received_data[received_data.indexOf('S')+1]);
            buffer = (buffer << 8) |(uint16_t)((uint8_t)received_data[received_data.indexOf('S')+2]);

            this->cur_speed = (float)(buffer)/100;
            break;
        }
        default:
        {
            this->cur_speed = this->get_curspeed();
        }
    }

}
//get methods
float Temperature_Control::get_temp()
{
    return this->temp;
}
float Temperature_Control::get_SP()
{
    return this->SP;
}
float Temperature_Control::get_P()
{
    return this->P;
}
float Temperature_Control::get_I()
{
    return this->I;
}
float Temperature_Control::get_D()
{
    return this->D;
}
float   Temperature_Control::get_speed()
{
    return this->SP_speed;
}

float Temperature_Control::get_curspeed()
{
    return this->cur_speed;
}

//set methods
void Temperature_Control::set_temp(float value)
{
    this->temp = value;
}
void Temperature_Control::set_SP(float value)
{
    this->SP = value;
}
void Temperature_Control::set_P(float value)
{
    this->P = value;
}
void Temperature_Control::set_I(float value)
{
    this->I = value;
}
void Temperature_Control::set_D(float value)
{
    this->D = value;
}
void  Temperature_Control::set_speed(float speed)
{
    this->SP_speed = speed;
}
void  Temperature_Control::set_curspeed(float speed)
{
    this->cur_speed = speed;
}
