#include <Boards.h>
#include <Firmata.h>

/*
 * rosserial Publisher Example
 * Prints "hello world!"
 */

#include <ros.h>
#include <ros/time.h>
#include <analog_read/analog_array.h>
#include <std_msgs/Bool.h>
//#include <analog_read/analog_fields.h>

#define N_AN_PINS 14

float y0[N_AN_PINS]; //= {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
float y1[N_AN_PINS]; //= {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
float y2[N_AN_PINS]; //= {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
float y3[N_AN_PINS]; //= {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
float y4[N_AN_PINS]; //= {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
float y5[N_AN_PINS]; //= {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

int x0[N_AN_PINS]; //= {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int x1[N_AN_PINS]; //= {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int x2[N_AN_PINS]; //= {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int x3[N_AN_PINS]; //= {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int x4[N_AN_PINS]; //= {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int x5[N_AN_PINS]; //= {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

#define MOSFT_PINS 14

ros::NodeHandle  nh;

void messageCb( const std_msgs::Bool& toggle_msg){
  if (toggle_msg.data == true){
    for (int i=2; i<MOSFT_PINS; i++)
      digitalWrite(i, HIGH);   // blink the led
  }else{
    for (int i=2; i<MOSFT_PINS; i++)
      digitalWrite(i, LOW);   // blink the led
  }
}

ros::Subscriber<std_msgs::Bool> sub("mosfet_status", &messageCb );

analog_read::analog_array aaff;
ros::Publisher a_reads("a_reads", &aaff);
uint16_t ar[N_AN_PINS];

void setup()
{
  analogReference(EXTERNAL);
  for (int i=2; i<MOSFT_PINS; i++)
    pinMode(i, OUTPUT);
  for (int i=0; i<N_AN_PINS; i++)
    y0[i] = y1[i] = y2[i] = y3[i] = y4[i] = x0[i] = x1[i] = x2[i] = x3[i] = x4[i] = 1;
  nh.initNode();
  aaff.an_read_length = N_AN_PINS;
  aaff.header.frame_id = "/a_reads";
  aaff.an_read = ar;
  nh.advertise(a_reads);
  nh.subscribe(sub);
  delay(10);
}

float averageAnalog(int pin){
  /*
  int v=0;
  for(int i=0; i<4; i++) v+= analogRead(pin);
  return v/4;
  */
// kind of works

  x4[pin] = analogRead(pin);
  y4[pin] = 0.001*(0.0582*x4[pin] + 0.2327*x3[pin] + 0.3490*x2[pin] + 0.2327*x1[pin] + 0.0582*x0[pin]) + (3.5168*y3[pin]) - (4.6637*y2[pin]) + (2.7621*y1[pin]) - (0.6161*y0[pin]);
  x0[pin]=x1[pin];
  x1[pin]=x2[pin];
  x2[pin]=x3[pin];
  x3[pin]=x4[pin];
 
  y0[pin]=y1[pin];
  y1[pin]=y2[pin];
  y2[pin]=y3[pin];
  y3[pin]=y4[pin];

  return y4[pin];
  /*
  x5[pin] = analogRead(pin);

  y5[pin] = 0.001*(0.0909*x5[pin] + 0.4546*x4[pin] + 0.9093*x3[pin] + 0.9093*x2[pin] + 0.4546*x1[pin] + 0.0909*x0[pin]) - 100000*(-4.8983*y4[pin] + 9.5985*y3[pin] -9.4053*y2[pin] + 4.6085*y1[pin] -0.9033*y0[pin]);

  x0[pin]=x1[pin];
  x1[pin]=x2[pin];
  x2[pin]=x3[pin];
  x3[pin]=x4[pin];
  x4[pin]=x5[pin];
 
  y0[pin]=y1[pin];
  y1[pin]=y2[pin];
  y2[pin]=y3[pin];
  y3[pin]=y4[pin];
  y4[pin]=y5[pin];

  return y5[pin];
*/
}

void loop()
{  
  aaff.an_read = ar;
  for(int i=0; i<aaff.an_read_length; i++){
    //ar[i] = (int)averageAnalog(i);
    ar[i] = analogRead(i);
  }
  aaff.header.stamp = nh.now();
  a_reads.publish( &aaff );
  nh.spinOnce();
  //delay(10);
}
