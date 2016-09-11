#pragma once
#include "types.h"
#include "maths.h"
#include <3ds.h>
#include <float.h>
#include <stdbool.h>
#include <assert.h>

/**************************************************
 * Common Non-Standard Citro3D Functions 
 **************************************************/

/**
 * @brief Obtain the smaller C3D_FVec vector of the given 2 C3D_FVec vectors.
 * @param[in]    lhs      C3D_FVec vector to compare.
 * @param[in]    rhs      C3D_FVec vector to compare.
 * @return The smallest C3D_FVec vector.
 */
C3D_FVec FVec3_Min(C3D_FVec lhs, C3D_FVec rhs);

/**
 * @brief Obtain the larger C3D_FVec vector of the given 2 C3D_FVec vectors.
 * @param[in]    lhs      C3D_FVec vector to compare.
 * @param[in]    rhs      C3D_FVec vector to compare.
 * @return The largest C3D_FVec vector.
 */
C3D_FVec FVec3_Max(C3D_FVec lhs, C3D_FVec rhs);

/**
 * @brief See: http://box2d.org/2014/02/computing-a-basis/
 * @param[in]   a   A unit vector.
 * @param[out]  b   A unit vector perpendicular to unit vector, "a".
 * @param[out]  c   A unit vector perpendicular to unit vectors, "a" and "b".
 */ 
void FVec3_ComputeBasis(const C3D_FVec* a, C3D_FVec* b, C3D_FVec* c);

/**************************************************
 * Basic Structures 
 **************************************************/

typedef struct C3D_HalfSpace 
{
	C3D_FVec normal;
	float distance;
} C3D_HalfSpace;

typedef struct C3D_RaycastData 
{
	C3D_FVec rayOrigin;
	C3D_FVec direction;
	float endPointTime;
	float timeOfImpact;  //Solved time of impact.
	C3D_FVec normal;     //Surface normal at impact.
} C3D_RaycastData;

/**************************************************
 * AABB Box Collision Helper Functions 
 **************************************************/

typedef struct C3D_AABB 
{
	C3D_FVec min;
	C3D_FVec max;
} C3D_AABB;

/**
 * @brief Checks if inner AABB box is within the outer AABB box.
 * @param[in]   outer   Outer AABB box.
 * @param[in]   inner    Inner AABB box to compare with.
 * @return True if outer AABB box contains an inner AABB box. False, if otherwise. 
 */
bool AABB_ContainsAABB(const C3D_AABB* outer, const C3D_AABB* inner);

/**
 * @brief Checks if C3D_FVec point is within the AABB box.
 * @param[in]   outer   Outer AABB box.
 * @param[in]   inner   C3D_FVec vector position.
 * @return True if vector position is within the AABB box. False, if otherwise.
 */
bool AABB_ContainsFVec3(const C3D_AABB* outer, const C3D_FVec* inner);

/**
 * @brief Obtain the surface area of the AABB box specified.
 * @param[in]  myself   The AABB box for finding the surface area.
 * @return The surface area.
 */
float AABB_GetSurfaceArea(const C3D_AABB* myself);

/**
 * @brief Unionize two AABB boxes to create 1 bigger AABB box that contains the two AABB boxes.
 * @param[out]   out   The resulting larger AABB box.
 * @param[in]    a     The first AABB box.
 * @param[in]    b     The second AABB box.
 */
void AABB_Combine(C3D_AABB* out, const C3D_AABB* a, const C3D_AABB* b);

/**
 * @brief Checks if the two AABB boxes intersects each other.
 * @param[in]    a     The first AABB box.
 * @param[in]    b     The second AABB box.
 * @return True if both boxes intersect each other. False, if otherwise.
 */
bool AABB_CollidesAABB(const C3D_AABB* a, const C3D_AABB* b);

/**************************************************
 * Raycasting Helper Functions
 **************************************************/

