import pandas as pd
import networkx as nx
import random

# read csv files
movements_df = pd.read_csv("/home/vsicard/Documents/POSTDOC-INSERM/DATA/cattle_data//movements.csv")
attributes_df = pd.read_csv("/home/vsicard/Documents/POSTDOC-INSERM/DATA/cattle_data/attributes.csv", low_memory=False)

# Create empty graph
G = nx.MultiGraph()

# create nodes
print("create graph G")
for _, row in attributes_df.iterrows():
    node = row['node']
    attributes = {
        'ID': row['ID'],
        'species': row['species'],
        'longitude': row['longitude'],
        'latitude': row['latitude'],
        'istat': row['istat'],
        'ptype': row['ptype'],
        'province': row['province'],
        'region': row['region'],
        'region_name': row['region_name'],
        'state': 'S'
    }
    G.add_node(node, **attributes)

# add edges with attributes
for _, row in movements_df.iterrows():
    source = row['i']
    target = row['j']
    weight = row['w']
    time = row['t']
    G.add_edge(source, target, t=time, weight=weight)


# G = nx.read_gpickle("Graph.gpickle")
print('number of nodes:', G.number_of_nodes())
print('number of edges:', G.number_of_edges())

nx.write_gpickle(G, "Graph.gpickle")

# # only one t graph
#
# # Extraction du sous-graphe à t = 0
# print("create graph G_t0")
# G_t0 = nx.edge_subgraph(G, [(u, v, attr) for u, v, attr in G.edges(data='t', default=0) if attr == 0])
# G_t0.nodes = G.nodes
#
# print(G_t0.number_of_edges())
#
# nx.write_gpickle(G_t0, "Graph_t0.gpickle")

# ii = 0
# for node in G.nodes:
#     if ii < 5:
#         print(G.nodes[node]['state'], list(G.neighbors(node)))
#     else:
#         G.nodes[node]['state'] = 'I'
#         print(G.nodes[node]['state'])
#         break
#     ii += 1

# select a node randomly
random_node = random.choice(list(G.nodes))
G.nodes[random_node]['state'] = 'I'
print("infected node : ", random_node)

# SIR
max_time = 5

beta = 0.3
gamma = 0.1

for t in range(max_time):
    print("T=", t)
    S = []
    I = []
    R = []
    for node in G.nodes():
        if G.nodes[node]["state"] == 'I':
            for neighbor in G.neighbors(node):
                if G.nodes[neighbor]['state'] == 'S':
                    if random.random() < beta:
                        I.append(neighbor)
                        # print("Node", neighbor, 'Infected')
            if random.random() < gamma:
                R.append(node)
                # print("Node", node, "Recovered")
    # Maj states
    for node in I:
        G.nodes[node]['state'] = "I"
    for node in R:
        G.nodes[node]['state'] = "R"
        
    print("\t Infected", len(I))
    print("\t Recovered", len(R))


