//
// Created by glodxy on 2021/4/22.
//
#include "details/DependencyGraph.h"

#include <cassert>

namespace GCore::RG{
    DependencyGraph::DependencyGraph() noexcept {
        mNodes.reserve(8);
        mEdges.reserve(16);
    }

    DependencyGraph::~DependencyGraph() noexcept=default;

    uint32_t DependencyGraph::GenerateNodeId() noexcept {
        return mNodes.size();
    }

    void DependencyGraph::RegisterNode(GCore::RG::DependencyGraph::Node *node, GCore::RG::DependencyGraph::NodeID id) noexcept {
        assert(id == mNodes.size());
        mNodes.push_back(node);
    }

    void DependencyGraph::Link(GCore::RG::DependencyGraph::Edge *edge) noexcept {
        mEdges.push_back(edge);
    }

    DependencyGraph::NodeContainer const& DependencyGraph::GetNodes() const noexcept {
        return mNodes;
    }

    DependencyGraph::EdgeContainer const& DependencyGraph::GetEdges() const noexcept {
        return mEdges;
    }

    bool DependencyGraph::IsEdgeValid(const GCore::RG::DependencyGraph::Edge *edge) const noexcept {
        auto& nodes=mNodes;
        Node* begin=nodes[edge->from];
        Node* end=nodes[edge->to];
        return !begin->IsCulled()&&!end->IsCulled();
    }

    DependencyGraph::EdgeContainer DependencyGraph::GetIncomingEdges(const GCore::RG::DependencyGraph::Node *node) const  noexcept {
        //todo:更高效的获取入边
        EdgeContainer result;
        result.reserve(mEdges.size());
        NodeID const nodeId = node->GetId();
        for (auto* edge : mEdges) {
            if (edge->to == nodeId) {
                result.push_back(edge);
            }
        }
        return result;
    }

    DependencyGraph::EdgeContainer DependencyGraph::GetOutgoingEdges(const GCore::RG::DependencyGraph::Node *node) const noexcept {
        //todo:更高效的获取出边
        EdgeContainer result;
        result.reserve(mEdges.size());
        NodeID const nodeId = node->GetId();
        for (auto* edge : mEdges) {
            if (edge->from == nodeId) {
                result.push_back(edge);
            }
        }
        return result;
    }

    DependencyGraph::Node const* DependencyGraph::GetNode(DependencyGraph::NodeID id) const noexcept {
        return mNodes[id];
    }

    DependencyGraph::Node* DependencyGraph::GetNode(DependencyGraph::NodeID id) noexcept {
        return mNodes[id];
    }

    void DependencyGraph::Cull() noexcept {

        auto& nodes = mNodes;
        auto& edges = mEdges;

        // 根据边更新节点的引用次数
        for (Edge* const pEdge : edges) {
            Node* node = nodes[pEdge->from];
            node->mRefCount++;
        }

        // cull nodes with a 0 reference count
        NodeContainer stack;
        stack.reserve(nodes.size());
        for (Node* const pNode : nodes) {
            if (pNode->GetRefCount() == 0) {
                stack.push_back(pNode);
            }
        }
        while (!stack.empty()) {
            Node* const pNode = stack.back();
            stack.pop_back();
            EdgeContainer const& incoming = GetIncomingEdges(pNode);
            for (Edge* edge : incoming) {
                Node* pLinkedNode = GetNode(edge->from);
                if (--pLinkedNode->mRefCount == 0) {
                    stack.push_back(pLinkedNode);
                }
            }
        }
    }

    void DependencyGraph::Clear() noexcept {
        mEdges.clear();
        mNodes.clear();
    }


    bool DependencyGraph::IsAcyclic() const noexcept {
#ifndef NDEBUG
        // We work on a copy of the graph
        DependencyGraph graph;
        graph.mEdges = mEdges;
        graph.mNodes = mNodes;
        return DependencyGraph::IsAcyclicInternal(graph);
#else
        return true;
#endif
    }

    bool DependencyGraph::IsAcyclicInternal(DependencyGraph& graph) noexcept {
#ifndef NDEBUG
        while (!graph.mNodes.empty() && !graph.mEdges.empty()) {
            //查找我们是否存在叶子节点
            //遍历所有节点，再遍历所有边，看是否存在以该节点为起点的边，如果不存在则该节点为叶节点。
            // check if we have at lest one leaf (i.e. nodes that have incoming but no outgoing edges)
            auto pos = std::find_if(graph.mNodes.begin(), graph.mNodes.end(),
                                    [&graph](Node const* node) {
                                        auto pos = std::find_if(graph.mEdges.begin(), graph.mEdges.end(),
                                                                [node](Edge const* edge) {
                                                                    return edge->from == node->GetId();
                                                                });
                                        return pos == graph.mEdges.end();
                                    });

            //不存在叶节点，则该图为循环图
            if (pos == graph.mNodes.end()) {
                return false;   // cyclic
            }

            // 如果存在，则移除该叶节点相关的边
            auto last = std::remove_if(graph.mEdges.begin(), graph.mEdges.end(),
                                       [&pos](Edge const* edge) {
                                           return edge->to == (*pos)->GetId() || edge->from == (*pos)->GetId();
                                       });
            graph.mEdges.erase(last, graph.mEdges.end());

            // 移除该叶节点
            graph.mNodes.erase(pos);
        }
#endif
        return true; // acyclic
    }
//------------------------------------------------------------------
//节点类
    void DependencyGraph::Node::MakeTarget() noexcept {
        assert(mRefCount==0||mRefCount==TARGET);
        mRefCount=TARGET;
    }

    uint32_t DependencyGraph::Node::GetRefCount() const noexcept {
        //如果为终结点，返回1，否则返回引用数
        return (mRefCount & TARGET) ? 1u : mRefCount;
    }

    const std::string& DependencyGraph::Node::GetName() const noexcept {
        return "unknown";
    }

    DependencyGraph::Node::Node(DependencyGraph& graph) noexcept : mId(graph.GenerateNodeId()) {
        graph.RegisterNode(this, mId);
    }
}



