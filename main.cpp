#include <SFML/Graphics.hpp>
#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <sstream>
using namespace std ;
using namespace sf ;

//#include "BFS_Grid.h"
//#include "DFS_Grid.h"
//#include "DP_Grid.h"

class TreeVisualizer {
private:
    struct Node {

        float x, y; // positions
        bool vis; // is visited node ?
        CircleShape node; // the node circle shape

        Node(float x, float y) : x(x), y(y), vis(false) {
            node.setPosition(x - 20, y - 20);
            node.setRadius(20);
            node.setFillColor(Color::White);
            node.setOutlineThickness(2);
            node.setOutlineColor(Color::Black);
        }
    };
    RenderWindow &dfs_tree_window;
    vector<Node> nodes;
    vector<vector<bool>> adj;

public:
    TreeVisualizer( RenderWindow& window) :  dfs_tree_window(window) {

    }
    void init() {
        // add nodes positions and edges of the tree
        nodes.emplace_back(300, 100); // 0
        nodes.emplace_back(200, 200); // 1
        nodes.emplace_back(300, 200);
        nodes.emplace_back(400, 200);
        nodes.emplace_back(250, 300);
        nodes.emplace_back(350, 300);
        nodes.emplace_back(200, 400);
        nodes.emplace_back(300, 400);
        nodes.emplace_back(450, 300);
        nodes.emplace_back(400, 400); // 9
        nodes.emplace_back(500, 400); // 10
        nodes.emplace_back(450, 500); // 11
        nodes.emplace_back(550, 500); // 12


        // resize the adj_list size based on number of nodes
        adj.resize(nodes.size(), vector<bool>(nodes.size(), false));

        // mark nodes in adj matrix
        adj[0][1] = adj[1][0] = true;
        adj[0][2] = adj[2][0] = true;
        adj[0][3] = adj[3][0] = true;
        adj[2][4] = adj[4][2] = true;
        adj[2][5] = adj[5][2] = true;
        adj[4][6] = adj[6][4] = true;
        adj[4][7] = adj[7][4] = true;
        adj[3][8] = adj[8][3] = true;
        adj[9][8] = adj[8][9] = true;
        adj[10][8] = adj[8][10] = true;
        adj[11][10] = adj[10][11] = true;
        adj[12][10] = adj[10][12] = true;
        drawTree();

    }

    void drawTree() {
        dfs_tree_window.clear();

        // draw edges lines between nodes O-----O
        for (int i = 0; i < nodes.size(); ++i) {
            for (int j = 0; j < nodes.size(); ++j) {
                if (adj[i][j]) {
                    sf::Vertex line[] = {
                            Vertex(Vector2f(nodes[i].x, nodes[i].y)), // line (edge) positions
                            Vertex(Vector2f(nodes[j].x, nodes[j].y))
                    };
                    dfs_tree_window.draw(line, 2, Lines);
                }
            }
        }

        // draw nodes
        for (const auto& node : nodes) {
            dfs_tree_window.draw(node.node);
        }
        dfs_tree_window.display();
    }

    void dfs(int parent) {
        stack<int> dfsStack;
        dfsStack.push(parent);

        while (!dfsStack.empty()) {
            int currentNode = dfsStack.top();
            dfsStack.pop();
            sleep(milliseconds(500)) ;
            if (!nodes[currentNode].vis) {
                nodes[currentNode].vis = true;
                nodes[currentNode].node.setFillColor(sf::Color::Green); // Change color when vis
                drawTree();

                // Push unvisited neighbors onto the dfsStack
                for (int i = nodes.size() - 1; i >= 0; --i) {
                    if (adj[currentNode][i] && !nodes[i].vis) {
                        dfsStack.push(i);
                    }
                }
            }
        }
    }
};


