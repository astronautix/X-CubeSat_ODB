
#include "system/qb50.h"
#include "STRUCTURE.h"
#include "ADCSCLASS.h"

#include <safe_stm32f4xx.h>

using namespace qb50;



using namespace std;
adcs::adcs( )
{
M_control_attitude.typ=T_control_attitude;
M_PING.typ=T_PING;
M_GPS.typ=T_GPS;
M_TEM.typ=T_TEMPS;
M_MES.typ=T_MESURE;
M_QUAT.typ=T_QUAT;
M_ROT.typ=T_ROT;
M_POS.typ=T_POS;
M_STOP.typ=T_STOP;}

void adcs::setADCSOn()
{
    //setADCSOn();
}

void adcs::InitADCS()
{
//InitADCS();
}


message adcs::readADCSmsg()
{
message  msg1;
//msg1.typ=liremsgtyp();
  msg1.typ=T_PING;
  msg1.M[0]=1;
  msg1.M[1]=22;
  msg1.M[2]=33;
  msg1.M[3]=44;
//msg1=readADCSmsg;



switch (msg1.typ)
{
 case T_QUAT:{
                       for(int i=0;i<=3;i++)  msg1.quat.Q[i]=msg1.M[i];

 }break;
 case T_ROT:{
                       for (int i=0;i<=2;i++) msg1.rot.R[i]=msg1.M[i];
 }break;
 case    T_POS:{
                        for (int i=0;i<=2;i++) msg1.pos.P[i]=msg1.M[i];
 }break;
 case T_PING:{
                        msg1.pin.p=msg1.M[0];
 }break;





 default : break;
};
return msg1;
    };

void adcs::writeADCSMessage(message a)
{switch (a.typ)
{default : break;

    /*
case 0 :cout<<"asking for quaternions"<<endl;break;//appelle a la fct write adcs msg
case 1:cout<<" asking for rotations"<<endl;break;
case 2:cout<<"sending GPS parameters"<<endl;break;
case 3 : cout<<"asking for positions"<<endl;break;
case 4: cout<<"activation of detumbling"<<endl;break;
case 5:cout<<"sending ping"<<endl;break;
case 6:  cout<<"sending time"<<endl;break;
case 7: cout<<"activation of mesure mode"<<endl;break;
case 8: cout<<"desactivation of adcs"<<endl;break;
*/
   }; };

void adcs::WriteExtMemorie(signed int A,signed int *B)
{}
;
int adcs::ReadExtMemorieint(signed int *D)
{return 0;
};

void adcs::setADCSOff()
{//setADCSOff();
}



