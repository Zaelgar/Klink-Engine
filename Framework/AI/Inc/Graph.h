#ifndef INCLUDED_KLINK_AI_GRAPH_H
#define INCLUDED_KLINK_AI_GRAPH_H

#include "Common.h"

namespace Klink::AI
{
	struct Node
	{
		std::vector<size_t> neighbours;
		bool isNodeConnected = true;
	};

	constexpr size_t InvalidNode = std::numeric_limits<size_t>::max();

	template <typename NodeType>
	class Graph
	{
	public:

		void Initialise(int mWidth, int height, int marginPadding, int cellPadding, float radius, bool hasClockwiseNeighbours);

		bool DisconnectNode(size_t nodeToDisconnect);
		void ReconnectNode(size_t nodeToConnect);

		NodeType& GetNode(size_t index);
		const NodeType& GetNode(int index) const;

		int GetNodeCount() { return mNodeCount; }
		int GetWidth() { return mWidth; }
		int GetHeight() { return mHeight; }

		const bool IsWall(size_t node) const;

		void DisconnectAtPosition(int pixelPositionX, int pixelPositionY);
		//void ReconnectAtPosition(int pixelPositionX, int pixelPositionY);

		std::vector<size_t>& GetDisconnected() { return mDisconnectedNodes; }

	private:
		int mNodeCount;
		std::vector<NodeType> mNodes;
		std::vector<size_t> mDisconnectedNodes;

		int mWidth;
		int mHeight;
		int mMarginPadding;
		int mCellPadding;
		float mRadius;
		bool mHasClockwiseNeighbours;
	};

	struct MapNode : public Klink::AI::Node
	{
		JMath::Vector2 position;
	};

	using NavGraph = Klink::AI::Graph<MapNode>;

	template<typename NodeType>
	inline void Graph<NodeType>::Initialise(int width, int height, int marginPadding, int cellPadding, float radius, bool hasClockwiseNeighbours)
	{
		mNodes.clear();
		mNodeCount = width * height;
		mNodes.resize(mNodeCount);

		mDisconnectedNodes.clear();

		mWidth = width;
		mHeight = height;
		mMarginPadding = marginPadding;
		mCellPadding = cellPadding;
		mRadius = radius;
		mHasClockwiseNeighbours = hasClockwiseNeighbours;
	}

	template<typename NodeType>
	inline const bool Graph<NodeType>::IsWall(std::size_t node) const
	{
		for (std::vector<std::size_t>::const_iterator iter = mDisconnectedNodes.begin(); iter != mDisconnectedNodes.end(); ++iter)
		{
			if (*iter == node)
			{
				return true;
			}
		}
		return false;
	}

	template<typename NodeType>
	void Graph<NodeType>::DisconnectAtPosition(int pixelPositionX, int pixelPositionY)
	{
		
	}

	template<typename NodeType>
	bool Graph<NodeType>::DisconnectNode(size_t nodeToDisconnect)
	{
		for (std::vector<std::size_t>::iterator iter = mDisconnectedNodes.begin(); iter != mDisconnectedNodes.end(); ++iter)
		{
			if (*iter == nodeToDisconnect)
			{
				return false;
			}
		}

		// Bounds checking
		if (nodeToDisconnect > mNodeCount || nodeToDisconnect < 0)
		{
			return false;
		}
		else
		{
			// for each neighbour node of the nodeToDisconnect...
			for (auto& currentNode : mNodes[nodeToDisconnect].neighbours)
			{
				// find its neighbour that is the nodeToDisconnect and...
				for (std::vector<std::size_t>::iterator neighbourNode = mNodes[currentNode].neighbours.begin(); neighbourNode != mNodes[currentNode].neighbours.end(); ++neighbourNode)
				{
					// remove it...
					if (*neighbourNode == nodeToDisconnect)
					{
						mNodes[currentNode].neighbours.erase(neighbourNode);
						break;
					}
				}
			}

			// then remove all of nodeToDisconnect's neighbours
			mNodes[nodeToDisconnect].isNodeConnected = false;
			mNodes[nodeToDisconnect].neighbours.clear();
			mDisconnectedNodes.push_back(nodeToDisconnect);
		}

		return true;
	}