/**
 * @brief Create a new C3D_RaycastData object.
 * @param[out]    out            The resulting C3D_RaycastData object. If out is NULL, it will crash.
 * @param[in]     origin         The beginning point of the ray.
 * @param[in]     direction      The direction of the ray.
 * @param[in]     endPointTime   The time specifying the ray end point.
 */
static inline void Raycast_New(C3D_RaycastData* out, const C3D_FVec* origin, const C3D_FVec* direction, const float endPointTime) 
{
	out->rayOrigin = *origin;
	out->direction = FVec3_Normalize(*direction);
	out->endPointTime = endPointTime;
}

/**************************************************
 * Half-Space Helper Functions
 **************************************************/

/**
 * @brief Create a new C3D_HalfSpace object.
 * @param[out]     out         The resulting C3D_HalfSpace object. If out is NULL, it will crash.
 * @param[in]      normal      The normal vector.
 * @param[in]      distance    The distance.
 */
static inline void HS_Init(C3D_HalfSpace* out, const C3D_FVec normal, float distance)
{
	out->normal = normal;
	out->distance = distance;
}

/**
 * @brief Create a new C3D_HalfSpace object, using 3 points in the 3D space.
 * @param[out]     out         The resulting C3D_HalfSpace object. If out is NULL, it will crash.
 * @param[in]      a           The first point.
 * @param[in]      b           The second point.
 * @param[in]      c           The third point.
 */
static inline void HS_NewFVec(C3D_HalfSpace* out, const C3D_FVec* a, const C3D_FVec* b, const C3D_FVec* c)
{
	out->normal = FVec3_Normalize(FVec3_Cross(FVec3_Subtract(*b, *a), FVec3_Subtract(*c, *a)));
	out->distance = FVec3_Dot(out->normal, *a);
}

/**
 * @brief Create a new C3D_HalfSpace object, using a normal vector and a vector position.
 * @param[out]     out      The resulting C3D_HalfSpace object. If out is NULL, it will crash.
 * @param[in]      normal   The normal vector.
 * @param[in]      point    The vector position.
 */
static inline void HS_New(C3D_HalfSpace* out, const C3D_FVec* normal, const C3D_FVec* point)
{
	out->normal = FVec3_Normalize(*normal);
	out->distance = FVec3_Dot(out->normal, *point);
}

/**
 * @brief Obtain the origin from the C3D_HalfSpace object.
 * @param[in]     in    C3D_HalfSpace object to retrieve the origin vector position from.
 * @return The C3D_FVec origin.
 */
C3D_FVec HS_GetOrigin(C3D_HalfSpace* in);

/**
 * @brief Get the distance from the half space to the point.
 * @param[in]    in     C3D_HalfSpace object to measure from.
 * @param[in]    point  C3D_FVec vector position to measure to.
 * @return The distance between the half space to the point.
 */
float HS_GetDistance(C3D_HalfSpace* in, const C3D_FVec* point);

/**
 * @brief Projects the half space to the vector position.
 * @param[in]    in     C3D_HalfSpace object.
 * @param[in]    point  Projection destination position.
 * @return The projection vector.
 */
C3D_FVec HS_Project(C3D_HalfSpace* in, const C3D_FVec* point);

/**************************************************
 * Physics Memory Functions
 **************************************************/

//May need to change the stack and heap size to different values.
#define C3D_PHYSICSSTACK_MAX_SIZE 1024*20   //20KB
#define C3D_PHYSICSHEAP_MAX_SIZE 1024*20    //20KB
#define C3D_PHYSICSHEAP_INIT_SIZE 1024      //1KB
#define MACRO_POINTER_ADD(POINTER,BYTES) ((__typeof__(POINTER))(((u8 *)POINTER)+(BYTES)))

typedef struct C3D_PhysicsStackEntry 
{
	u8* data;
	unsigned int size;
} C3D_PhysicsStackEntry;

