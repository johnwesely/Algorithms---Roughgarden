#include "weighted-undirected-graph.h"

// subroutines for creating and destroying unweighted directed graphs
void destroy_graph(graph_t* graph) {
    for (size_t i = 0; i < graph->vertex_count; ++i) {
        destroy_nodes(graph->vertices[i].edges.head);
        destroy_nodes(graph->vertices[i].pre_reqs.head);
    }

    free(graph->vertices);
    free(graph);
}

void append_edge(graph_t* graph, size_t v_id, size_t j_id) {
    // apppend edge to edge list
    node_t* edge = malloc(sizeof(node_t));
    edge->value = j_id;
    edge->next = graph->vertices[v_id].edges.head;
    graph->vertices[v_id].edges.head = edge;

    // append pre req to pre req list
    node_t* pre_req = malloc(sizeof(node_t)); pre_req->value = v_id;
    pre_req->next = graph->vertices[j_id].pre_reqs.head;
    graph->vertices[j_id].pre_reqs.head = pre_req;

}