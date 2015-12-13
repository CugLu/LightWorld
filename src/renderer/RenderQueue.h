#ifndef __RENDERQUEUE_H__
#define	__RENDERQUEUE_H__ 
#include "../glutils.h"
class Material;

class RenderQueue
{
public:
	RenderQueue();
	~RenderQueue();

	void SetMaterial(Material* material);

	void Frame();
private:

	Material* _material;
};


#endif
