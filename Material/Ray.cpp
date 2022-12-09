

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>       
#include <glm/vec3.hpp> 
#include <glm/vec4.hpp> 
#include <glm/mat4x4.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"





class Ray
{

public:
	
	vec3 intersection;
	vec3 origin;
	vec3 dir;

	vec2 getNormCoords(int scrWidth, int scrHeight,vec2 coordsMouse) {
		vec2 res{};
		res.x = 2 * coordsMouse[0] / scrWidth - 1;
		res.y = 1 - 2 * coordsMouse[1] / scrHeight;
		return res;
	}


	vec4 toEye(mat4 proj, vec4 clip) {
		vec4 eyeCoordinates = glm::inverse(proj) * clip;
		return vec4(eyeCoordinates.x, eyeCoordinates.y, -1.0f, 0.0f);
	}

	vec3 toWorld(vec4 eyeCoordinates, mat4 viewMatrix) {
		vec4 ray = glm::inverse(viewMatrix) * eyeCoordinates;
		return glm::normalize(vec3(ray));
	}

	vec3 createRay(int scrWidth, int scrHeight, Camera camera, mat4 proj,float fieldOfView, vec2 coordsMouse) {
		vec2 normCoords = getNormCoords(scrWidth, scrHeight, coordsMouse);
		vec4 clippedCoordinates = vec4(normCoords[0], normCoords[1], -1.0f, 1.0f);
		vec4 eyeCoordinates = toEye(proj,clippedCoordinates);
		vec3 ray = toWorld(eyeCoordinates, camera.GetViewMatrix());
		origin = camera.Position;
		dir = ray;
		return ray;
	}

	bool rayTriangleIntersection(vec3 origin, vec3 dir, vec3 p0, vec3 p1, vec3 p2) {

		vec3 p0p1, p0p2;
		p0p1.x = p1.x - p0.x;
		p0p1.y = p1.y - p0.y;
		p0p1.z = p1.z - p0.z;

		p0p2.x = p2.x - p0.x;
		p0p2.y = p2.y - p0.y;
		p0p2.z = p2.z - p0.z;

		vec3 N = glm::cross(p0p1, p0p2);

		float kEpsilon = 0.000001;

		float nRayDir = glm::dot(N, dir);

		if (fabs(nRayDir) < kEpsilon) {
			return false;
		}

		float d = glm::dot(N, p0);


		float t = -((glm::dot(N, origin) - d) / nRayDir);

		vec3 P;
		P.x = origin.x + t * dir.x;
		P.y = origin.y + t * dir.y;
		P.z = origin.z + t * dir.z;

		intersection = P;

		vec3 C;

		vec3 edge0;

		edge0.x = p1.x - p0.x;
		edge0.y = p1.y - p0.y;
		edge0.z = p1.z - p0.z;

		vec3 vp0;

		vp0.x = P.x - p0.x;
		vp0.y = P.y - p0.y;
		vp0.z = P.z - p0.z;

		C = glm::cross(edge0, vp0);

		if (glm::dot(N, C) < 0) { return false; }

		vec3 e1;

		e1.x = p2.x - p1.x;
		e1.y = p2.y - p1.y;
		e1.z = p2.z - p1.z;

		vec3 vp1;

		vp1.x = P.x - p1.x;
		vp1.y = P.y - p1.y;
		vp1.z = P.z - p1.z;

		C = glm::cross(e1, vp1);

		if (glm::dot(N, C) < 0) { return false; }

		vec3 e2;

		e2.x = p0.x - p2.x;
		e2.y = p0.y - p2.y;
		e2.z = p0.z - p2.z;

		vec3 vp2;

		vp2.x = P.x - p2.x;
		vp2.y = P.y - p2.y;
		vp2.z = P.z - p2.z;

		C = glm::cross(e2, vp2);

		if (glm::dot(N, C) < 0) { return false; }

		return true;
	}
	
	

	bool rayIntersection(vec3 v1, vec3 v2, vec3 v3, vec3 origin, vec3 dir)
	{
		bool result = true;
		vec3 e1 = v2 - v1;
		vec3 e2 = v3 - v1;

		vec3 pvec = glm::cross(dir,e2);
		vec3 normRayDir = glm::normalize(dir);

		float det = glm::dot(pvec, e1);
			
		float ep = 0.5f;

		if (det < ep)
			{
				result = false;
				return result;
			}

			vec3 triangleVertMinusRayOrig = origin - v1;
			float u = glm::dot(triangleVertMinusRayOrig, pvec);
			if (u < 0.0 || u > det)
			{
				result = false;
				return result;
			}

		    vec3 qvec = glm::cross(triangleVertMinusRayOrig,e1);
			float v = glm::dot(normRayDir, qvec);

			if (v < 0.0f || v + u > det)
			{
				result = false;
				return result;
			}
			

			if (det < ep && det > -ep)
			{
				result = false;
				return result;
			}

			float invDet = 1.0f / det;
			triangleVertMinusRayOrig = origin - v1;
			 u = invDet * glm::dot(triangleVertMinusRayOrig, pvec);

			if (u < 0.0f || u > 1.0f)
			{
				result = false;
				return result;
			}

			qvec = glm::cross(triangleVertMinusRayOrig, e1);
			 v = invDet * glm::dot(qvec, normRayDir);

			if (v < 0.0f || u + v > 1.0f)
			{
				result = false;
				return result;
			}

			return result;
		}
	
};