typedef struct C3D_PhysicsStack 
{
	u8 memory[C3D_PHYSICSSTACK_MAX_SIZE];
	struct C3D_PhysicsStackEntry* entries;
	
	unsigned int index;
	unsigned int allocation;
	unsigned int entryCount;
	unsigned int entryCapacity;
} C3D_PhysicsStack;

typedef struct HeapHeader 
{
	struct HeapHeader* next;
	struct HeapHeader* previous;
	unsigned int size;
} HeapHeader;

typedef struct HeapFreeBlock 
{
	struct HeapHeader* header;
	unsigned int size;
} HeapFreeBlock;

typedef struct C3D_PhysicsHeap 
{
	struct HeapHeader* memory;
	struct HeapFreeBlock* freeBlocks;
	unsigned int freeBlocksCount;
	unsigned int freeBlocksCapacity;
} C3D_PhysicsHeap;

typedef struct PageBlock 
{
	struct PageBlock* next;
} PageBlock;

typedef struct Page 
{
	struct Page* next;
	struct PageBlock* data;
} Page;

typedef struct C3D_PhysicsPage 
{
	unsigned int blockSize;
	unsigned int blocksPerPage;
	struct Page* pages;
	unsigned int pagesCount;
	struct PageBlock* freeList;
} C3D_PhysicsPage;

/**
 * @brief Initializes the C3D_PhysicsStack object. If out is NULL, it will crash.
 * @param[in,out]     out    C3D_PhyiscsStack object to be initialized.
 */
void PhysicsStack_Init(C3D_PhysicsStack* out);

/**
 * @brief Releases the C3D_PhysicsStack object. If out is NULL or if the C3D_PhysicsStack object is not empty, it will crash/assert failure.
 * @param[in]    in     Check/Assert the object if empty.  
 */
void PhysicsStack_Free(C3D_PhysicsStack* in);

/**
 * @brief Allocates new memory to the C3D_PhysicsStack object. If stack is NULL, it will crash.
 * @param[in,out]   stack      The C3D_PhysicsStack object to assign the allocated memory to.
 * @param[in]       newSize    Specify the new size of the allocated memory.
 * @return Pointer to the allocated memory.
 */
void* PhysicsStack_Allocate(C3D_PhysicsStack* stack, unsigned int newSize);

/**
 * @brief Releases the memory from the C3D_PhysicsStack object. If stack is NULL, it will crash.
 * @param[in,out]     stack       The C3D_PhysicsStack object to release memory from.
 * @param[in]         data        The pointer that the C3D_PhysicsStack object needs to reference to release.
 */
void PhysicsStack_Deallocate(C3D_PhysicsStack* stack, void* data);

/**
 * @brief Initializes the C3D_PhysicsHeap object.
 * @param[in,out]     out     The C3D_PhysicsHeap object to be initialized.
 */
void PhysicsHeap_Init(C3D_PhysicsHeap* out);

/**
 * @brief Releases the C3D_PhysicsHeap object.
 * @param[in,out]      out     The C3D_PhysicsHeap object to be released.
 */
void PhysicsHeap_Free(C3D_PhysicsHeap* out);

/**
 * @brief Allocates new memory to the C3D_PhysicsHeap object. If heap is NULL, it will crash.
 * @param[in,out]      heap       The C3D_PhysicsHeap object to assign the allocated memory to.
 * @param[in]          newSize    Specify the new size of the allocated memory.
 * @return Pointer to the allocated memory.
 */
void* PhysicsHeap_Allocate(C3D_PhysicsHeap* heap, unsigned int newSize);

/**
 * @brief Releases the memory from the C3D_PhysicsHeap object. If heap is NULL, it will crash.
 * @param[in,out]      heap      The C3D_PhysicsHeap object to release allocated memory from.
 * @param[in]          data      The pointer that the C3D_PhysicsHeap object needs to reference to release.
 */
void PhysicsHeap_Deallocate(C3D_PhysicsHeap* heap, void* data);

