#ifndef INCLUDED_KLINK_AI_GRAPHUTIL_H
#define INCLUDED_KLINK_AI_GRAPHUTIL_H

#include "Common.h"
#include "Graph.h"

using namespace Klink::Graphics;
using namespace Klink::JMath;
using namespace Klink::AI::GraphSearch;
using namespace Klink::Core;

inline void BuildGraph(NavGraph& graph, size_t width, size_t height, int marginPadding, int cellPadding, float radius, bool hasClockwiseNeighbours = true)
{
	size_t size = width * height;

	//graph.Initialise(width, height, marginPadding, cellPadding, radius, hasClockwiseNeighbours);
	if (size == 0)
	{
		return;
	}
	else if (size == 1) // no neighbours, TODO: Position
	{
		graph.GetNode(static_cast<size_t>(0)).position.x = (float)marginPadding;
		graph.GetNode(static_cast<size_t>(0)).position.y = (float)marginPadding;
		return;
	}
	else if (hasClockwiseNeighbours)
	{
		// Set Neighbours
		for (size_t nodeIndex = 0; nodeIndex < size; nodeIndex++)
		{
			// Top left corner (three neighbours)
			if (nodeIndex == 0)
			{
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + 1);			// East neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width + 1);	// South-East neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width);		// South neighbour
			}
			// Top right corner (three neighbours)
			else if (nodeIndex + 1 == width)
			{
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width);		// South neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width - 1);	// South-West neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - 1);			// West neighbour
			}
			// Bottom left corner (three neighbours)
			else if (nodeIndex + width == size)
			{
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width);		// North neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width + 1);	// North-East neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + 1);			// East neighbour
			}
			// Bottom Right Corner (three neighbours)
			else if ((nodeIndex + 1) == size)
			{
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width);		// North neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - 1);			// West neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width - 1);	// North-West neighbour
			}
			// Top row (five neighbours)
			else if (nodeIndex < width)
			{
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + 1);			// East neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width + 1);	// South-East neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width);		// South neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width - 1);	// South-West neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - 1);			// West neighbour
			}
			// Left side (five neighbours)
			else if (nodeIndex % width == 0)
			{
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width);		// North neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width + 1);	// North-East neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + 1);			// East neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width + 1);	// South-East neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width);		// South neighbour
			}
			// Right side (five neighbours)
			else if ((nodeIndex + 1) % width == 0)
			{
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width);		// North neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width);		// South neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width - 1);	// South-West neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - 1);			// West neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width - 1);	// North-West neighbour
			}
			// Bottom side (five neighbours)
			else if (nodeIndex + width > size)
			{
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width);		// North neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width + 1);	// North-East neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + 1);			// East neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - 1);			// West neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width - 1);	// North-West neighbour
			}
			// Center (eight neighbours)
			else
			{
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width);		// North neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width + 1);	// North-East neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + 1);			// East neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width + 1);	// South-East neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width);		// South neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width - 1);	// South-West neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - 1);			// West neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width - 1);	// North-West neighbour
			}
		}
	}
	else
	{
		for (size_t nodeIndex = 0; nodeIndex < size; nodeIndex++)
		{
			// Top left corner (three neighbours)
			if (nodeIndex == 0)
			{
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width);		// South neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + 1);			// East neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width + 1);	// South-East neighbour
			}
			// Top right corner (three neighbours)
			else if (nodeIndex + 1 == width)
			{
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width);		// South neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - 1);			// West neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width - 1);	// South-West neighbour
			}
			// Bottom left corner (three neighbours)
			else if (nodeIndex + width == size)
			{
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width);		// North neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + 1);			// East neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width + 1);	// North-East neighbour
			}
			// Bottom Right Corner (three neighbours)
			else if ((nodeIndex + 1) == size)
			{
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width);		// North neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - 1);			// West neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width - 1);	// North-West neighbour
			}
			// Top row (five neighbours)
			else if (nodeIndex < width)
			{
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width);		// South neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + 1);			// East neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - 1);			// West neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width - 1);	// South-West neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width + 1);	// South-East neighbour
			}
			// Left side (five neighbours)
			else if (nodeIndex % width == 0)
			{
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width);		// North neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width);		// South neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + 1);			// East neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width + 1);	// North-East neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width + 1);	// South-East neighbour
			}
			// Right side (five neighbours)
			else if ((nodeIndex + 1) % width == 0)
			{
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width);		// North neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width);		// South neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - 1);			// West neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width - 1);	// South-West neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width - 1);	// North-West neighbour
			}
			// Bottom side (five neighbours)
			else if (nodeIndex + width > size)
			{
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width);		// North neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + 1);			// East neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - 1);			// West neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width + 1);	// North-East neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width - 1);	// North-West neighbour
			}
			// Center (eight neighbours)
			else
			{
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width);		// North neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width);		// South neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + 1);			// East neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - 1);			// West neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width + 1);	// North-East neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width - 1);	// South-West neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex - width - 1);	// North-West neighbour
				graph.GetNode(nodeIndex).neighbours.push_back(nodeIndex + width + 1);	// South-East neighbour
			}
		}
	}

	// Set positions
	int x = 0;
	int y = 0;

	for (size_t h = 0; h < height; h++)
	{
		y = (int)((marginPadding + radius) + (radius * 2.0f * h + (cellPadding * h)));
		for (size_t w = 0; w < width; w++)
		{
			x = (int)((marginPadding + radius) + (radius * 2.0f * w + (cellPadding * w)));

			graph.GetNode(static_cast<size_t>(w + (width * h))).position.x = (float)x;
			graph.GetNode(static_cast<size_t>(w + (width * h))).position.y = (float)y;
		}
	}

	for (std::vector<size_t>::iterator iter = graph.GetDisconnected().begin(); iter != graph.GetDisconnected().end(); ++iter)
	{
		graph.DisconnectNode(*iter);
	}
}

