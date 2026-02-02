#include "Utils.h"
#include "CRayCastResult.h"


RayCastResult RayCastResult::raycastOnColliders(Ray r, std::vector<Collider*>* p_colliders)
{
	RayCastResult res;
	glm::vec2 tInterval(0.01f, INFINITY);

	for (int i = 0; i < p_colliders->size(); i++)
	{
		RayCastResult rcr = ((*p_colliders)[i])->raycast(r, tInterval);
		if (rcr.hit)
		{
			res = rcr;
			tInterval.y = rcr.t;
		}
	}

	return res;

}