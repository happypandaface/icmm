#ifndef c_images
#define c_images


typedef struct{
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  unsigned char	 pixel_data[];
}cImage;

cImage stoneWall;
cImage gaseousBall;
cImage gaseousBall2;
cImage hand;
cImage punchFist;
#endif