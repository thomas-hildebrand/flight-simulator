/*
File: City.cpp
Author: Thomas Hildebrand
Project: UCSD CSE 167 - Computer Graphics
		 Final Project - Flight Simulator
Description: Subclass of Transfrom class. Procedurally generates City using procedurally generated buildings
*/

#include "City.h"

/*
Function: City::City(std::vector<Transform*>* objects) : Transform()
Description: Constructor for City object.
Parameters:
	objects - Pointer to vector containing all objects
Return: NONE
*/
City::City(std::vector<Transform*>* objects) : Transform()
{
	std::cout << "hi" << std::endl;
	// Zones Structure
	Zones zone;

	// Generate seed for random number generation using time
	srand(time(0));

	// For Loops simulate city streets.  Determine x, z coordinates of center of city lots.
	for (float x = zone.CCCoordinates.x - cityXRadius * cityScale; x < zone.CCCoordinates.x + cityXRadius * cityScale; x += lotCloseness * cityScale) {
		for (float z = zone.CCCoordinates.z - cityZRadius * cityScale; z < zone.CCCoordinates.z + cityZRadius * cityScale; z += lotCloseness * cityScale) {
			
			// Compute Distance from city center (city origin)
			float distance = sqrt(pow(zone.CCCoordinates.x - x,2) + pow(zone.CCCoordinates.z - z, 2));

			// Set city zone based on distance from city center
			int zoneNum = setZone(distance, zone);
			
			// Set bldgType based on city zone
			int bldgType = setBldgType(zoneNum);

			// Set Grammar based on bldgType
			int grammar = setGrammar(bldgType);

			// Set building height based on city zone
			int height = setHeight(zoneNum, zone) * cityScale;

			// Building Grammar dependent on zone and bldg type
			int base = rand() % bases.size();

			// city y coordinates
			float y = zone.CCCoordinates.y;

			// Construct building using appropriate building grammar
			(*bldgGrammar[bldgType][grammar])( x,  y,  z, bases,  base,  zoneNum,  height, objects);

		}
	}

}

/*
Function: City::~City()
Description: Destructor for City object.
Parameters: NONE
Return: NONE
*/
City::~City()
{

}

/*
Function: void City::draw(Camera* C, GLuint shader)
Description: Virtual Function, unused.
Parameters: NONE
Return: NONE
*/
void City::draw(Camera* C, GLuint shader)
{
	
}

/*
Function: int City::setZone(float distance, Zones zone)
Description: Set city zone of building based on distance from city center.
Parameters:
	distance - Distance from city center.
	zone - Conatains values concerning city zones such as zone radius from city center.
Return: Returns assigned city zone.
*/
int City::setZone(float distance, Zones zone)
{
	if (distance <= zone.CCRadiusMax * cityScale) {
		return 0;
	}
	else if (distance <= zone.DTRadiusMax * cityScale) {
		return 1;
	}
	else if (distance <= zone.MTRadiusMax * cityScale) {
		return 2;
	}
	else {
		return 3;
	}
}

/*
Function: int City::setHeight(int zoneNum, Zones zone)
Description: Set height of building based on city zone building is located in.
Parameters:
	zoneNum - Assigned city zone
	zone - Conatains values concerning city zones
Return: Returns assgined building height.
*/
int City::setHeight(int zoneNum, Zones zone)
{
	if (zoneNum == 0) {
		return (rand() % zone.CCFloorMax + zone.CCFloorMin);
	}
	else if (zoneNum == 1) {
		return (rand() % zone.DTFloorMax + zone.DTFloorMin);
	}
	else if (zoneNum == 2) {
		return (rand() % zone.MTFloorMax + zone.MTFloorMin);
	}
	else {
		return (rand() % zone.SUBFloorMax + zone.SUBFloorMin);
	}
}

/*
Function: int City::setBldgType(int zoneNum)
Description: Set building type based on city zone building is located in.
Parameters:
	zoneNum - Assigned city zone
Return: Returns assgined building type.
*/
int City::setBldgType(int zoneNum)
{

	int result = rand() % Zoning[zoneNum].size();
	return Zoning[zoneNum][result];
}

/*
Function: int City::setGrammar(int bldgType)
Description: Set shape grammar based on building type.
Parameters:
	bldgType - Assigned building type
Return: Returns assgined shape grammar.
*/
int City::setGrammar(int bldgType)
{
	int result = rand() % Grammars[bldgType].size();

	return Grammars[bldgType][result];

}

