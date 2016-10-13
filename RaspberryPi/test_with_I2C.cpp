#include "opencv2/opencv.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <iostream>

#define ADDRESS 0x04

using namespace cv;
using namespace std;

static const char *devName = "/dev/i2c-1";

int main(int, char **argv)
{
  //Mat in_image, in_image2;
  Mat test_m[64] = Mat(768, 1280, CV_16U);
  Mat gray = Mat(768, 1280, CV_16U);
  int shift = 0, a, k = 0;
  unsigned char st[8]="s";
  char  counter[8];

  printf("I2C connection started!\n");
  int file;
  if((file = open(devName, O_RDWR)) < 0) {
    fprintf(stderr, "I2C: failed to access %d\n", devName);
    exit(1);
  }
  printf("output is %d\n", (int)file);
  printf("I2C: acquiring bus to 0x%x\n", ADDRESS);
  if(a=ioctl(file, I2C_SLAVE, ADDRESS) < 0) {
    printf("I2C: failed to acquire bus access/talk to slave 0x%x\n", ADDRESS);
    exit(1);
  }
  printf("output is %d\n", a);
  
  for(int n=0 ; n < 64 ; n++)
    {
      shift = n * 5;
      for(int i =0; i < 1280; i++)
	{
	  if(((i+shift) % 320) < 160)
	    {
	      for(int j = 0; j < 768; j++) test_m[n].at<uint16_t>(j, i) = 65535;
	    }
	}
    }

  for(int i =0; i < 1280; i++)
  {
    for(int j = 0; j < 768; j++) gray.at<uint16_t>(j, i) = 32767;
  }

  //  in_image = imread(argv[1], IMREAD_UNCHANGED);
  // in_image2 = imread(argv[2], IMREAD_UNCHANGED);
  
  //if(in_image.empty() || in_image2.empty())
  //  {
  //    printf("Can NOT open image!\n");
  //    return -1;
  //  }

  namedWindow("FULL", WINDOW_NORMAL);
  setWindowProperty("FULL", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

  imshow("FULL", gray);
  //imshow("FULL", in_image);
  
  printf("sleep\n");

  for(;;)
    {
      sprintf(counter, "%.3d", ++k);
      write(file, counter, 3); usleep(1000); 
      write(file, st, 1); 
      for(int n = 0; n < 64; n++)
	{
	  if(waitKey(20) > 0)
	    {
	      write(file, "x", 1); usleep(10000);
	      return 0;
	    }
	  imshow("FULL", test_m[n]); 
	  //    imshow(argv[1], in_image2);

	  //waitKey(10);
	  //    imshow(argv[1], in_image);
	}
      //waitKey(10);
      imshow("FULL", gray);
      write(file, "g", 1); usleep(10000);
      if(waitKey(4000) > 0)
	{
	      write(file, "x", 1); usleep(10000);
	      return 0;
	}
      //  imshow(argv[1], test_m[0]);
    }
  
  waitKey();    
  return 0;
}