class DP_Visualization {
public:
    DP_Visualization(int rows, int cols, sf::RenderWindow& window) : rows(rows), cols(cols), window(window) {
        // Initialize the grid with zeros
        grid.resize(rows, std::vector<int>(cols, 0));

        // Initialize cell node
        cellShape.setSize(sf::Vector2f(100, 100));
        cellShape.setOutlineThickness(4);
        cellShape.setFillColor(Color(235,200,200));

        // Load a font for displaying text and Error handling
        if (!font.loadFromFile("D:\\Algo Visualizer\\Manrope.ttf")) {
            cerr << "Failed to load the font" << '\n';
        }
        // Initialize cell texts
        cellTexts.resize(rows * cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                cellTexts[i * cols + j].setFont(font);
                cellTexts[i * cols + j].setCharacterSize(50); // set the font size to 50
                cellTexts[i * cols + j].setFillColor(sf::Color::Black);
                cellTexts[i * cols + j].setPosition(j * 100 + 15, i * 100 + 15);
            }
        }
    }
    void handleEvent(sf::Event& event) {
        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            // Handle left mouse button click
            // Update cell values based on mouse position
            int x = event.mouseButton.x / cellShape.getSize().x;
            int y = event.mouseButton.y / cellShape.getSize().y;
            sf::sleep(sf::milliseconds(50));
            if (x >= 0 && x < cols && y >= 0 && y < rows) {
                grid[y][x]++;
            }
        }
        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
            // Handle right mouse button click
            // Update cell values based on mouse position
            int x = event.mouseButton.x / cellShape.getSize().x;
            int y = event.mouseButton.y / cellShape.getSize().y;
            if (x >= 0 && x < cols && y >= 0 && y < rows) {
                grid[y][x]--;
            }
        }
    }
    void render(bool bld= false ) {
        // Render the grid with text values
        window.clear();

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                cellShape.setPosition(j * cellShape.getSize().x, i * cellShape.getSize().y);
                // set cell color when build the dp table
                if(bld) cellShape.setFillColor(sf::Color(255 , 235 - grid[i][j] * 10, 235- grid[i][j] * 10));
                // convert the value to string and set it as text
                ostringstream oss;
                oss << grid[i][j];
                cellTexts[i * cols + j].setString(oss.str());
                window.draw(cellShape);
                window.draw(cellTexts[i * cols + j]);
            }
        }
        window.display();
    }
    void build() {
        // Implement dynamic programming algorithm here
        // You can use a separate class or function for the algorithm
        for (int i = 0; i < grid.size(); ++i) {
            for (int j = 0; j < grid[0].size(); ++j) {
                if ( i>0 && j==0 ) grid[i][j]+=grid[i-1][j];
                else if ( j>0 && i==0 ) grid[i][j]+=grid[i][j-1];
                else if (i>0 && j>0) grid[i][j]+= min(grid[i][j-1] , grid[i-1][j]);
                sf::sleep(sf::milliseconds(100));
                render(true) ;
            }
        }
    }
private:
    int rows, cols;
    vector<vector<int>> grid;
    RenderWindow& window;
    RectangleShape cellShape;
    vector<Text> cellTexts;
    Font font;
};


class DFSGrid {
public:
    DFSGrid(int rows, int cols, RenderWindow& window) : rows(rows), cols(cols), window(window) {
        visited.resize(rows, vector<int>(cols, 0));
        cellShape.setSize(Vector2f(70, 70));
        cellShape.setOutlineThickness(1);
    }
    void update(bool F = true) {
        // implementation of dfs using a stack
        stack<pair<int, int>> dfsStack;
        int startX = 0  ,startY = 0 ;
        int endX = 5  ,endY = 5 ;
        dfsStack.push({startX, startY}); // The start Node
        visited[endX][endY]=-1;
        visited[startX][startY]=1 ;
        vector<pair<int,int>>obstacles{
                {2,3},
                {0,3},
                {1,3},
                {4,3},
                {3,5},
                {9,1},
                {8,1},
        };
        for(auto &[obstX , obstY] : obstacles){
            visited[obstX][obstY]=3 ;
        }

        while (!dfsStack.empty() && F) {
            auto [row, col] = dfsStack.top();
            dfsStack.pop();
            // mark the cell as vis
            if (!visited[row][col])
                visited[row][col] = 2;
            if (row==endX and col==endY) break;

            // Add neighboring cells to the stack (up, down, left, right) **you can customize it to be diagonal
            const vector<pair<int, int>> directions = {
                     {0, 1}
                    ,{0, -1}
                    ,{1, 0}
                    ,{-1, 0}};

            for (const auto& dir : directions) {
                int newRow = row + dir.first;
                int newCol = col + dir.second;
                // check if the Cell is Valid and not vis
                if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols && visited[newRow][newCol] <= 0) {
                    dfsStack.push({newRow, newCol});
                }
            }

            // sleep for a short duration to show the process slowly
            sleep(milliseconds(30));
            render();
            window.display();
        }
    }
    void render() {
        window.clear();

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                cellShape.setPosition(j * cellShape.getSize().x, i * cellShape.getSize().y);

                if (visited[i][j]==1) {
                    // Visited cells are colored red
                    cellShape.setFillColor(sf::Color::Red);
                }else if (visited[i][j]==2) {
                    // Visited cells are colored red
                    cellShape.setFillColor(sf::Color::Blue);
                }else if (visited[i][j]==3) {
                    // Visited cells are colored red
                    cellShape.setFillColor(sf::Color::Black);
                } else if (visited[i][j]==-1) {
                    // Visited cells are colored red
                    cellShape.setFillColor(sf::Color::Red);
                } else {
                    // Unvisited cells are colored Cyan
                    cellShape.setFillColor(sf::Color(200 , 150 , 100));
                }

                window.draw(cellShape);
            }
        }
    }
