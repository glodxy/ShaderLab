//
// Created by glodxy on 2021/4/22.
//

#ifndef SHADERLAB_DEPENDENCYGRAPH_H
#define SHADERLAB_DEPENDENCYGRAPH_H

#include <cstdint>
#include <string>
#include <vector>
#include <map>

namespace GCore::RG{
    class DependencyGraph {
    public:
        DependencyGraph() noexcept;
        ~DependencyGraph() noexcept;
        DependencyGraph(const DependencyGraph&) noexcept = delete;
        DependencyGraph& operator=(const DependencyGraph&) noexcept = delete;

        using NodeID = uint32_t;

        class Node;

        /**
         * 边不可改变
         */
        struct Edge {
            //起始与终结点
            const NodeID from;
            const NodeID to;

            Edge(DependencyGraph& graph, Node* from, Node* to);


            Edge(Edge const& rhs) noexcept = delete;
            Edge& operator=(Edge const& rhs) noexcept = delete;
        };

        /**
         * A generic node
         */
        class Node {
        public:

            Node(DependencyGraph& graph) noexcept;

            Node(Node const&) noexcept = delete;
            Node& operator=(Node const&) noexcept = delete;

            Node(Node&&) noexcept = default;

            virtual ~Node() noexcept = default;


            NodeID GetId() const noexcept { return mId; }

            //让该节点为终结点，不会被裁剪掉
            void MakeTarget() noexcept;

            //判断是否是终结点
            bool IsTarget() const noexcept { return mRefCount >= TARGET; }

            //判断该节点是否被裁剪掉
            bool IsCulled() const noexcept { return mRefCount == 0; }

            //获取该节点的引用数
            uint32_t GetRefCount() const noexcept;

        public:

            virtual const std::string& GetName() const noexcept;
            //输出相关信息

//
//            //! output itself as a graphviz string
//            virtual utils::CString graphvizify() const noexcept;
//
//            //! output a graphviz color string for an Edge from this node
//            virtual utils::CString graphvizifyEdgeColor() const noexcept;

        private:
            friend class DependencyGraph;
            static const constexpr uint32_t TARGET = 0x80000000u;
            uint32_t mRefCount = 0;
            const NodeID mId;
        };

        using EdgeContainer = std::vector<Edge*>;
        using NodeContainer = std::vector<Node*>;

        //清除所有的节点和边（仅清除在该图内的引用）
        void Clear() noexcept;

        /** return the list of all edges */
        EdgeContainer const& GetEdges() const noexcept;

        /** return the list of all nodes */
        NodeContainer const& GetNodes() const noexcept;

        //获取某一结点的入边
        EdgeContainer GetIncomingEdges(Node const* node) const noexcept;

        //获取某一结点的出边
        EdgeContainer GetOutgoingEdges(Node const* node) const noexcept;

        //根据id获取节点
        Node const* GetNode(NodeID id) const noexcept;
        Node* GetNode(NodeID id) noexcept;

        //裁剪所有无用的边
        void Cull() noexcept;

        //判断裁剪后的边是否可用
        bool IsEdgeValid(Edge const* edge) const noexcept;


        //是否无环
        bool IsAcyclic() const noexcept;

    private:
        // 生成id的函数，id为该节点在Nodes容器中的键
        uint32_t GenerateNodeId() noexcept;
        //注册节点
        void RegisterNode(Node* node, NodeID id) noexcept;
        //注册边
        void Link(Edge* edge) noexcept;
        //检查有无环
        static bool IsAcyclicInternal(DependencyGraph& graph) noexcept;
        NodeContainer mNodes;
        EdgeContainer mEdges;
    };
}

#endif //SHADERLAB_DEPENDENCYGRAPH_H