/*
SHAPE GRAMMARS
All shape grammars create buildings by randomly selecting shape grammar based on building type which is determined by city zone of building location.
*/

/*
Function: void sg1(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
Description: Skyscraper shape grammar 1.  Reserved designs for only the tallest buildings in the center of the city. 
	x - x coordinate of building's base
	y - y coordinate of building's base
	z - z coordinate of building's base
	bases - List of all possible buiding bases and their attributes
	base - Building base to be used when constructing the building
	zoneNum - City zone the building will be placed in (based on distance from city center)
	height - Determines how tall the building will be at its maximum point.
	objects - Vector containing list of building objects.  Used to store newly created building.
*/
void sg1(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
{
	
	float xscale = 1.0f;
	float yscale = 1.0f;
	Building* b1;
	if (rand() % 4 == 0) {
		xscale = 0.75f;
		yscale = 0.25f;
		b1 = new Building(bases[base].first, &glm::vec3(bases[base].second->x + xscale, bases[base].second->y , bases[base].second->z - yscale), height);
		if (rand() % 4 == 0) {
		b1->SetRotation(glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}
	else
	{
		b1 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * 2.0f, bases[base].second->y, bases[base].second->z* 2.0f), height);
		if (rand() % 4 == 0) {
			b1->SetRotation(glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}

	b1->SetPosition(glm::vec3(x, y, z));
	objects->push_back((Transform*)b1);
}

/*
Function: void sg2(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
Description: Skyscraper shape grammar 2.  Buildings will not be as tall as sg1() but still large buildings.
	x - x coordinate of building's base
	y - y coordinate of building's base
	z - z coordinate of building's base
	bases - List of all possible buiding bases and their attributes
	base - Building base to be used when constructing the building
	zoneNum - City zone the building will be placed in (based on distance from city center)
	height - Determines how tall the building will be at its maximum point.
	objects - Vector containing list of building objects.  Used to store newly created building.
*/
void sg2(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
{
	float xscale = 1.25f;
	float zscale = 1.25f;
	float rescale = 0.65f;
	float translate = 1.0f;
	
	Building* b1 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height);
	b1->SetPosition(glm::vec3(x, y, z));
	objects->push_back((Transform*)b1);

	int rule = rand() % 8;
	if (rule == 0) {

		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 1)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 2)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x - translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 3)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x - translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 4)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 5)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x - translate, y - 0.01f, z));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 6)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x, y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 7)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x, y - 0.01f, z - translate));
		objects->push_back((Transform*)b2);
	}
	
	
}

/*
Function: void sg3(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
Description: Skyscraper shape grammar 3.  Buildings will not be as tall as sg1() but still large buildings.
	x - x coordinate of building's base
	y - y coordinate of building's base
	z - z coordinate of building's base
	bases - List of all possible buiding bases and their attributes
	base - Building base to be used when constructing the building
	zoneNum - City zone the building will be placed in (based on distance from city center)
	height - Determines how tall the building will be at its maximum point.
	objects - Vector containing list of building objects.  Used to store newly created building.
*/
void sg3(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
{
	float xscale = 1.25f;
	float zscale = 1.25f;
	float rescale = 0.65f;
	float translate = 1.0f;

	if (rand() % 2 == 0)
	{
		xscale = 1.5f;
		zscale = 1.5f;
	}

	Building* b1 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height);
	b1->SetPosition(glm::vec3(x, y, z));
	objects->push_back((Transform*)b1);

	int rule = rand() % 8;
	if (rule == 0) {

		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x - translate, y - 0.01f, z));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 1)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x - translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 2)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x , y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x , y - 0.01f, z - translate));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 3)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x - translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x + translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 4)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x - translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 5)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x + translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 6)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x - translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 7)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x - translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x - translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b3);
	}
}