	template<typename NodeType>
	void Graph<NodeType>::ReconnectNode(std::size_t nodeToConnectS)
	{
		int nodeToConnect = static_cast<int>(nodeToConnectS);
		std::vector<std::size_t>::iterator neighbourIterator;
		if (mHasClockwiseNeighbours)
		{
			// Top left corner (three neighbours)
			if (nodeToConnect == 0)
			{
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + 1);			// East neighbour
				mNodes[nodeToConnect + 1].neighbours.emplace(std::advance(mNodes[nodeToConnect+1].neighbours.begin(), 4), nodeToConnect);						//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth + 1);	// South-East neighbour
				mNodes[nodeToConnect + mWidth + 1].neighbours.emplace(std::advance(mNodes[nodeToConnect + mWidth + 1].neighbours.begin(), 7), nodeToConnect);	//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth);		// South neighbour
				mNodes[nodeToConnect + mWidth].neighbours.emplace(mNodes[nodeToConnect + mWidth].neighbours.begin(), nodeToConnect);									//
			}
			// Top right corner (three neighbours)
			else if (nodeToConnect + 1 == mWidth)
			{
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth);		// South neighbour
				mNodes[nodeToConnect + mWidth].neighbours.emplace(mNodes[nodeToConnect + mWidth].neighbours.begin(), nodeToConnect);									//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth - 1);	// South-West neighbour
				mNodes[nodeToConnect + mWidth - 1].neighbours.emplace(std::advance(mNodes[nodeToConnect + mWidth - 1].neighbours.begin(), 1), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - 1);			// West neighbour
				mNodes[nodeToConnect - 1].neighbours.emplace(mNodes[nodeToConnect - 1].neighbours.begin(), nodeToConnect);										//
			}
			// Bottom left corner (three neighbours)
			else if (nodeToConnect + mWidth == mNodeCount)
			{
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth);		// North neighbour
				mNodes[nodeToConnect - mWidth].neighbours.emplace(std::advance(mNodes[nodeToConnect - mWidth].neighbours.begin(), 4), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth + 1);	// North-East neighbour
				mNodes[nodeToConnect - mWidth + 1].neighbours.emplace(std::advance(mNodes[nodeToConnect - mWidth + 1].neighbours.begin(), 5), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + 1);			// East neighbour
				mNodes[nodeToConnect + 1].neighbours.emplace(std::advance(mNodes[nodeToConnect + 1].neighbours.begin(), 3), nodeToConnect);			//
			}
			// Bottom Right Corner (three neighbours)
			else if ((nodeToConnect + 1) == mNodeCount)
			{
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth);		// North neighbour
				mNodes[nodeToConnect - mWidth].neighbours.emplace(std::advance(mNodes[nodeToConnect - mWidth].neighbours.begin(), 1), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - 1);			// West neighbour
				mNodes[nodeToConnect - 1].neighbours.emplace(std::advance(mNodes[nodeToConnect - 1].neighbours.begin(), 2), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth - 1);	// North-West neighbour
				mNodes[nodeToConnect - mWidth - 1].neighbours.emplace(std::advance(mNodes[nodeToConnect - mWidth - 1].neighbours.begin(), 3), nodeToConnect);			//
			}
			// Top row (five neighbours)
			else if (nodeToConnect < mWidth)
			{
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + 1);			// East neighbour
				mNodes[nodeToConnect + 1].neighbours.emplace(std::advance(mNodes[nodeToConnect + 1].neighbours.begin(), 2), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth + 1);	// South-East neighbour
				mNodes[nodeToConnect + mWidth + 1].neighbours.emplace(std::advance(mNodes[nodeToConnect + mWidth + 1].neighbours.begin(), 4), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth);		// South neighbour
				mNodes[nodeToConnect + mWidth].neighbours.emplace(std::advance(mNodes[nodeToConnect + mWidth].neighbours.begin(), 4), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth - 1);	// South-West neighbour
				mNodes[nodeToConnect + mWidth - 1].neighbours.emplace(std::advance(mNodes[nodeToConnect + mWidth - 1].neighbours.begin(), 1), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - 1);			// West neighbour
				mNodes[nodeToConnect - 1].neighbours.emplace(mNodes[nodeToConnect - 1].neighbours.begin(), nodeToConnect);			//
			}
			// Left side (five neighbours)
			else if (nodeToConnect % mWidth == 0)
			{
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth);		// North neighbour
				mNodes[nodeToConnect - mWidth].neighbours.emplace(std::advance(mNodes[nodeToConnect - mWidth].neighbours.begin(), 2), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth + 1);	// North-East neighbour
				mNodes[nodeToConnect - mWidth + 1].neighbours.emplace(std::advance(mNodes[nodeToConnect - mWidth + 1].neighbours.begin(), 3), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + 1);			// East neighbour
				mNodes[nodeToConnect + 1].neighbours.emplace(std::advance(mNodes[nodeToConnect + 1].neighbours.begin(), 6), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth + 1);	// South-East neighbour
				mNodes[nodeToConnect + mWidth + 1].neighbours.emplace(std::advance(mNodes[nodeToConnect + mWidth + 1].neighbours.begin(), 7), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth);		// South neighbour
				mNodes[nodeToConnect + mWidth].neighbours.emplace(mNodes[nodeToConnect + mWidth].neighbours.begin(), nodeToConnect);			//
			}
			// Right side (five neighbours)
			else if ((nodeToConnect + 1) % mWidth == 0)
			{
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth);		// North neighbour
				mNodes[nodeToConnect - mWidth].neighbours.emplace(mNodes[nodeToConnect - mWidth].neighbours.begin(), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth);		// South neighbour
				mNodes[nodeToConnect + mWidth].neighbours.emplace(mNodes[nodeToConnect + mWidth].neighbours.begin(), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth - 1);	// South-West neighbour
				mNodes[nodeToConnect + mWidth - 1].neighbours.emplace(std::advance(mNodes[nodeToConnect + mWidth - 1].neighbours.begin(), 1), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - 1);			// West neighbour
				mNodes[nodeToConnect - 1].neighbours.emplace(std::advance(mNodes[nodeToConnect - 1].neighbours.begin(), 2), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth - 1);	// North-West neighbour
				mNodes[nodeToConnect - mWidth - 1].neighbours.emplace(std::advance(mNodes[nodeToConnect - mWidth - 1].neighbours.begin(), 1), nodeToConnect);			//
			}
			// Bottom side (five neighbours)
			else if (nodeToConnect + mWidth > mNodeCount)
			{
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth);		// North neighbour
				mNodes[nodeToConnect - mWidth].neighbours.emplace(std::advance(mNodes[nodeToConnect - mWidth].neighbours.begin(), 4), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth + 1);	// North-East neighbour
				mNodes[nodeToConnect - mWidth + 1].neighbours.emplace(std::advance(mNodes[nodeToConnect - mWidth + 1].neighbours.begin(), 1), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + 1);			// East neighbour
				mNodes[nodeToConnect + 1].neighbours.emplace(std::advance(mNodes[nodeToConnect + 1].neighbours.begin(), 1), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - 1);			// West neighbour
				mNodes[nodeToConnect - 1].neighbours.emplace(std::advance(mNodes[nodeToConnect - 1].neighbours.begin(), 2), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth - 1);	// North-West neighbour
				mNodes[nodeToConnect - mWidth - 1].neighbours.emplace(std::advance(mNodes[nodeToConnect - mWidth - 1].neighbours.begin(), 3), nodeToConnect);			//
			}
			// Center (eight neighbours)
			else
			{
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth);		// North neighbour
				mNodes[nodeToConnect - mWidth].neighbours.emplace(std::advance(mNodes[nodeToConnect - mWidth].neighbours.begin(), 2), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth + 1);	// North-East neighbour
				mNodes[nodeToConnect - mWidth].neighbours.emplace(std::advance(mNodes[nodeToConnect - mWidth].neighbours.begin(), 1), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + 1);			// East neighbour
				mNodes[nodeToConnect + 1].neighbours.emplace(std::advance(mNodes[nodeToConnect + 1].neighbours.begin(), 3), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth + 1);	// South-East neighbour
				mNodes[nodeToConnect + mWidth + 1].neighbours.emplace(std::advance(mNodes[nodeToConnect + mWidth + 1].neighbours.begin(), 2), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth);		// South neighbour
				mNodes[nodeToConnect + mWidth].neighbours.emplace(mNodes[nodeToConnect + mWidth].neighbours.begin(), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth - 1);	// South-West neighbour
				mNodes[nodeToConnect + mWidth - 1].neighbours.emplace(std::advance(mNodes[nodeToConnect + mWidth - 1].neighbours.begin(), 1), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - 1);			// West neighbour
				mNodes[nodeToConnect - 1].neighbours.emplace(std::advance(mNodes[nodeToConnect - 1].neighbours.begin(), 2), nodeToConnect);			//

				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth - 1);	// North-West neighbour
				mNodes[nodeToConnect - mWidth - 1].neighbours.emplace(std::advance(mNodes[nodeToConnect - mWidth - 1].neighbours.begin(), 1), nodeToConnect);			//
			}
			mNodes[nodeToConnect].isNodeConnected = true;
		}
		else
		{
			// Top left corner (three neighbours)
			if (nodeToConnect == 0)
			{
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth);		// South neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + 1);			// East neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth + 1);	// South-East neighbour
			}
			// Top right corner (three neighbours)
			else if (nodeToConnect + 1 == mWidth)
			{
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth);		// South neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - 1);			// West neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth - 1);	// South-West neighbour
			}
			// Bottom left corner (three neighbours)
			else if (nodeToConnect + mWidth == mNodeCount)
			{
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth);		// North neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + 1);			// East neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth + 1);	// North-East neighbour
			}
			// Bottom Right Corner (three neighbours)
			else if ((nodeToConnect + 1) == mNodeCount)
			{
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth);		// North neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - 1);			// West neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth - 1);	// North-West neighbour
			}
			// Top row (five neighbours)
			else if (nodeToConnect < mWidth)
			{
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth);		// South neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + 1);			// East neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - 1);			// West neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth - 1);	// South-West neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth + 1);	// South-East neighbour
			}
			// Left side (five neighbours)
			else if (nodeToConnect % mWidth == 0)
			{
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth);		// North neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth);		// South neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + 1);			// East neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth + 1);	// North-East neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth + 1);	// South-East neighbour
			}
			// Right side (five neighbours)
			else if ((nodeToConnect + 1) % mWidth == 0)
			{
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth);		// North neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth);		// South neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - 1);			// West neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth - 1);	// South-West neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth - 1);	// North-West neighbour
			}
			// Bottom side (five neighbours)
			else if (nodeToConnect + mWidth > mNodeCount)
			{
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth);		// North neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + 1);			// East neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - 1);			// West neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth + 1);	// North-East neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth - 1);	// North-West neighbour
			}
			// Center (eight neighbours)
			else
			{
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth);		// North neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth);		// South neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + 1);			// East neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - 1);			// West neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth + 1);	// North-East neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth - 1);	// South-West neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect - mWidth - 1);	// North-West neighbour
				mNodes[nodeToConnect].neighbours.push_back(nodeToConnect + mWidth + 1);	// South-East neighbour
			}

			mNodes[nodeToConnect].isNodeConnected = true;
		}
		
		for (std::vector<std::size_t>::iterator iter = mDisconnectedNodes.begin(); iter != mDisconnectedNodes.end(); ++iter)
		{
			if (*iter == nodeToConnectS)
			{
				mDisconnectedNodes.erase(iter);
			}
		}
	}

	template<typename NodeType>
	inline NodeType & Graph<NodeType>::GetNode(size_t index)
	{
		return mNodes[index];
	}

	template<typename NodeType>
	inline const NodeType & Graph<NodeType>::GetNode(int index) const
	{
		return mNodes[index];
	}

}; // namespace AI

#endif // INCLUDED_KLINK_AI_GRAPH