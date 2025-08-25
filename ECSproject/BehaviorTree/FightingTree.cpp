#include "TreeBuilder.h"
#include "BaseNode.h"
#include "FightingTree.h"

BehaviorResult CanUseSkill::tick(AIContext& context) 
{
	if (context.getBehavior() == AIPreferBehavior::NormalAttackOnly)
		return BehaviorResult::FAILURE;
	return BehaviorResult::SUCCESS;
}

BehaviorResult ShouldUseSkill::tick(AIContext& context) 
{
	if (context.getDist() > 100.0f) return BehaviorResult::SUCCESS;
	else if (context.getSkill()->getType() == SkillType::ParallelRange) {
		if (context.getPlayerPos().x - context.getTransform()->position.x < 200.f) return BehaviorResult::SUCCESS;
	}
	return BehaviorResult::FAILURE;
}

BehaviorResult CanUseParallelRangeSkill::tick(AIContext& context)
{
	if (context.getSkill()->getType() == SkillType::Range || context.getSkill()->getType() == SkillType::ParallelRange) {
		return BehaviorResult::SUCCESS;
	}
	else {
		return BehaviorResult::FAILURE;
	}
}

BehaviorResult UsingSkill::tick(AIContext& context)
{
	context.getMovement()->stop();
	context.getSkill()->UseSkill();
	return BehaviorResult::SUCCESS;
}

BehaviorResult CanNormalAttack::tick( AIContext& context )
{
	if (context.getBehavior() == AIPreferBehavior::SkillOnly)
		return BehaviorResult::FAILURE;
	return BehaviorResult::SUCCESS;
}

BehaviorResult ShouldNormalAttack::tick(AIContext& context)
{
	if (context.getDist() < 100.0f) return BehaviorResult::SUCCESS;
	return BehaviorResult::FAILURE;
}

BehaviorResult UseNormalAttack::tick(AIContext& context)
{
	if (context.getDist() > 100.0f) return BehaviorResult::SUCCESS;

	// context.getTransform()->setDirection(Vector2D(0.f, 0.f));
	if (context.getState()->isResting()) return BehaviorResult::FAILURE;

	BoundingBox box;
	if (context.getSprite()->getFlip()) box = { context.getTransform()->position - Vector2D(40.f, 0.f), context.getTransform()->width, context.getTransform()->height };
	else box = { context.getTransform()->position + Vector2D(40.f, 0.f), context.getTransform()->width, context.getTransform()->height };
	context.getAttack()->startAttack(box);

	// std::cout << "fighting" << std::endl;

	return BehaviorResult::RUNNING;
}

std::unique_ptr<BehaviorNode<AIstate&, AIContext&>> FightingTreeFactory::buildFightingTree(void)
{
	std::unique_ptr<FallbackNode<AIstate&, AIContext&>> rootNode = TreeBuilder::FallbackNodeList(
		TreeBuilder::SequenceNodeList(
			// Skill sequence
			TreeBuilder::PeelContext(std::make_unique<CanUseSkill>()),
			TreeBuilder::PeelContext(std::make_unique<ShouldUseSkill>()),
			TreeBuilder::PeelContext(std::make_unique<UsingSkill>())
		),
		TreeBuilder::SequenceNodeList(
			// Normal attack sequence
			TreeBuilder::PeelContext(std::make_unique<CanNormalAttack>()),
			TreeBuilder::PeelContext(std::make_unique<ShouldNormalAttack>()),
			TreeBuilder::PeelContext(std::make_unique<UseNormalAttack>())
		),
		TreeBuilder::SequenceNodeList(
			// Take a rest
			// TreeBuilder::PeelContext(std::make_unique<KeepDistance>()),
			TreeBuilder::PeelContext(std::make_unique<Patrol>() ),
			// TreeBuilder::PeelContext(std::make_unique<setExhaustion>()),
			TreeBuilder::PeelContext(std::make_unique<Exhaustion>())
		)
	);

	return rootNode;
}