/*
Function: void sg5(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
Description: Skyscraper shape grammar 5.  Buildings will not be as tall as sg1() but still large buildings.
	x - x coordinate of building's base
	y - y coordinate of building's base
	z - z coordinate of building's base
	bases - List of all possible buiding bases and their attributes
	base - Building base to be used when constructing the building
	zoneNum - City zone the building will be placed in (based on distance from city center)
	height - Determines how tall the building will be at its maximum point.
	objects - Vector containing list of building objects.  Used to store newly created building.
*/
void sg5(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
{
	int rule = rand() % 5;
	//int rule = 4;

	if (rule == 0) { 
		float xscale = 1.25f;
		float zscale = 1.25f;
		float rescale = 0.75f;
		float translate = 1.0f;

		Building* b1 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height);
		b1->SetPosition(glm::vec3(x, y, z));
		objects->push_back((Transform*)b1);

		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x - translate, y - 0.01f, z));
		objects->push_back((Transform*)b3);

		Building* b4 = new Building(*b2);
		b4->SetPosition(glm::vec3(x , y - 0.01f, z + translate));
		objects->push_back((Transform*)b4);

		Building* b5 = new Building(*b2);
		b5->SetPosition(glm::vec3(x , y - 0.01f, z - translate));
		objects->push_back((Transform*)b5);

	}
	else if (rule == 1)
	{
		float xscale = 1.25f;
		float zscale = 1.25f;
		float rescale = 0.75f;
		float translate = 1.0f;

		Building* b1 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height);
		b1->SetPosition(glm::vec3(x, y, z));
		objects->push_back((Transform*)b1);

		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.60f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x + translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b3);

		Building* b4 = new Building(*b2);
		b4->SetPosition(glm::vec3(x - translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b4);

		Building* b5 = new Building(*b2);
		b5->SetPosition(glm::vec3(x - translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b5);
	}
	else if (rule == 2)
	{
		
		float xscale = 1.25f;
		float zscale = 0.0f;
		float rescale = 0.66f;
		float xtranslate = 1.0f;
		float ztranslate = 1.0f;

		Building* b1 = new Building(bases[0].first, &glm::vec3(bases[0].second->x + 2.0f, bases[0].second->y, bases[0].second->z), height);
		b1->SetPosition(glm::vec3(x, y, z));
		objects->push_back((Transform*)b1);
		
		Building* b2 = new Building(bases[6].first, &glm::vec3(bases[6].second->x * rescale, bases[6].second->y, bases[6].second->z * rescale), height *= 0.80f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + xtranslate, y - 0.01f, z + ztranslate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x + xtranslate, y - 0.01f, z - ztranslate));
		objects->push_back((Transform*)b3);

		Building* b4 = new Building(*b2);
		b4->SetPosition(glm::vec3(x - xtranslate, y - 0.01f, z + ztranslate));
		objects->push_back((Transform*)b4);

		Building* b5 = new Building(*b2);
		b5->SetPosition(glm::vec3(x - xtranslate, y - 0.01f, z - ztranslate));
		objects->push_back((Transform*)b5);
	}
	else if (rule == 3)
	{
		float xscale = 1.5f;
		float zscale = 0.75f;
		float rescale = 1.0f;
		float xtranslate = 1.5f;

		Building* b1 = new Building(bases[0].first, &glm::vec3(bases[0].second->x + xscale, bases[0].second->y, bases[0].second->z - zscale), height);
		b1->SetPosition(glm::vec3(x, y, z));
		objects->push_back((Transform*)b1);

		Building* b2 = new Building(bases[0].first, &glm::vec3(bases[0].second->x * rescale, bases[0].second->y, bases[0].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + xtranslate, y - 0.01f, z));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x - xtranslate, y - 0.01f, z));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 4)
	{
	float xscale = 1.5f;
	float zscale = 0.75f;
	float rescale = 1.5f;

		Building* b1 = new Building(bases[0].first, &glm::vec3(bases[0].second->x + xscale, bases[0].second->y, bases[0].second->z - zscale), height);
		b1->SetPosition(glm::vec3(x, y, z));
		objects->push_back((Transform*)b1);

		Building* b2 = new Building(bases[4].first, &glm::vec3(bases[4].second->x * rescale, bases[4].second->y, bases[4].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x , y - 0.01f, z));
		objects->push_back((Transform*)b2);
	}
}