private:
    int rows, cols;
    vector<std::vector<int>> visited;
    RenderWindow& window;
    RectangleShape cellShape;
};

class BFSGrid {
public:
    BFSGrid(int rows, int cols, RenderWindow& window) : rows(rows), cols(cols), window(window) {
        visited.resize(rows, vector<int>(cols, 0));
        cellShape.setSize(Vector2f(70, 70));
        cellShape.setOutlineThickness(1);
    }
    void update() {
        // implementation of dfs using a stack
        queue<pair<int, int>> bfsQueue;
        int startX = 5  ,startY = 5 ;
        int endX = 8  ,endY = 9 ;
        bfsQueue.push({startX, startY}); // The start Node
        visited[endX][endY]=-1;
        visited[startX][startY]=1 ;
        vector<pair<int,int>>obstacles{
                {7,7},
                {6,6},
                {6,8},
                {8,7},
                {9,7},
        };
        for(auto &[obstX , obstY] : obstacles){
            visited[obstX][obstY]=3 ;
        }
        bool F = true ;
        while (!bfsQueue.empty() && F ) {
            auto [row, col] = bfsQueue.front();
            bfsQueue.pop();
            // mark the cell as vis
            if (!visited[row][col])
                visited[row][col] = 2;

            // Add neighboring cells to the Queue (up, down, left, right) **you can customize it to be diagonal
            const vector<pair<int, int>> directions = {
                     {1, 0}
                    ,{0, 1}
                    ,{0, -1}
                    ,{-1, 0} };

            for (const auto& dir : directions) {
                int newRow = row + dir.first;
                int newCol = col + dir.second;
                if (newRow==endX and newCol==endY) F=0 ;

                // check if the Cell is Valid and not vis
                if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols && visited[newRow][newCol] <= 0) {
                    bfsQueue.push({newRow, newCol});
                }
            }

            // sleep for a short duration to show the process slowly
            sleep(milliseconds(5));
            render();
            window.display();
        }
    }
    void render() {
        window.clear();

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                cellShape.setPosition(j * cellShape.getSize().x, i * cellShape.getSize().y);

                if (visited[i][j]==1) {
                    // Visited cells are colored red
                    cellShape.setFillColor(sf::Color::Red);
                }else if (visited[i][j]==2) {
                    // Visited cells are colored red
                    cellShape.setFillColor(sf::Color::Blue);
                }else if (visited[i][j]==3) {
                    // Visited cells are colored red
                    cellShape.setFillColor(sf::Color::Black);
                } else if (visited[i][j]==-1) {
                    // Visited cells are colored red
                    cellShape.setFillColor(sf::Color::Red);
                } else {
                    // Unvisited cells are colored Yellow
                    cellShape.setFillColor(sf::Color(205 , 150 , 200));
                }
                window.draw(cellShape);
            }
        }
    }

private:
    int rows, cols;
    vector<std::vector<int>> visited;
    RenderWindow& window;
    RectangleShape cellShape;
};


int main() {
    // Tree dfs visualizer

    RenderWindow dfs_tree_window(VideoMode(800, 600), "DFS Tree Visualization") ;
    TreeVisualizer dfs_tree(dfs_tree_window);
    dfs_tree.init();
    while (dfs_tree_window.isOpen()) {
        Event event;
        while (dfs_tree_window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                dfs_tree_window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                dfs_tree.dfs(0);
            }
        }
        dfs_tree.drawTree() ;
    }

    // DP  min-cost path in Grid

    RenderWindow DP_window(VideoMode(500, 500), "DP Visualization");
    DP_Visualization dp_grid(5, 5, DP_window);
    while (DP_window.isOpen()) {
        Event event;
        while (DP_window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                DP_window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                dp_grid.build();
            }
            dp_grid.handleEvent(event);
        }
        dp_grid.render();
    }

    // DFS in Grid with obstacles
    RenderWindow DFS_window(sf::VideoMode(700, 700), "DFS Visualization");
    DFSGrid dfs_Grid(10, 10, DFS_window);
    while (DFS_window.isOpen()) {
        Event event;
        while (DFS_window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                DFS_window.close();
            }
        }
        dfs_Grid.update() ;
    }

    // BFS in Grid with obstacles
    RenderWindow BFS_window(VideoMode(700, 700), "BFS Visualization");
    BFSGrid bfs_grid(10, 10, BFS_window);
    while (BFS_window.isOpen()) {
        Event event;
        while (BFS_window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                BFS_window.close();
            }
        }
        bfs_grid.update() ;
    }
    return 0;
}
