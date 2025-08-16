#include "TreeBuilder.h"
#include "BaseNode.h"
#include "FightingTree.h"
#include "BehaviorTreeFactory.h"

BehaviorResult TryExitFighting::tick(AIstate& state, AIContext& context)
{
	if (context.getAttack()->attacking) {
		// std::cout << "should not exit fighting" << std::endl;
		return BehaviorResult::RUNNING;
	}
	else if (context.getSkill()->usingSkill) {
		return BehaviorResult::RUNNING;
	}
	else if (context.getDist() < 100.f) {
		return BehaviorResult::FAILURE;
	}
	// state.fighting = false;
	std::cout << "should exit fighting" << std::endl;
	return BehaviorResult::SUCCESS;
}

BehaviorResult SetFighingState::tick(AIstate& state, AIContext& context)
{
	state.fighting = true;
	context.getMovement()->stop();

	// std::cout << "enter fighting" << std::endl;

	return BehaviorResult::SUCCESS;
}

BehaviorResult SetExitFightingState::tick(AIstate& state, AIContext& context)
{
	state.fighting = false;
	context.getMovement()->setToDefaultSpeed();

	// std::cout << "exit fighting" << std::endl;

	return BehaviorResult::SUCCESS;
}

BehaviorResult TryGoHome::tick( AIContext& context )
{
	Vector2D defaultPos = context.getDefaultPos();
	auto dist = context.getTransform()->position.distanceTo(defaultPos);
	if (dist < 80.f) return BehaviorResult::FAILURE;
	return BehaviorResult::SUCCESS;
}

BehaviorResult GoHome::tick( AIContext& context )
{
	Vector2D defaultPos = context.getDefaultPos();
	auto dist = context.getTransform()->position.distanceTo(defaultPos);

	if (context.getDist() < 100.f) return BehaviorResult::FAILURE;
	else if (dist < 80.f) return BehaviorResult::SUCCESS;

	context.getMovement()->setToDefaultSpeed();

	context.getMovement()->setDirection((defaultPos - context.getTransform()->position).normalize());

	if (context.getMovement()->getDirection().x >= 0) { context.getSprite()->setAnimate("walkR"); }
	else { context.getSprite()->setAnimate("walkL"); }
	// std::cout << "backing" << context.getTransform()->position.distanceTo(defaultPos) << std::endl;
	return BehaviorResult::RUNNING;
}

BehaviorTree<AIstate&, AIContext&> BehaviorTreeFactory::buildMasterTree()
{
	FightingTreeFactory fightingTreeFactory;

	BehaviorTree<AIstate&, AIContext&> tree = BehaviorTree<AIstate&, AIContext&>();

	tree.setRoot(TreeBuilder::FallbackNodeList(
		// ===== Fighting sequence =====
		TreeBuilder::SequenceNodeList(
			// ==== Sequence of enter fighting ====
			TreeBuilder::SequenceNodeList(
				// try get in fighting state
				TreeBuilder::FallbackNodeList(
					// if is fighting continous, Success
					TreeBuilder::PeelState(std::make_unique<IsFighting>()),
					// if not try get in fighting state
					TreeBuilder::PeelContext(std::make_unique<TryFighting>())
				),

				// if into fighting state, execute fighting tree
				std::make_unique<SetFighingState>(),					// set fighting state
				std::move(fightingTreeFactory.buildFightingTree())		// run fighting tree
			),

			// ==== Sequence of exit fighting ====
			TreeBuilder::SequenceNodeList(
				// if can't fighting, the fighting flow end. Run the following behavior
				std::make_unique<TryExitFighting>(),					// Try exit fighting mode
				std::make_unique<SetExitFightingState>()				// set exit fighting state
			)
		),

		TreeBuilder::SequenceNodeList(
			TreeBuilder::PeelContext(std::make_unique<TryTracking>()),
			TreeBuilder::PeelContext(std::make_unique<Tracking>())
		),

		TreeBuilder::SequenceNodeList(
			TreeBuilder::PeelContext(std::make_unique<TryGoHome>()),
			TreeBuilder::PeelContext(std::make_unique<GoHome>())
		),

		TreeBuilder::PeelContext(std::make_unique<Patrol>())
	));

	return tree;
}
