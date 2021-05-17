#ifndef INCLUDED_KLINK_AI_GRAPHSEARCH_H
#define INCLUDED_KLINK_AI_GRAPHSEARCH_H

#include "Graph.h"
#include "Graphics/Inc/SimpleDraw.h"

using namespace Klink::AI;

namespace Klink::AI::GraphSearch
{
	struct GraphSearchContext
	{
		size_t nodeCount;

		std::list<size_t> openList;
		std::list<size_t> closedList;

		std::vector<size_t> parent;
		std::vector<size_t> checked;

		std::vector<bool> open;
		std::vector<bool> closed;
		std::vector<float> g;
		std::vector<float> h;

		size_t startNode = InvalidNode;
		size_t endNode = InvalidNode;
		bool found = false;

		int nodeCompares = 0;

		void Init(size_t nodes)
		{
			nodeCount = nodes;
			nodeCompares = 0;
		}

		void Reset()
		{
			// Clear and reset all lists and variables
			parent.clear();
			openList.clear();
			closedList.clear();
			open.clear();
			closed.clear();
			checked.clear();
			g.clear();
			g.resize(nodeCount, 0.0f);
			h.clear();
			h.resize(nodeCount, 0.0f);
			startNode = InvalidNode;
			endNode = InvalidNode;
			found = false;
			nodeCompares = 0;


			// Reserve memory for lists
			checked.resize(0);
			parent.resize(nodeCount, InvalidNode);
			open.resize(nodeCount, false);
			closed.resize(nodeCount , false);
		}
	};	// struct GraphSearchContext

	template <typename GraphType>
	struct CanOpenNode
	{
		bool operator()(const GraphType& graph, GraphSearchContext& context, size_t node)
		{
			if (context.open[node])	//	Check to see if open, if open, early out
			{
				return false;
			}
			if (graph.IsWall(node))
			{
				return false;
			}

			return true;
		}
	};

	template <typename GraphType, typename OpenPolicy, typename NextPolicy, typename ValidationPolicy>
	struct BaseSearch
	{
		BaseSearch(const GraphType& graph)
			:graph(graph)
		{}

		void Execute(GraphSearchContext& context)
		{
			if (!context.open[context.startNode])							// If the start node is not on the open list...
			{
				Open(graph, context, AI::InvalidNode, context.startNode);			// Use our open policy to open the first node, setting invalid node as the parent
			}

			while (!context.found && !context.openList.empty())				// Check to make sure we haven't already found it and make sure we have nodes to actually check
			{
				size_t nodeIndex = Next(context);							// Use our next policy to determine which node is next

				++context.nodeCompares;
				context.checked.push_back(nodeIndex);
				if (nodeIndex == context.endNode)							// The actual search - if true, we've found our node
				{
					context.found = true;
				}
				else // node search
				{
					auto& node = graph.GetNode(nodeIndex);					// grab the next node
					for (auto& neighbourNode : node.neighbours)				// find its neighbours
					{
						if (IsValid(graph, context, neighbourNode))				// if the neighbour is not already on the openList...
						{
							Open(graph, context, nodeIndex, neighbourNode);		// ...add it so we can search later
						}
					}
				}

				context.closedList.push_back(nodeIndex);					// Since the node we looked at was not the one we were looking for, add it to the closed list
				context.closed[nodeIndex] = true;
			}
		} // void Execute()

		const GraphType& graph;
		OpenPolicy Open;
		NextPolicy Next;
		ValidationPolicy IsValid;

	}; // struct BaseSearch
	
	template <typename GraphType>
	struct PushBackPolicy
	{
		void operator()(const GraphType& graph, GraphSearchContext& context, size_t parentIndex, size_t nodeIndex)
		{
			context.openList.push_back(nodeIndex);		// Put node into open list
			context.open[nodeIndex] = true;				// Set the open flag for this node to true
			context.parent[nodeIndex] = parentIndex;	// Set the parent of GetNode(nodeIndex) to parentIndex (saved from earlier in loop) - parent is a separate list
		}
	};

	struct PopFrontPolicy
	{
		size_t operator()(GraphSearchContext& context)
		{
			auto next = context.openList.front();
			context.openList.pop_front();
			return next;
		}
	};

	struct PopBackPolicy
	{
		size_t operator()(GraphSearchContext& context)
		{
			auto next = context.openList.back();
			context.openList.pop_back();
			return next;
		}
	};

