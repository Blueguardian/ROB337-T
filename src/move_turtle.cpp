#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/TeleportAbsolute.h>
#include <turtlesim/SetPen.h>
#include <iostream>
#include "stdlib.h"
#include "time.h"
#include "string.h"

//WOWOWOWOWOWO CHANGES ARE MADE NOW!!!

double euDist (double x1, double y1){
  double centerx = 5.544445;
  double centery = 5.544445;
  double x = centerx - x1;
  double y = centery - y1;
  double dist = pow(x, 2)+pow(y, 2);

  dist = sqrt(dist);
  return dist;
}

bool compDist (double ar1[2], double ar2[2]){
  double ar1x = euDist(ar1[0], ar1[1]);
  double ar2x = euDist(ar2[1], ar2[1]);
  return ar1x < ar2x;
}

void initTurtlePosition(double x, double y, int randomNumber1, int randomNumber2, int randomNumber3, ros::ServiceClient teleport_client, ros::ServiceClient pen_client, turtlesim::TeleportAbsolute srv, turtlesim::SetPen pen_srv){
  pen_srv.request.off = true;
  pen_client.call(pen_srv);
  srv.request.x = x;
  srv.request.y = y;
  teleport_client.call(srv);
  pen_srv.request.off = false;
  pen_srv.request.width = 10;
  pen_srv.request.r = randomNumber1;
  pen_srv.request.g = randomNumber2;
  pen_srv.request.b = randomNumber3;
  pen_client.call(pen_srv);
}

void moveTurtle(double x, double y, ros::ServiceClient teleport_client, turtlesim::TeleportAbsolute srv){
  srv.request.x = x;
  srv.request.y = y;
  teleport_client.call(srv);
}

void fillArray (double (&anArray)[10][2]) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (int i=0;i<10; i++)
    {
        double a = 3.0 + rand() / ((RAND_MAX/(10.0-3.0)));
        double b = 3.0 + rand() / ((RAND_MAX/(10.0-3.0)));
        double& a_ref = a;
        double& b_ref = b;
        anArray[i][0]=a_ref;
        anArray[i][1]=b_ref;
    }
    double a = anArray[0][0];
    double b = anArray[0][1];
    anArray[9][0]=a;
    anArray[9][1]=b;
}

void sortArray (double (&anArray)[10][2]) {
int i;
double temp; 
int flag = 0, length = 10;
int d = length;
while(flag || (d < 1)){
  flag = 0;
  d = (d+1)/2;
  for (i = 0; i<(length - d); i++){
    if(compDist(anArray[i+d], anArray[i])){
      temp = anArray[i+d][0];
      anArray[i+d][0] = anArray[i][0];
      anArray[i][0] = temp;

      flag = 1;
    }
  }
}


  double a = anArray[0][0];
  double b = anArray[0][1];
  anArray[9][0]=a;
  anArray[9][1]=b;
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  int randomNumber1 = 0;
  int randomNumber2 = 0;
  int randomNumber3 = 0;

  randomNumber1 = rand() %100+1;
  randomNumber2 = rand() %100+2;
  randomNumber3 = rand() %100+3;
  randomNumber1 *= 2;
  randomNumber2 *= 2;
  randomNumber3 *= 2;

  ros::init(argc, argv, "draw_turtle");
  ros::NodeHandle nh;
  ros::service::waitForService("/turtle1/teleport_absolute", -1);
  ros::ServiceClient teleport_client = nh.serviceClient<turtlesim::TeleportAbsolute>("/turtle1/teleport_absolute");
  turtlesim::TeleportAbsolute srv;
  ros::ServiceClient pen_client = nh.serviceClient<turtlesim::SetPen>("/turtle1/set_pen");
  turtlesim::SetPen pen_srv;
  ros::Rate loop_rate(2);

  float startX = 5.544445, startY = 5.544445;
  if(argc>1){
    startX = atof(argv[1]);
    startY = atof(argv[2]);
  }

  initTurtlePosition(startX, startY, randomNumber1, randomNumber2, randomNumber3, teleport_client, pen_client, srv, pen_srv);
  loop_rate.sleep();
  double coordinates[10][2];

  fillArray(coordinates);
  sortArray(coordinates);
  for(int i = 0; i<10; i++) {
    for(int j = 0; j<2; j++){
      std::cout << std::to_string(coordinates[i][j]) + "\n";
    }
    
  }

  for(int i=0; i<10; i++){
    if(i<1){
      pen_srv.request.off = true;
      pen_client.call(pen_srv);
      moveTurtle(coordinates[i][0], coordinates[i][1], teleport_client, srv);
      loop_rate.sleep();
    }
    else{
      pen_srv.request.off = false;
      pen_srv.request.r = randomNumber1;
    pen_srv.request.g = randomNumber2;
    pen_srv.request.b = randomNumber3;
      pen_client.call(pen_srv);
      moveTurtle(coordinates[i][0], coordinates[i][1], teleport_client, srv);
      loop_rate.sleep();
    }
    
  }


/* Exercise 2
  pen_srv.request.off = true;
  pen_client.call(pen_srv);
  srv.request.x = startX;
  srv.request.y = startY;
  teleport_client.call(srv);
  pen_srv.request.off = false;
  pen_srv.request.width = 2;
  pen_client.call(pen_srv);
  loop_rate.sleep();

  int sizeCoordA = 5;
  int sizeCoordB = 2;
  float coordinates[sizeCoordA][sizeCoordB] = {{startX+5, startY},{startX+5, startY+4},{startX+2, startY+4},{startX,startY+6},{startX, startY}};

  for(int i=0; i<sizeCoordA; i++){
    srv.request.x = coordinates[i][0];
    srv.request.y = coordinates[i][1];
    teleport_client.call(srv);
    loop_rate.sleep();
  }

  */

/* Exercise 1
  int sizeCoordA = 5;
  int sizeCoordB = 2;
  float startX = 5.544445, startY = 5.544445;
  float coordinates[sizeCoordA][sizeCoordB] = {{startX+3, startY},{startX+3, startY+4},{startX-3, startY+4},{startX-3,startY},{startX, startY}};

  for(int i=0; i<sizeCoordA; i++){
    srv.request.x = coordinates[i][0];
    srv.request.y = coordinates[i][1];
    teleport_client.call(srv);
    loop_rate.sleep();
  }
*/
  return 0;
}