/**
 * @brief Initializes the C3D_PhysicsPage object. If out is NULL, it will crash.
 * @param[in,out]     out               The resulting C3D_PhysicsPage object.
 * @param[in]         elementSize       The size of each element intended for initialization.
 * @param[in]         elementsPerPage   The size of elements per page.
 */
void PhysicsPage_Init(C3D_PhysicsPage* out, unsigned int elementSize, unsigned int elementsPerPage);

/**
 * @brief Releases the memory from the C3D_PhysicsPage object. This is also used for clearing the C3D_PhysicsPage object. If out is NULL, it will crash.
 * @param[in,out]     out     The resulting C3D_PhysicsPage to be released.
 */
void PhysicsPage_Free(C3D_PhysicsPage* out);

/**
 * @brief Allocates new memory to the C3D_PhysicsPage object. if pageAllocator is NULL, it will crash.
 * @param[in,out]    pageAllocator    The C3D_PhysicsPage object. Here, this object handles the allocation of pages.
 * @return The page data that was most recently modified (allocated/deallocated).
 */
void* PhysicsPage_Allocate(C3D_PhysicsPage* pageAllocator);

/**
 * @brief Releases the allocated memory from the C3D_PhysicsPage object. If pageAllocator is NULL, it will crash.
 * @param[in,out]   pageAllocator    The C3D_PhysicsPage object to release the allocated memory from.
 * @param[in]       data             The pointer to the data that the C3D_PhysicsPage is referencing from to release.
 */
void PhysicsPage_Deallocate(C3D_PhysicsPage* pageAllocator, void* data);

/**************************************************
 * Axis-aligned Bounding Box (AABB) Container Functions.
 **************************************************/

typedef struct C3D_MassData 
{
	C3D_Mtx inertia;
	C3D_FVec center;
	float mass;
} C3D_MassData;

/**************************************************
 * Transform Helper Functions.
 **************************************************/

typedef struct C3D_Transform 
{
	C3D_FVec position;
	C3D_Mtx rotation;
} C3D_Transform;

/**
 * @brief Copies data from one to the other.
 * @param[out]   out   The destination to copy to.
 * @param[in]    in    The source to copy from.
 */
static inline void Transform_Copy(C3D_Transform* out, C3D_Transform* in)
{
	*out = *in;
}

/**
 * @brief Multiplies the C3D_Transform objects together. Breaks them into rotation matrix multiplication and matrix translation.
 * @param[out]      out      The resulting C3D_Transform object.
 * @param[in]       lhs      The C3D_Transform object operand to be multiplied on the left-hand side of the operator.
 * @param[in]       rhs      The C3D_Transform object operand to be multiplied on the right-hand side of the operator.
 */
static inline void Transform_Multiply(C3D_Transform* out, const C3D_Transform* lhs, const C3D_Transform* rhs)
{
	C3D_Transform temp;
	if (out == lhs || out == rhs)
	{
		Transform_Multiply(&temp, lhs, rhs);
		Transform_Copy(out, &temp);
		return;
	}
	Mtx_Multiply(&temp.rotation, &lhs->rotation, &rhs->rotation);
	temp.position = FVec3_Add(Mtx_MultiplyFVec3(&temp.rotation, rhs->position), lhs->position);
	temp.position.w = 1.0f;
	Transform_Copy(out, &temp);
}

/**
 * @brief First transposes the rotation matrix, then multiplies the rotation matrix with the vector.
 * @param[out]       out                The resulting C3D_FVec vector.
 * @param[in]        rotationMatrix     The rotation matrix, untransposed.
 * @param[in]        vector             The C3D_FVec vector to be multiplied.
 */
static inline void Transform_MultiplyTransposeFVec(C3D_FVec* out, const C3D_Mtx* rotationMatrix, const C3D_FVec* vector)
{
	C3D_Mtx transpose;
	Mtx_Copy(&transpose, rotationMatrix);
	Mtx_Transpose(&transpose);
	*out = Mtx_MultiplyFVec3(&transpose, *vector);
}