	template <typename GraphType, typename CostPolicy>
	struct InsertByLowestGCost
	{
		void operator()(const GraphType& graph, GraphSearchContext& context, size_t parentIndex, size_t nodeIndex)
		{
			// First node
			if (parentIndex == InvalidNode)
			{
				context.g[nodeIndex] = 0.0f;
				context.openList.push_back(nodeIndex);
				context.open[nodeIndex] = true;
				context.parent[nodeIndex] = parentIndex;
				return;
			}

			float g = context.g[parentIndex] + Cost(graph, context, parentIndex, nodeIndex);

			// Second node (Combine?)
			context.open[nodeIndex] = true;
			context.parent[nodeIndex] = parentIndex;
			context.g[nodeIndex] = g;
			if (context.openList.size() == 0)
			{
				context.openList.push_back(nodeIndex);
				return;
			}

			// Other nodes
			std::list<std::size_t>::iterator emplaceIterator = context.openList.begin();
			for (auto i : context.openList)
			{
				// If the currents node cost is greater than the currently checked openlist node's g cost, insert here. Smallest g cost in the back
				if (g >= context.g[i])
				{
					// use std::next to advance the iterator to the position in the vector that i refers to
					context.openList.emplace(emplaceIterator, nodeIndex);
					return;
				}
				emplaceIterator++;
			}
			context.openList.emplace_front(nodeIndex);
		}

		CostPolicy Cost;
	};


	template <typename GraphType, typename CostPolicy, typename HCostPolicy>
	struct InsertByLowestHCost
	{
		void operator()(const GraphType& graph, GraphSearchContext& context, size_t parentIndex, size_t nodeIndex)
		{
			// First node
			if (parentIndex == InvalidNode)
			{
				context.g[nodeIndex] = 0.0f;
				context.openList.push_back(nodeIndex);
				context.open[nodeIndex] = true;
				context.parent[nodeIndex] = parentIndex;
				return;
			}

			float g = context.g[parentIndex] + Cost(graph, context, parentIndex, nodeIndex);
			// H is an independant value based on node
			float h = HCost(graph, context, parentIndex, nodeIndex);

			// Second node (Combine?)
			context.open[nodeIndex] = true;
			context.parent[nodeIndex] = parentIndex;
			context.g[nodeIndex] = g;

			context.h[nodeIndex] = h;
			if (context.openList.size() == 0)
			{
				context.openList.push_back(nodeIndex);
				return;
			}

			// Other nodes
			std::list<std::size_t>::iterator emplaceIterator = context.openList.begin();
			for (auto i : context.openList)
			{
				// If the currents node cost is greater than the currently checked openlist node's g cost, insert here. Smallest g cost in the back
				if (h >= context.h[i])
				{
					// use std::next to advance the iterator to the position in the vector that i refers to
					context.openList.emplace(emplaceIterator, nodeIndex);
					return;
				}
				emplaceIterator++;
			}
			context.openList.emplace_back(nodeIndex);
		}

		CostPolicy Cost;
		HCostPolicy HCost;
	};


	template <typename GraphType>
	struct PixelDistanceCostPolicy
	{
		float operator()(const GraphType& graph, GraphSearchContext& context, size_t parentIndex, size_t nodeIndex)
		{
			return JMath::Distance({graph.GetNode(parentIndex).position.x, graph.GetNode(parentIndex).position.y, 0.0f }, { graph.GetNode(nodeIndex).position.x , graph.GetNode(nodeIndex).position.y , 0.0f});
		}
	};

	template <typename GraphType>
	struct EuclideanHeruristic
	{
		float operator()(const GraphType& graph, GraphSearchContext& context, size_t parentIndex, size_t nodeIndex)
		{
			// return actual pixel distance from this node to the end node
			return JMath::Distance({ graph.GetNode(nodeIndex).position.x, graph.GetNode(nodeIndex).position.y, 0.0f }, { graph.GetNode(context.endNode).position.x, graph.GetNode(context.endNode).position.y, 0.0f });
		}
	};

template <typename GraphType, typename ValidationPolicy>
using BFS = BaseSearch<GraphType, PushBackPolicy<GraphType>, PopFrontPolicy, ValidationPolicy>;

template <typename GraphType, typename ValidationPolicy>
using DFS = BaseSearch<GraphType, PushBackPolicy<GraphType>, PopBackPolicy, ValidationPolicy>;

template <typename GraphType, typename ValidationPolicy, typename CostPolicy>
using Dijkstra = BaseSearch<GraphType, InsertByLowestGCost<GraphType, PixelDistanceCostPolicy<GraphType>>, PopBackPolicy, ValidationPolicy>;

template <typename GraphType, typename ValidationPolicy, typename CostPolicy>
using AStar = BaseSearch<GraphType, InsertByLowestHCost<GraphType, PixelDistanceCostPolicy<GraphType>, EuclideanHeruristic<GraphType>>, PopBackPolicy, ValidationPolicy>;

} // namespace AI::GraphSearch

#endif // INCLUDED_KLINK_AI_GRAPHSEARCH_H


/*

Policy Programming:

Using a skeleton structure (like our node search), we can replace certain functionality with functors in order to facilitate swap outs of functionality based on how you want a
program to behave


Homework:

graph
- build a button for recalculating after resize

draw closed list
- show history of searching with drawlines
draw path
- show actual path found

Depth-First Search policies

*/