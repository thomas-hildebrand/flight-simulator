# flight-simulator
This is a class project from my CSE 167: Computer Graphic course at University of California of San Diego.

The project is a basic flight simulator that includes a WWII fighter plane modele, procedurally generated terrain, water, a skybox, procedurally generated buildings, and city layouts.  

The project is written using Visual Studio, C++, OpenGL, and GLSL.

My personal Contributions include:
  Building.h
  Building.cpp
  City.h
  City.cpp
  shader.frag
  shader.vert
  Extensive work with Window.h and Window.cpp
  
My first main contribution is the procedurally generated buildings using shape grammars in City.cpp which provide a set of rules for the program when randomly constructing building objects.

My second main contribution is the city layout generator which defines the parameters for each specific building based on the building placement in the predetermined city zones.  

Using the location at which the building will be placed the city generator determines the size of the building(taller buildings near the city center, smaller buildings further away from city center), building type (i.e. Skyscraper, Office Building, House), and finally an appropriate shape grammar to use based on building type.  At this time the building generator is then called and using the randomly choosen shape grammer the building generator will then generator a building.  At this time the city generator will then move on to the next city lot and once again determine the appropriate shape grammar for the next building.  This will continue until all city lots have been filled.
