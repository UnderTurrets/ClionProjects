#include<vector>
#include<iostream>
#include<memory>
#include <myAlgorithm.h>
using  namespace  std;

class Solution {
public:
    bool find_repeat(vector<char> v){
        vector<char> nums;bool ret =1;int x=0;
        for(int i=0;i<v.size();i++){
            if(v[i]>='0'&&v[i]<='9'){
                nums.push_back(v[i]);
            }
        }
        sort(nums.begin(),nums.end());
        if(!nums.empty()) {
            for (int i = 0; i < nums.size() - 1; i++) {
                if (nums[i] == nums[i + 1]) {
                    ret = 0;
                    break;
                }

            }
        }
        return ret;
    }
    bool isValidSudoku(vector<vector<char>>& board) {
        bool ret=1;
        for(int i=0;i<board.size();i+=3){
            for(int j=0;j<board[0].size();j+=3){
                ret=find_repeat({board[i][j],board[i][j+1],board[i][j+2],board[i+1][j+1],board[i+1][j+2],board[i+1][j],board[i+2][j],board[i][j+1],board[i][j+2],});
            }
            if(ret==0){
                break;
            }
        }
        for(int i=0;i<board.size();i++){
            ret=find_repeat({board[0][i],board[1][i],board[2][i],board[3][i],board[4][i],board[5][i],board[6][i],board[7][i],board[8][i],});
            if(ret==0){
                break;
            }
            ret=find_repeat({board[i][0],board[i][1],board[i][2],board[i][3],board[i][4],board[i][5],board[i][6],board[i][7],board[i][8],});
            if(ret==0){
                break;
            }


        }







        return ret;
    }
};



template<class T>
void print_forward_vector (const vector<T>v ){
    auto it =v.begin();
    for(it;it!=v.end();it++){
        cout<<*it<<ends;
    }
    cout<<endl;
}
using namespace std;


int main () {
//    vector<int> nums1 = {1, 2, 2, 1};
//    vector<int> nums2 = {2,};

//    vector<int> ret;
//    sort(nums1.begin(),nums1.end());
//    sort(nums2.begin(),nums2.end());
//    vector<int>::iterator it=nums2.begin();
//    for (int i =0;i<nums1.size();i++){
//        for ( it;it!=nums2.end();it++){
//            if(nums1[i]==(*it)){
//                ret.insert(ret.end(),nums1[i]);
//                break;
//            }
//
//
//        }
//    }

//    vector<int> ret;
//    int j = 0;
//    sort(nums1.begin(), nums1.end());
//    sort(nums2.begin(), nums2.end());
//    for (int i = 0; i < nums1.size(); i++) {
//        for (; j < nums2.size(); j++) {
//            if (nums1[i] = nums2[j]) {
//                ret.insert(ret.end(), nums1[i]);
//                j++;
//                break;
//            }
//            if (nums1[i] < nums2[j]) {
//                break;
//            }
//
//        }
//    }
//    print_forward_vector(ret);


//    vector<vector<int>> mat = {{1, 2},
//                               {3, 4}};
//    int r = 2, c = 2;
//    if ((mat.size() * mat[0].size()) == (r * c)) {
//        vector<vector<int>> des;
//        vector<int> line;
//        int k = 0;
//        for (int i = 0; i < mat.size(); i++) {
//            for (int j = 0; j < mat[i].size(); j++) {
//                line.insert(line.end(), mat[i][j]);
//
//
//            }
//        }
//        for (int i = 0; i < r; i++) {
//            for (int j = 0; j < c; j++) {
//                des[i][j] = line[k];
//                k++;
//            }
//        }


//
//    } else {
//
//    }

//   vector<vector<char>>board={{'.','4','.','.','.','.','.','.','.',},{'.','.','4','.','.','.','.','.','.',},{'.','.','.','.','.','.','.','.','.',},{'.','.','.','.','.','.','.','.','.',},{'.','.','.','.','.','.','.','.','.',},{'.','.','.','.','.','.','.','.','.',},{'.','.','.','.','.','.','.','.','.',},{'.','.','.','.','.','.','.','.','.',},{'.','.','.','.','.','.','.','.','.',},};
//    bool ret=1;Solution ex;bool x=1;
//    for(int i=1;i<board.size();i+=3){
//        for(int j=1;j<board.size();j+=3){
//            x=ex.find_repeat({board[i-1][j-1],board[i-1][j],board[i-1][j+1],board[i][j-1],board[i][j],board[i][j+1],board[i+1][j-1],board[i+1][j],board[i+1][j+1]});
//        }
//        if(x==0){
//            ret=0;
//            break;
//        }
//    }
//    for(int i=0;i<board.size();i++){
//        ret=ex.find_repeat({board[0][i],board[1][i],board[2][i],board[3][i],board[4][i],board[5][i],board[6][i],board[7][i],board[8][i],});
//        if(x==0){
//            ret=0;
//            break;
//        }
//        ret=ex.find_repeat({board[i][0],board[i][1],board[i][2],board[i][3],board[i][4],board[i][5],board[i][6],board[i][7],board[i][8],});
//        if(x==0){
//            ret=0;
//            break;
//        }
//
//
//    }
//   cout<<ret<<endl;


//       string s="z";
//        int ret=-1;
//        for(int i=0;i<s.size();i++){
//            int x=s.find(s[i]);
//            int y=s.rfind(s[i]);
//            if(x==y) {
//                ret=i;
//                break;
//            }
//
//        }
//        cout<< ret<<endl;

//    string ransomNote="ab";
//    string magazine="a";
//    bool  ret=1;
//    for(int i =0;i<ransomNote.size();i++){
//        int x=magazine.find(ransomNote[i]);
//        if(x!=-1){
//            magazine.erase(x,1);
//        }else if(x==-1){
//            ret=0;
//            break;
//        }
//    }
//    cout<<ret<<endl;

    BinTree<int> origin;
    origin.val=999;
    BinTree<int> second_one;
    origin.left=&second_one;
    second_one.val=998;
    BinTree<int> second_two;
    origin.right=&second_two;
    second_two.val=1000;
    BinTree<int>origin2 (origin);
    origin.insert(1,&origin);
    origin.Travesal_levelorder();


//vector<int> vector1={1,2,3,4,5,6,7,8,9,};
////MinHeap<BinTree<int>>minHeap1(10);
////minHeap1.insert(origin);
////minHeap1.insert(second_one);
////minHeap1.insert(second_two);
////    HuffMan(&minHeap1);
//MinHeap<int>minHeap2(vector1);
//minHeap2.insert(999);
//    minHeap2.insert(99);
//    minHeap2.insert(88);
//
//    minHeap2.insert(77);
//
//    minHeap2.insert(2);
//minHeap2.print_heap();
//MinHeap<int>minHeap3=minHeap2;
//minHeap3.print_heap();
}