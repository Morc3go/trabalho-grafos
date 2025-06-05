#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_USERS 100
#define MAX_NAME_LENGTH 50

typedef struct Node {
    int user_id;
    struct Node* next;
} Node;

typedef struct User {
    char name[MAX_NAME_LENGTH];
    Node* friends;
} User;

typedef struct SocialGraph {
    User users[MAX_USERS];
    int num_users;
} SocialGraph;

typedef struct Queue {
    int items[MAX_USERS];
    int front;
    int rear;
} Queue;

void initialize_graph(SocialGraph* graph) {
    graph->num_users = 0;
    for (int i = 0; i < MAX_USERS; i++) {
        graph->users[i].friends = NULL;
    }
}

int add_user(SocialGraph* graph, const char* name) {
    if (graph->num_users >= MAX_USERS) {
        printf("Limite de usuarios atingido!\n");
        return -1;
    }
    
    strncpy(graph->users[graph->num_users].name, name, MAX_NAME_LENGTH);
    graph->users[graph->num_users].friends = NULL;
    return graph->num_users++;
}

void add_friendship(SocialGraph* graph, int user1, int user2) {
    if (user1 < 0 || user1 >= graph->num_users || 
        user2 < 0 || user2 >= graph->num_users) {
        printf("ID de usuario invalido!\n");
        return;
    }
    
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->user_id = user2;
    newNode->next = graph->users[user1].friends;
    graph->users[user1].friends = newNode;
    
    newNode = (Node*)malloc(sizeof(Node));
    newNode->user_id = user1;
    newNode->next = graph->users[user2].friends;
    graph->users[user2].friends = newNode;
}

void show_friends(SocialGraph* graph, int user_id) {
    if (user_id < 0 || user_id >= graph->num_users) {
        printf("ID de usuario invalido!\n");
        return;
    }
    
    printf("Amigos de %s:\n", graph->users[user_id].name);
    Node* current = graph->users[user_id].friends;
    while (current != NULL) {
        printf("- %s\n", graph->users[current->user_id].name);
        current = current->next;
    }
}

Queue* create_queue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = -1;
    q->rear = -1;
    return q;
}

bool is_empty(Queue* q) {
    return q->front == -1;
}

void enqueue(Queue* q, int value) {
    if (q->rear == MAX_USERS - 1) return;
    if (q->front == -1) q->front = 0;
    q->rear++;
    q->items[q->rear] = value;
}

int dequeue(Queue* q) {
    if (is_empty(q)) return -1;
    int item = q->items[q->front];
    q->front++;
    if (q->front > q->rear) {
        q->front = q->rear = -1;
    }
    return item;
}

void bfs(SocialGraph* graph, int start_user) {
    if (start_user < 0 || start_user >= graph->num_users) {
        printf("ID de usuario invalido!\n");
        return;
    }
    
    bool visited[MAX_USERS] = {false};
    Queue* q = create_queue();
    
    visited[start_user] = true;
    enqueue(q, start_user);
    
    printf("Explorando rede social de %s (BFS):\n", graph->users[start_user].name);
    
    while (!is_empty(q)) {
        int current_user = dequeue(q);
        printf("- Visitado: %s\n", graph->users[current_user].name);
        
        Node* temp = graph->users[current_user].friends;
        while (temp != NULL) {
            int neighbor = temp->user_id;
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                enqueue(q, neighbor);
            }
            temp = temp->next;
        }
    }
    free(q);
}

void dfs_util(SocialGraph* graph, int user_id, bool visited[]) {
    visited[user_id] = true;
    printf("- Visitado: %s\n", graph->users[user_id].name);
    
    Node* temp = graph->users[user_id].friends;
    while (temp != NULL) {
        int neighbor = temp->user_id;
        if (!visited[neighbor]) {
            dfs_util(graph, neighbor, visited);
        }
        temp = temp->next;
    }
}

void dfs(SocialGraph* graph, int start_user) {
    if (start_user < 0 || start_user >= graph->num_users) {
        printf("ID de usuario invalido!\n");
        return;
    }
    
    bool visited[MAX_USERS] = {false};
    printf("Explorando rede social de %s (DFS):\n", graph->users[start_user].name);
    dfs_util(graph, start_user, visited);
}

void suggest_friends(SocialGraph* graph, int user_id) {
    if (user_id < 0 || user_id >= graph->num_users) {
        printf("ID de usuario invalido!\n");
        return;
    }
    
    bool visited[MAX_USERS] = {false};
    int distance[MAX_USERS] = {0};
    Queue* q = create_queue();
    
    visited[user_id] = true;
    enqueue(q, user_id);
    
    printf("Sugestoes de amizade para %s:\n", graph->users[user_id].name);
    
    while (!is_empty(q)) {
        int current_user = dequeue(q);
        
        Node* temp = graph->users[current_user].friends;
        while (temp != NULL) {
            int neighbor = temp->user_id;
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                distance[neighbor] = distance[current_user] + 1;
                enqueue(q, neighbor);
                
                if (distance[neighbor] == 2) {
                    printf("- %s (amigo de %s)\n", 
                           graph->users[neighbor].name,
                           graph->users[current_user].name);
                }
            }
            temp = temp->next;
        }
    }
    free(q);
}

int main() {
    SocialGraph graph;
    initialize_graph(&graph);
    
    int alice = add_user(&graph, "Alice");
    int bob = add_user(&graph, "Bob");
    int carol = add_user(&graph, "Carol");
    int dave = add_user(&graph, "Dave");
    int eve = add_user(&graph, "Eve");
    
    add_friendship(&graph, alice, bob);
    add_friendship(&graph, alice, carol);
    add_friendship(&graph, bob, dave);
    add_friendship(&graph, carol, eve);
    
    show_friends(&graph, alice);
    
    bfs(&graph, alice);
    
    dfs(&graph, alice);
    
    suggest_friends(&graph, alice);
    
    return 0;
}