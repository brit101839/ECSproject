#pragma once

#include "Quadtree/Quadtree.h"
#include "Quadtree/Box.h"
#include "ECS/TransformComponent.h"

using namespace quadtree;

class RenderQuadtreeManager
{
private:

	struct GetBox {
		Box<float> operator()(TransformComponent* trans) const {
			return trans->getBox();
		}
	};

	using RenderTree = Quadtree<TransformComponent*, GetBox>;
	RenderTree rendertree;

public:

	RenderQuadtreeManager(Box<float> interBox)
		:rendertree(interBox)
	{
	}

	void addTransfrom(TransformComponent* trans) {
		rendertree.add(trans);
	}

	void updateTrasform(TransformComponent* trans) {
		rendertree.remove(trans);
		rendertree.add(trans);
	}

	std::vector<TransformComponent*> query(const Box<float>& area) {
		
		return rendertree.query(area);
	}
};