/**
 * @brief Shorthand version of multiplying the rotation matrix from the C3D_Transform with the vector, relative to the C3D_Transform position.
 * @param[out]    out           The resulting C3D_FVec vector.
 * @param[in]     transform     The C3D_Transform object to work with.
 * @param[in]     vector        The C3D_FVec vector relative to the C3D_Transform object's position.
 */
static inline void Transform_MultiplyTransformFVec(C3D_FVec* out, const C3D_Transform* transform, const C3D_FVec* vector)
{
	C3D_FVec difference = FVec3_Subtract(*vector, transform->position);
	Transform_MultiplyTransposeFVec(out, &transform->rotation, &difference);
}

/**************************************************
 * Box Helper Functions.
 **************************************************/

/**
 * @note Extent: Half-extents, or the half size of a full axis-aligned bounding box volume. Center of the box, plus half width/height/depth.
 *       Extents as in, you have vec3 and the real position of the box is -vec3 (AABB.min) and +vec3 (AABB.max).
 *       RandyGaul: "Extent, as in the extent of each OBB axis."
 */
typedef struct C3D_Box 
{
	struct C3D_Transform localTransform;
	C3D_FVec extent; 
	struct C3D_Box* next;
	struct C3D_Body* body;
	float friction;
	float restitution;
	float density;
	unsigned int broadPhaseIndex;
	void* userData;
	bool sensor;
} C3D_Box;

/**
 * @brief Sets user data. Because this is C, you can directly manipulate user data from the C3D_Box object, if you choose so.
 * @note Possibly not needed at all.
 * @param[in,out]     box          The resulting C3D_Box object to store the user data.
 * @param[in]         ptrData      Pointer to the user data store in C3D_Box object.
 */
static inline void Box_SetUserData(C3D_Box* box, void* ptrData)
{
	box->userData = ptrData;
}

/**
 * @brief Gets user data. Because this is C, you can directly access user data from the C3D_Box object, if you choose so.
 * @note Possibly not needed at all.
 * @param[in]     box       The resulting C3D_Box object to access the user data.
 * @return Pointer to the user data from the C3D_Box object.
 */
static inline void* Box_GetUserData(C3D_Box* box)
{
	return box->userData;
}

/**
 * @brief Sets the C3D_Box object sensor flag.
 * @note Possibly not needed at all.
 * @param[in,out]     box      The resulting C3D_Box object.
 * @param[in]         flag     The new sensor flag value.
 */
static inline void Box_SetSensorFlag(C3D_Box* box, bool flag)
{
	box->sensor = flag;
}

/**
 * @brief Cast a ray
 * TODO: Check to see what this function really does.
 * @note Randy Gaul: The entire function performs box to ray and finds the hit point. Using the transpose lets one solve ray to AABB and still get the correct results. Ray to AABB is easier than ray to OBB.
 */
