#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <ray_tracing.h>
#include <camera.h>
#include "material.h"

// Define the pins used for the display
#define TFT_CS 10
#define TFT_DC 9
#define TFT_MOSI 11
#define TFT_CLK 13

// Create an instance of the display
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

void setup() {
  // Set up the display by beginning the SPI connection
  SPI.setMOSI(TFT_MOSI);
  SPI.setSCK(TFT_CLK);

  // Initialize the display
  tft.begin();

  // Set the rotation of the display
  tft.setRotation(3);
}

void loop() {
  // Clear the display
  tft.fillScreen(ILI9341_BLACK);

  // Create the World
  hittable_list world;

  // Create the materials
  // auto R = cos(pi/4);
  // auto material_left = make_shared<lambertian>(Color(0, 0, 1));
  // auto material_right = make_shared<lambertian>(Color(1, 0, 0));

  // // Create the spheres
  // world.add(make_shared<sphere>(point3(-R, 0, -1), R, material_left));
  // world.add(make_shared<sphere>(point3( R, 0, -1), R, material_right));
  // auto material_ground = std::make_shared<lambertian>(Color(0.8, 0.8, 0.0));
  // auto material_center = std::make_shared<lambertian>(Color(0.1, 0.2, 0.5));
  // auto material_left = std::make_shared<dielectric>(1.5);
  // auto material_bubble = std::make_shared<dielectric>(1/1.5);
  // auto material_right = std::make_shared<metal>(Color(0.8, 0.6, 0.2), 0.0);

  // Create the spheres
  // world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
  // world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.2),   0.5, material_center));
  // world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
  // world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.4, material_bubble));
  // world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));
  // auto material_ground = make_shared<lambertian>(Color(0.8, 0.8, 0.0));
  // auto material_center = make_shared<lambertian>(Color(0.1, 0.2, 0.5));
  // auto material_left   = make_shared<dielectric>(1.50);
  // auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
  // auto material_right  = make_shared<metal>(Color(0.8, 0.6, 0.2), 1.0);

  // world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
  // world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.2),   0.5, material_center));
  // world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
  // world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.4, material_bubble));
  // world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));
  auto ground_material = make_shared<lambertian>(Color(0.5, 0.5, 0.5));
  world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
      for (int b = -11; b < 11; b++) {
          auto choose_mat = random_double();
          point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

          if ((center - point3(4, 0.2, 0)).length() > 0.9) {
              shared_ptr<material> sphere_material;

              if (choose_mat < 0.8) {
                  // diffuse
                  auto albedo = Color::random() * Color::random();
                  sphere_material = make_shared<lambertian>(albedo);
                  world.add(make_shared<sphere>(center, 0.2, sphere_material));
              } else if (choose_mat < 0.95) {
                  // metal
                  auto albedo = Color::random(0.5, 1);
                  auto fuzz = random_double(0, 0.5);
                  sphere_material = make_shared<metal>(albedo, fuzz);
                  world.add(make_shared<sphere>(center, 0.2, sphere_material));
              } else {
                  // glass
                  sphere_material = make_shared<dielectric>(1.5);
                  world.add(make_shared<sphere>(center, 0.2, sphere_material));
              }
          }
      }
  }

  auto material1 = make_shared<dielectric>(1.5);
  world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<lambertian>(Color(0.4, 0.2, 0.1));
  world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<metal>(Color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
  
  // Create the camera
  camera cam;
  // cam.sample_per_pixel = 10;
  // cam.max_depth = 50;
  // cam.vfov = 20;
  // cam.lookfrom = point3(-2, 2, 1);
  // cam.lookat = point3(0, 0, -1);
  // cam.vup = Vector3(0, 1, 0);
  // cam.defocus_angle = 10.0;
  // cam.focus_distance   = 3.4;
  cam.sample_per_pixel = 50;
  cam.max_depth         = 50;

  cam.vfov     = 20;
  cam.lookfrom = point3(13,2,3);
  cam.lookat   = point3(0,0,0);
  cam.vup      = Vector3(0,1,0);

  cam.defocus_angle = 0.6;
  cam.focus_distance    = 10.0;

  cam.render(tft, world);


  
  delay(1000000); // Delay to view the result for a while
}