inline void ResizeGraph(NavGraph& graph, int width, int height)
{
	return;
}

inline void ClearGraph()
{
	return;
}

inline void ShowGraph(NavGraph& graph, float radius)
{
	int size = graph.GetNodeCount();

	for (int nodeIndex = 0; nodeIndex < size; ++nodeIndex)
	{
		MapNode currentNode = graph.GetNode(static_cast<size_t>(nodeIndex));
		if (currentNode.isNodeConnected)
		{
			SimpleDraw::DrawScreenCircle(currentNode.position, radius, { 0.3f, 0.3f, 0.3f, 1.0f });
			for (int neighbourIndex = 0; neighbourIndex < (int)currentNode.neighbours.size(); ++neighbourIndex)
			{
				if (graph.GetNode(static_cast<size_t>(neighbourIndex)).isNodeConnected)
				{
					SimpleDraw::DrawScreenLine(currentNode.position, graph.GetNode(currentNode.neighbours[static_cast<size_t>(neighbourIndex)]).position, { 0.1f, 0.2f, 0.1f, 1.0f });
				}
			}
		}
	}
}

inline bool ShowPath(NavGraph& graph, GraphSearchContext& context, std::list<size_t>& path, int pathsPerSecond, Timer& timer, bool reset, bool showSearchPath = true)
{
	static float timeSinceLastDraw = 0;
	timeSinceLastDraw += timer.GetDeltaTime();
	static int linesAllowed = 0;

	if (reset)
	{
		linesAllowed = 0;
		return false;
	}

	// If the increment to the next addition of a line is met, allow one more draw line
	if (timeSinceLastDraw >= (1000 / pathsPerSecond))
	{
		linesAllowed++;
		timeSinceLastDraw = 0;	// Reset draw line timer to zero
	}

	int lineDrawCount = 0; // Keep track of the lines we have drawn this time (each second we add pathsPerSecond more, and should draw all old lines again + any new lines)


	if (showSearchPath)
	{
		for (auto& node : context.checked)
		{
			SimpleDraw::DrawScreenCircle(graph.GetNode(node).position, 22.0f, Colours::Yellow);
			lineDrawCount++;
			if (lineDrawCount == linesAllowed)
				return false;
		}
	}

	float radius = 20.0f;	// from winmain
	size_t lastNode = Klink::AI::InvalidNode;
	for (auto& node : path)
	{
		SimpleDraw::DrawScreenCircle(graph.GetNode(static_cast<size_t>(node)).position, radius + 5.0f);
		if (lastNode != Klink::AI::InvalidNode)
		{
			SimpleDraw::DrawScreenLine({ graph.GetNode(static_cast<size_t>(lastNode)).position.x, graph.GetNode(lastNode).position.y }, { graph.GetNode(node).position.x, graph.GetNode(node).position.y });
		}
		lastNode = node;
	}

	if (lastNode == context.endNode)
	{
		return true;
	}
}

#endif // INCLUDED_AI_GRAPH_UTIL