bool Box_Raycast(C3D_Box* box, const C3D_Transform* transform, C3D_RaycastData* raycastData)
{
	C3D_Transform worldTransform;
	Transform_Multiply(&worldTransform, transform, &box->localTransform);
	C3D_FVec direction;
	Transform_MultiplyTransposeFVec(&direction, &worldTransform.rotation, &raycastData->direction);
	C3D_FVec position;
	Transform_MultiplyTransformFVec(&position, &worldTransform, &raycastData->rayOrigin);
	float minimumTime = 0.0f;
	float maximumTime = raycastData->endPointTime;
	float time0Value;  //Point at t = 0 (unit)
	float time1Value;  //Point at t = 1 (unit)
	C3D_FVec normal0;
	for (int i = 3; i > 0; i--)
	{
		//C3D_FVec is structured as WZYX, so the index goes from 3 -> 2 -> 1 -> break.
		if (fabsf(direction.c[i]) < FLT_EPSILON)
		{
			if (position.c[i] < -box->extent.c[i] || position.c[i] > box->extent.c[i])
				return false;
		}
		else 
		{
			float inverseDirection = 1.0f / direction.c[i];
			float sign = (direction.c[i] >= 0.0f ? 1.0f : -1.0f);
			float extentValue = box->extent.c[i] * sign;
			C3D_FVec normal = {};
			normal.c[i] = -sign;
			
			time0Value = -(extentValue + position.c[i]) * inverseDirection;
			time1Value = (extentValue - position.c[i]) * inverseDirection;
			if (time0Value > minimumTime)
			{
				normal0 = normal;
				minimumTime = time0Value;
			}
			maximumTime = (maximumTime < time1Value ? maximumTime : time1Value);
			if (minimumTime > maximumTime)
				return false;
		}
	}
	raycastData->normal = Mtx_MultiplyFVec3(&worldTransform.rotation, normal0);
	raycastData->timeOfImpact = minimumTime;
	return true;
}

/**
 * @brief Using the given C3D_Box object data, create a C3D_AABB object that encapsulate the C3D_Box data.
 * @param[out]     aabb           The resulting C3D_AABB object from the C3D_Box data.
 * @param[in]      box            The C3D_Box object to derive C3D_AABB object from.
 * @param[in]      transform      The C3D_Transform object's transform properties to convert from local transform space to world transform space.
 */
