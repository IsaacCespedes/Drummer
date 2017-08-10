#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\gsm\physics\Physics.h"

class PhysicalProperties
{
protected:
	float buoyancy;
	float mass;
	bool  collidable;
	float coefficientOfRestitution;
	float x;
	float y;
	float z;
	float velocityX;
	float velocityY;
	float accelerationX;
	float accelerationY;

public:
	PhysicalProperties()	{accelerationX = 0; accelerationY = 0;}
	~PhysicalProperties()	{}

	// INLINED METHODS
	float getBuoyancy()						{ return buoyancy;				}
	float getMass()							{ return mass;					}
	float getCoefficientOfRestitution()		{ return coefficientOfRestitution;	}
	bool isCollidable()						{ return collidable;			}
	float getX()							{ return x;						}
	float getY()							{ return y;						}
	float getZ()							{ return z;						}
	float getVelocityX()					{ return velocityX;				}
	float getVelocityY()					{ return velocityY;				}
	float getAccelerationX()				{ return accelerationX;			}
	float getAccelerationY()				{ return accelerationY;			}
	void setBuoyancy(float initBuoyancy)	{ buoyancy = initBuoyancy;		}
	void setMass(float initMass)			{ mass = initMass;				}
	void setCofficientOfRestitution(float cR) { coefficientOfRestitution = cR; }
	void setCollidable(bool initCollidable)	{ collidable = initCollidable;	}
	void setX(float initX)					{ x = initX;					}
	void incX(float xToInc)					{ x += xToInc;					}
	void setY(float initY)					{ y = initY;					}
	void incY(float yToInc)					{ y += yToInc;					}
	void setZ(float initZ)					{ z = initZ;					}
	void capVelocity(Physics *physics)
	{
		float maxVelocity = physics->getMaxVelocity();
		if (calcTotalVelocity() > maxVelocity)
		{
			// LET'S CALCULATE THE ANGLE OF
			// THE CURRENT VELOCITY VECTOR
			float xyRatio = x/y;
			float initX = x;
			float initY = y;
			if (initX < 0)	initX *= -1;
			if (initY < 0)	initY *= -1;
			float tempX = sqrt(((maxVelocity*maxVelocity)*(xyRatio*xyRatio))/(1+(xyRatio*xyRatio)));
			float tempY = sqrt((maxVelocity*maxVelocity)/((xyRatio*xyRatio)+1));
			float xRatio = tempX/initX;
			float yRatio = tempY/initY;
			x *= xRatio;
			y *= yRatio;
		}
	}

	int round(float realNum)
	{
		return (int)floor(realNum + 0.5f);
	}

	void setPosition(float initX, float initY)
	{
		x = initX;
		y = initY;
	}

	void setVelocity(float initVx, float initVy)
	{
		velocityX = initVx;
		velocityY = initVy;
	}

	void setVelocitySafely(Physics *physics, float initVx, float initVy)
	{
		velocityX = initVx;			
		velocityY = initVy;
		//capVelocity(physics);
	}
	void incVelocity(Physics *physics, float incVx, float incVy)
	{ 
		velocityX += incVx;
		velocityY += incVy;
		//capVelocity(physics);
	}
	void setAccelerationX(float initAx)		{ accelerationX = initAx;		}
	void setAccelerationY(float initAy)		{ accelerationY = initAy;		}

	float calcTotalVelocity()
	{
		float xSquared = x * x;
		float ySquared = y * y;
		float totalVelocity = xSquared + ySquared;
		totalVelocity = sqrtf(totalVelocity);
		return totalVelocity;
	}
};