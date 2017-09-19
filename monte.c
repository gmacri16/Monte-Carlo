/*
Gregory Macri

This program takes user input on the amount of points in a circle that 
they would like to generate divided amoung a user specified amount of threads.
These randomly generated points will provide us the data to do a Monte Carlo
estimation to get an estimated value of pi.
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>


int numberOfPoints;//Number of points divided by number of threads
int returnValue[10];//Return values of each thread

//gets the current time, converts to seconds and returns value as double
double mytime()
{
  struct timeval t1;
  gettimeofday(&t1,NULL);
  
  return t1.tv_sec + t1.tv_usec/1000000.0;
}

//Generates random points within a range of -1 to 1 and calculates the number
//of points generated that fall within or on the circle
void *PointsInCircle(void *threadid)
{
  int id = (long)threadid;//Takes threadid and casts as int to be stored in id

  unsigned int seed = id;//seed for rand_r

  int i,pointsInC,random_x,random_y;
  double true_x,true_y,diameter;
  
  pointsInC = 0;//Total number of points in a circle

  diameter = 0;//Diameter of the circle based on the points

  for(i = 0; i < numberOfPoints;++i)//Generate this number of points
  {
    random_x = rand_r(&seed);//get random x value
    random_y = rand_r(&seed);//get random y value

    true_x = (random_x * 1.0)/RAND_MAX;//get value within range of 0 to 1 
    true_y = (random_y * 1.0)/RAND_MAX;//
    true_x = (true_x * 2.0) - 1.0;//get value within range of -1 to 1
    true_y = (true_y * 2.0) - 1.0;//

    diameter = sqrt((true_x * true_x) + (true_y * true_y));//get diameter

    if(diameter <= 1.0)//if diameter is less than or equal to 1
      ++pointsInC;//increment number of points in a circle  
  }

  returnValue[id] = pointsInC;//store value in global array
  pthread_exit(NULL);
}

int main(int argc,char *argv[])
{
  int numberOfThreads,i,sum;
  double newNum,elapsedTime,t1secs, t2secs;
  numberOfThreads = atoi(argv[1]);//get number of threads from command line
  numberOfPoints = atoi(argv[2]) / numberOfThreads;//get number of points

  pthread_t threads[numberOfThreads];//initialize thread array

  t1secs = mytime();//get start time in seconds

  //generate threads
  for(i = 0;i < numberOfThreads;++i)
  {
    pthread_create(&threads[i], NULL, PointsInCircle,(void *)i);
  }
  
  //wait for all threads to finish
  for(i = 0;i < numberOfThreads;++i)
  {
    pthread_join(threads[i], NULL);
  }

  sum = 0;

  //get the sum of all points in the circle from all threads
  for(i = 0;i < numberOfThreads;++i)
  {
    sum = returnValue[i];
  }

  newNum = numberOfPoints * 1.0;//get pi estimation
  newNum = (4*sum)/newNum;//

  t2secs = mytime();//get end time
  elapsedTime = t2secs - t1secs;//compute difference

  printf("Estimate of pi is %lf\n",newNum);//print pi estimate
  printf("It took %lf seconds\n",elapsedTime);//print elapsed time
  
  pthread_exit(0);
}



