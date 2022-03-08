#include<iostream>
const int maxn = 100;
int main(){
    int ans;
    int a[maxn],b[maxn];
    for(int i=0;i<maxn;i++){
        for (int j = 0; j < maxn; j++)
        {
            int k = 0;
            while(a[i+k]==b[j+k]) k++;
            ans = std::max(ans,k);
        }
    }
}