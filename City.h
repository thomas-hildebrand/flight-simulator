/*
	File: City.h
	Author: Thomas Hildebrand
	Project: UCSD CSE 167 - Computer Graphics
			 Final Project - Flight Simulator
	Description: Header file for City.cpp
*/

#ifndef City_h
#define City_h

#include "Transform.hpp"
#include "Window.h"
#include "Camera.h"
#include "Building.h"

#include <vector>
#include <tuple> 
#include <utility>  
#include <cmath>
#include <time.h>

typedef void func_t(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects);
typedef func_t* pfunc_t;
typedef std::vector<std::vector<pfunc_t>> grammarPtr;

// SkyScraper Gramamrs
void sg1(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects);
void sg2(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects);
void sg3(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects);
void sg5(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects);

// Office Building Grammars
void obg1(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects);
void obg2(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects);
void obg3(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects);

// House Building Grammars
void h1(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects);

// No Grammar, Mainly for testing
void ng(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects);


class City : public Transform {
private:
	float cityScale = 1.0f;
	float cityXRadius = 40.0f;
	float cityZRadius = 30.0f;
	float lotCloseness = 10.0f;

	// Bldg types
	int skyscraper = 0;
	int officeBldg = 1;
	int house = 2;

	/*
	grammarPtr bldgGrammar = {
		{sg1, sg1, sg2, sg3, sg5, sg5, sg5, sg5, sg5, sg5},  // SkyScraper grammars
		{obg1, obg2, obg3, obg3, obg3, sg5, sg5, sg5, sg5, sg5},	// Office Building grammars
		{h1, h1, h1, obg2, obg2, obg3, obg3, obg3, sg5, sg5}	// House grammars
	};
	*/


	grammarPtr bldgGrammar = {
		{sg5, sg5, sg5, sg5, sg5, sg5, sg5, sg5, sg5, sg5},  // SkyScraper grammars
		{sg5, sg5, sg5, sg5, sg5, sg5, sg5, sg5, sg5, sg5},	// Office Building grammars
		{sg5, sg5, sg5, sg5, sg5, sg5, sg5, sg5, sg5, sg5}	// House grammars
	};

	std::vector< std::pair<const char*, glm::vec3*>> bases = { 
		std::make_pair("cube.obj", &(glm::vec3(2.0f, 2.0f, 2.0f) * cityScale)),
		std::make_pair("cube.obj", &(glm::vec3(2.0f, 2.0f, 2.0f) * cityScale)),
		std::make_pair("cube.obj", &(glm::vec3(2.0f, 2.0f, 2.0f) * cityScale)),
		std::make_pair("cube.obj", &(glm::vec3(2.0f, 2.0f, 2.0f) * cityScale)),
		std::make_pair("body_s.obj", &(glm::vec3(1.0f, 1.0f, 1.0f) * cityScale)),
		std::make_pair("eyeball_s.obj", &(glm::vec3(12.0f, 12.0f, 12.0f) * cityScale)),
		std::make_pair("limb_s.obj", &(glm::vec3(10.0f, 1.85f, 10.0f) * cityScale)),
	};

	std::vector< std::vector< int> > Zoning = 
	{
		{ 0 },		// Zone 0 - City Center
		{ 1 },	// Zone 1 - Down Town
		{ 1, 2},	// Zone 2 - Mid Town
		{ 1, 2}		// Zone 3 - Suburbs
	};

	std::vector< std::vector< int> > Grammars =
	{
		{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 },		// Skyscrapers
		{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 },	// Office Buildings
		{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }	// Houses
	};

	

	struct Zones {
		
		//City Center
		glm::vec3 CCCoordinates = glm::vec3(-60.f, 5.0f, 0.0f);
		float CCRadiusMax = 7.0f;
		int CCFloorMax = 20;
		int CCFloorMin = 17;

		//DownTown
		float DTRadiusMax = 20.0f;
		int DTFloorMax = 13;
		int DTFloorMin = 8;

		//Midtown
		float MTRadiusMax = 40.0f;
		int MTFloorMax = 8;
		int MTFloorMin = 6;

		//Suburbs
		int SUBFloorMax = 4;
		int SUBFloorMin = 1;

	};

public:
	void draw(Camera* camera, GLuint shader);
	int setZone(float distance, Zones zone);
	int setHeight(int zoneNum, Zones zone);
	int setBldgType(int zoneNum);
	int setGrammar(int bldgType);

	City(std::vector<Transform*>*);

	~City();

};

#endif /* Geometry_hpp */