void Box_ComputeAABB(C3D_AABB* aabb, C3D_Box* box, const C3D_Transform* transform)
{
	C3D_Transform worldTransform;
	Transform_Multiply(&worldTransform, transform, &box->localTransform);
	C3D_FVec vectors[8] = 
	{
		FVec3_New(-box->extent.x, -box->extent.y, -box->extent.z),
		FVec3_New(-box->extent.x, -box->extent.y,  box->extent.z),
		FVec3_New(-box->extent.x,  box->extent.y, -box->extent.z),
		FVec3_New(-box->extent.x,  box->extent.y,  box->extent.z),
		FVec3_New( box->extent.x, -box->extent.y, -box->extent.z),
		FVec3_New( box->extent.x, -box->extent.y,  box->extent.z),
		FVec3_New( box->extent.x,  box->extent.y, -box->extent.z),
		FVec3_New( box->extent.x,  box->extent.y,  box->extent.z)
	};
	for (int i = 0; i < 8; i++)
		Transform_MultiplyTransformFVec((vectors + i), &worldTransform, (vectors + i));
	C3D_FVec minimum = FVec3_New(FLT_MAX, FLT_MAX, FLT_MAX);
	C3D_FVec maximum = FVec3_New(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (int i = 0; i < 8; i++)
	{
		minimum = FVec3_Min(minimum, vectors[i]);
		maximum = FVec3_Max(maximum, vectors[i]);
	}
	aabb->min = minimum;
	aabb->max = maximum;
}

/**************************************************
 * Physics Body Functions.
 **************************************************/

typedef enum C3D_BodyType 
{
	StaticBody,
	DynamicBody,
	KinematicBody
} C3D_BodyType;

typedef enum C3D_BodyFlag 
{
	Awake         = 0x001,
	Active        = 0x002,
	AllowSleep    = 0x004,
	BodyIsland    = 0x010,
	Static        = 0x020,
	Dynamic       = 0x040,
	Kinematic     = 0x080,
	LockAxisX     = 0x100,
	LockAxisY     = 0x200,
	LockAxisZ     = 0x400,
} C3D_BodyFlag;

typedef struct C3D_Body 
{
	C3D_Mtx inverseInertiaModel;
	C3D_Mtx inverseInertiaWorld;
	float mass;
	float inverseMass;
	C3D_FVec linearVelocity;
	C3D_FVec angularVelocity;
	C3D_FVec force;
	C3D_FVec torque;
	struct C3D_Transform transform;
	C3D_FQuat quaternion;
	C3D_FVec localCenter;
	C3D_FVec worldCenter;
	float sleepTime;
	float gravityScale;
	float layers;
	unsigned int flags;
	struct C3D_Box* boxes;
	void* userData;
	struct C3D_Scene* scene;
	struct C3D_Body* next;
	struct C3D_Body* previous;
	unsigned int islandIndex;
	float linearDamping;
	float angularDamping;
	struct C3D_ContactEdge* contactList;
} C3D_Body;


/**************************************************
 * Contact Functions
 **************************************************/

/**
 *  The closest pair of features between two objects (a feature is either a vertex or an edge). 
 *  in stands for "incoming"
 *  out stands for "outgoing"
 *  I stands for "incident"
 *  R stands for "reference"
 *  See Dirk Gregorius GDC 2015 on creating contacts for more details. (Physics for Game Programmers: Robust Contact Creation for Physics Simulations)
 *  
 *  Each feature pair is used to cache solutions from one physics tick to another. This is
 *  called warmstarting, and lets boxes stack and stay stable. Feature pairs identify points
 *  of contact over multiple physics ticks. Each feature pair is the junction of an incoming
 *  feature and an outgoing feature, usually a result of clipping routines. The exact info
 *  stored in the feature pair can be arbitrary as long as the result is a unique ID for a
 *  given intersecting configuration.
 */
typedef union C3D_FeaturePair 
{
	struct 
	{
		u8 incomingReference;
		u8 outgoingReference;
		u8 incomingIncident;
		u8 outgoingIncident;
	};
	unsigned int key;
} C3D_FeaturePair;

typedef struct C3D_Contact 
{
	C3D_FVec position;               //World coordinate contact position
	float penetration;               //Depth of penetration from collision
	float normalImpulse;             //Accumulated normal impulse.
	float tangentImpulse[2];         //Accumulated friction impulse. Tangent, because it's the opposite direction.
	float bias;                      //Restitution + Baumgarte Stabilization.
	float normalMass;                //Normal constraint mass.
	float tangentMass[2];            //Tangent constraint mass.
	C3D_FeaturePair featurePair;     //Features on A and B for this contact position.
	u8 warmStarted;                  //Used for debug rendering.
} C3D_Contact;

typedef struct C3D_Manifold
{
	struct C3D_Box* A;
	struct C3D_Box* B;
	C3D_FVec normal;
	C3D_FVec tangentVectors[2];
	struct C3D_Contact contacts[8];
	unsigned int contactsCount;
	struct C3D_Manifold* next;
	struct C3D_Manifold* previous;
	bool sensor;
} C3D_Manifold;

typedef enum C3D_ContactConstraintFlag 
{
	Colliding        = 0x00000001,
	WasColliding     = 0x00000002,
	ConstraintIsland = 0x00000004,
} C3D_ContactConstraintFlag;

typedef struct C3D_ContactEdge 
{
	struct C3D_Body* other;
	struct C3D_ContactConstraint* constraint;
	struct C3D_ContactEdge* next;
	struct C3D_ContactEdge* previous;
} C3D_ContactEdge;

typedef struct C3D_ContactConstraint 
{
	struct C3D_Box* A;
	struct C3D_Box* B;
	struct C3D_Body* bodyA;
	struct C3D_Body* bodyB;
	struct C3D_ContactEdge edgeA;
	struct C3D_ContactEdge edgeB;
	struct C3D_ContactConstraint* next;
	struct C3D_ContactConstraint* previous;
	float friction;
	float restitution;
	struct C3D_Manifold manifold;
	unsigned int flags;
} C3D_ContactConstraint;


/**************************************************
 * Scene Functions
 **************************************************/

typedef struct C3D_ContactManager 
{
	
} C3D_ContactManager;



typedef struct C3D_Scene 
{
	C3D_ContactManager contactManager;
	
} C3D_Scene;