/*
Function: void obg1(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
Description: Office Building shape grammar 1.  Medium to small buildings.  Always shorter than skyscrapers.
	x - x coordinate of building's base
	y - y coordinate of building's base
	z - z coordinate of building's base
	bases - List of all possible buiding bases and their attributes
	base - Building base to be used when constructing the building
	zoneNum - City zone the building will be placed in (based on distance from city center)
	height - Determines how tall the building will be at its maximum point.
	objects - Vector containing list of building objects.  Used to store newly created building.
*/
void obg1(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
{
	Building* b1;
	if (rand() % 4 == 0) {
	
		b1 = new Building(bases[base].first, &glm::vec3(bases[base].second->x, bases[base].second->y, bases[base].second->z), height);
		if (rand() % 4 == 0) {
			b1->SetRotation(glm::vec3(0.0f, 2.0f, 0.0f));
		}
	}
	else
	{
		b1 = new Building(bases[base].first, &glm::vec3(bases[base].second->x , bases[base].second->y, bases[base].second->z ), height);
		if (rand() % 4 == 0) {
			b1->SetRotation(glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}

	b1->SetPosition(glm::vec3(x, y, z));
	objects->push_back((Transform*)b1);
}

/*
Function: void obg2(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
Description: Office Building shape grammar 2.  Medium to small buildings.  Always shorter than skyscrapers.
	x - x coordinate of building's base
	y - y coordinate of building's base
	z - z coordinate of building's base
	bases - List of all possible buiding bases and their attributes
	base - Building base to be used when constructing the building
	zoneNum - City zone the building will be placed in (based on distance from city center)
	height - Determines how tall the building will be at its maximum point.
	objects - Vector containing list of building objects.  Used to store newly created building.
*/
void obg2(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
{
	float xscale = 0.9f;
	float zscale = 0.9f;
	float rescale = 0.65f;
	Building* b1 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height);
	b1->SetPosition(glm::vec3(x, y, z));
	objects->push_back((Transform*)b1);

	int rule = rand() % 8;
	if (rule == 0) {

		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + 0.75f, y - 0.01f, z + 0.75f));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 1)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + 0.75f, y - 0.01f, z - 0.75f));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 2)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x - 0.75f, y - 0.01f, z + 0.75f));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 3)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x - 0.75f, y - 0.01f, z - 0.75f));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 4)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + 0.75f, y - 0.01f, z));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 5)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x - 0.75f, y - 0.01f, z));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 6)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x, y - 0.01f, z + 0.75f));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 7)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x, y - 0.01f, z - 0.75f));
		objects->push_back((Transform*)b2);
	}

}

/*
Function: void obg3(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
Description: Office Building shape grammar 3.  Medium to small buildings.  Always shorter than skyscrapers.
	x - x coordinate of building's base
	y - y coordinate of building's base
	z - z coordinate of building's base
	bases - List of all possible buiding bases and their attributes
	base - Building base to be used when constructing the building
	zoneNum - City zone the building will be placed in (based on distance from city center)
	height - Determines how tall the building will be at its maximum point.
	objects - Vector containing list of building objects.  Used to store newly created building.
*/
void obg3(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
{
	float xscale = 1.0f;
	float zscale = 1.0f;
	float rescale = 0.65f;
	float translate = 1.0f;

	if (rand() % 2 == 0)
	{
		xscale = 1.25f;
		zscale = 1.25f;
	}

	Building* b1 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height);
	b1->SetPosition(glm::vec3(x, y, z));
	objects->push_back((Transform*)b1);

	int rule = rand() % 8;
	if (rule == 0) {

		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x - translate, y - 0.01f, z));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 1)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x - translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 2)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x, y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x, y - 0.01f, z - translate));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 3)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x - translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x + translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 4)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x - translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 5)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x + translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 6)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x - translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 7)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x - translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x - translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b3);
	}
}

/*
Function: void h1(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
Description: House shape grammar 1.  Small buildings.  Never bigger than office buildings.
	x - x coordinate of building's base
	y - y coordinate of building's base
	z - z coordinate of building's base
	bases - List of all possible buiding bases and their attributes
	base - Building base to be used when constructing the building
	zoneNum - City zone the building will be placed in (based on distance from city center)
	height - Determines how tall the building will be at its maximum point.
	objects - Vector containing list of building objects.  Used to store newly created building.
*/
void h1(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
{
	float xscale = 0.5f;
	float zscale = 0.5f;
	Building* b1;
	
	b1 = new Building(bases[base].first, &glm::vec3(bases[base].second->x + xscale, bases[base].second->y, bases[base].second->z - zscale), height);
	if ((float)(rand() % 4) == 0) {
		b1->SetRotation(glm::vec3(0.0f, 2.0f , 0.0f));
	}
	b1->SetPosition(glm::vec3(x, y, z));
	objects->push_back((Transform*)b1);
}

/*
Function: void ng(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
Description: No grammar.  Reserves an empty lot(s) for placing landmark, unique building, etc.
	x - x coordinate of building's base
	y - y coordinate of building's base
	z - z coordinate of building's base
	bases - List of all possible buiding bases and their attributes
	base - Building base to be used when constructing the building
	zoneNum - City zone the building will be placed in (based on distance from city center)
	height - Determines how tall the building will be at its maximum point.
	objects - Vector containing list of building objects.  Used to store newly created building.
*/
void ng(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
{
	// No grammar, don't render anything
}