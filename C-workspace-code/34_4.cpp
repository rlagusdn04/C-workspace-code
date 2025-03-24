#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;

char arr[10001][501];
int visited[10001][501];
int A, B, cnt;
int dx[3] = { 0,1,-1 };
int dy[3] = {1,1,1};
queue<pair<int, int>> q;

void bfs(int x, int y){
    q.push({x,y});
    arr[x][y] = '.'; //방문처리
    
    while (!q.empty()) { //연결된 노드 탐색
        int nx = q.front().first;
        int ny = q.front().second;
        
        q.pop();
        for (int i = 0; i < 3; i++)
        {
            nx += dx[i];
            ny += dy[i];
            cout << dx[i] << dy[i];
            cout << "check" << nx << ny << ' ';
            
            
            if (nx < A && ny < B && arr[nx][ny] == 'x') {
                q.push({ nx,ny });
                cout << "ok" << ' ';
                arr[nx][ny] = '.';
                
                cnt++;
            }
            else {
                nx -= dx[i];
                ny -= dy[i];
            }
        }
        
    }
}


int main(void)
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    cin >> A >> B;
    for (int i = 0; i < A; i++) {
       
        for (int j = 0; j < B; j++) {
            cin >> arr[i][j];
        }
    }
    
    int result = 1;

    for (int i = 0; i < A; i++) {
        for (int j = 0; j < B; j++) {
            cnt = 1;
            if (arr[i][j] == 'x') {
                cout << "check" << i << j << ' ';
                bfs(i, j);
                cout << endl;
            }
            result = max(result, cnt);
        }
    }
    
    cout << result;
}