/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials 
*/
#include "simpletools.h"                      // Include simple tools
#include "servo.h"

unsigned int stack1[40 + 25]; // Stack vars for cog1
unsigned int stack2[40 + 25]; // Stack vars for cog2
unsigned int stack3[40 + 25]; // Stack vars for cog3
unsigned int stack4[40 + 25]; // Stack vars for cog4
unsigned int stack5[40 + 25]; // Stack vars for cog5

static volatile int ir_left, ir_right,ir_center;
static volatile int us_left, us_right,us_center;
static volatile int actuator;
int obstacle=0;

int servo_left=16, servo_right=15;
int tracker_x=0;
int tracker_y=1;

//forward declarations
void irsensorLeft(void *par1);
void irsensorRight(void *par2);
void irsensorCenter(void *par3);
void objDetector(void *par3);
void ledControl(void *par5);

void forwardDrive();
void stopDrive();
void rightDriveMore();
void rightDriveLess();
void leftDriveMore();
void leftDriveLess();
void right90();
void left90();

int Ping(int tPin, int ePin);
int Ping_cm(int tPin, int ePin);
                         
void driveBot();
//forward declaration ends
int main()                                    // Main function
{
  us_center=us_right=200;
  actuator=0;
  
  int cog1= cogstart(&irsensorLeft, NULL, stack1, sizeof(stack1));
  int cog2= cogstart(&irsensorRight, NULL, stack2, sizeof(stack2));
  int cog3= cogstart(&irsensorCenter, NULL, stack3, sizeof(stack3));   
  int cog4= cogstart(&objDetector, NULL, stack4, sizeof(stack4));
  int cog5= cogstart(&ledControl, NULL, stack5, sizeof(stack5));
  
  simpleterm_open(); 
  print("Dist = ");
  while(us_center>6)
  {
    driveBot();  
  } 
  right90();right90(); 
  while(1)
  {
     driveBot();  
  }    
  stopDrive();
  simpleterm_close();
}
/* Cog action are declared in this block
 *  --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
// *cog 1 for ir left sensors *******************************************************************************************************************************************
void irsensorLeft(void *par1)
{  
  set_direction(2,0);
 //  simpleterm_open();
  while(1)
  {
    ir_left=!get_state(2);
    pause(20);
    //print("%d\n",ir_left);
    //pause(20); 
  }
//  simpleterm_close();
}  
// *cog 2 for ir right sensors *******************************************************************************************************************************************
void irsensorRight(void *par2)
{  
  set_direction(3,0);
 //  simpleterm_open();
  while(1)
  {
    ir_right=!get_state(3);
    pause(20);
    //print("%d\n",ir_left);
    //pause(20); 
  }
//  simpleterm_close();
}  
// *cog 3 for ir left sensors *******************************************************************************************************************************************
void irsensorCenter(void *par3)
{  
  set_direction(4,0);
 //  simpleterm_open();
  while(1)
  {
    ir_center=!get_state(4);
    pause(20);
    //print("%d\n",ir_left);
    //pause(20); 
  }
//  simpleterm_close();
} 
// *cog 4 for ir left sensors *******************************************************************************************************************************************
void objDetector(void *par4)
{  
  set_direction(8,0);//echo pin center
  set_direction(9,1);//trig pin center
  
  set_direction(6,0);//echo pin right
  set_direction(7,1);//trig pin right
  while(1)
  {
    us_center=Ping_cm(9,8);
    pause(20); 
    us_right=Ping_cm(9,8);
    pause(20);
  }
}    

// *cog 5 for control of actuator ****************************************************************************************************************************************
 void ledControl(void *par5)
 {
   set_direction(5,1);//intersection led
   while(1)
   {
     switch(actuator)
     {
       case 1:high(5);
       pause(300);
       low(5);
       actuator=0;
       break;
     }       
   }     
   
 }   
    
/* End of Cog block
 * -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
// ALL the function from this point
// HIGH LEVEL ABSTRACTION BLOCK ***********************************************************************************************************************************
void driveBot()
{ 
    if (ir_left == 1 && ir_center == 0 && ir_right == 1) 
    {
      forwardDrive();
    } 
    else if (ir_left == 0 && ir_center == 0 && ir_right == 0) //intersection detected
    {
      actuator=1;
      forwardOverDrive();
      tracker_x+=1;
      //stopDrive();
      //pause(500);
    }
    else if (ir_left == 1 && ir_center == 1 && ir_right == 1) //stop condition
    {
       forwardDrive();
    } 
    else if (ir_left == 0  && ir_right == 1 && ir_center==0)  //driving sequence
    {
      leftDriveLess();
    } 
    else if (ir_left == 1&& ir_right == 0 && ir_center==0) 
    {
      rightDriveLess();
    } 
    else if (ir_left == 0  && ir_right == 1 && ir_center==1)  //driving sequence
    {
      leftDriveMore();
    } 
    else if (ir_left == 1&& ir_right == 0 && ir_center==1) 
    {
      rightDriveMore();
    } 
}
// * -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
// ALL the function from this point
// LOW LEVEL ABSTRACTION BLOCK ***********************************************************************************************************************************
  
void forwardDrive()
{
  servo_angle(servo_left , 950);
  servo_angle(servo_right , 850);
  pause( 20);
}  
void forwardOverDrive()
{
  while(!(ir_left || ir_right))
  {
    forwardDrive();
    pause( 20);
  }    
}  
void stopDrive()
{
  servo_angle(servo_left , 900);
  servo_angle(servo_right , 900);
  pause( 20);
}  
void rightDriveMore()
{
  servo_angle(servo_left , 950);
  servo_angle(servo_right , 890);
  pause( 20);
}  
void leftDriveMore()
{
  servo_angle(servo_left , 910);
  servo_angle(servo_right , 850);
  pause( 20);
}  
void rightDriveLess()
{
  servo_angle(servo_left , 950);
  servo_angle(servo_right , 870);
  pause( 20);
}  
void leftDriveLess()
{
  servo_angle(servo_left , 930);
  servo_angle(servo_right , 850);
  pause( 20);
}  
void right90()
{
  servo_angle(servo_left , 950);
  servo_angle(servo_right , 950);
  pause( 750);
  stopDrive();
} 
void left90()
{
  servo_angle(servo_left , 850);
  servo_angle(servo_right , 850);
  pause( 750);
  stopDrive();
} 

int Ping(int tPin, int ePin)
{
  low(tPin);  
  pulse_out(tPin, 10);
  return pulse_in(ePin, 1);
}

int Ping_cm(int tPin, int ePin)
{
  long tEcho = Ping(tPin, ePin);
  int cmDist = tEcho / 58;
  return cmDist;
}