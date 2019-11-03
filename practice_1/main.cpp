#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>
#include <queue>
#include <limits>
#include <gtest/gtest.h>


const char empty_word = '!';
struct Node{
 public:
  Node(){
    is_term_ = true;
  }

  Node(char c, Node* next_node){
    links_[next_node] = c;
    is_term_ = false;
  }

  Node(Node* first_root, Node* second_root){
    links_[first_root] = empty_word;
    links_[second_root] = empty_word;
    is_term_ = false;
  }

  std::map<Node*, char> links_;
  bool is_term_;
};

class Automaton{
 public:
  Automaton(char c){
    term_ = new Node();
    root_ = new Node(c, term_);
    nodes_in_automaton.push_back(root_);
    nodes_in_automaton.push_back(term_);
  }

  Automaton(Automaton* A, Automaton* B, size_t type_of_operations){
    if (type_of_operations == 0){
      root_ = new Node(A->root_, B->root_);
      term_ = new Node();
      A->term_->is_term_ = false;
      B->term_->is_term_ = false;
      A->term_->links_[term_] = empty_word;
      B->term_->links_[term_] = empty_word;

      nodes_in_automaton.push_back(root_);
      nodes_in_automaton.push_back(term_);

      for (auto node:A->nodes_in_automaton){
        nodes_in_automaton.push_back(node);
      }
      for (auto node:B->nodes_in_automaton){
        nodes_in_automaton.push_back(node);
      }

    } else if (type_of_operations == 1) {
      root_ = new Node(empty_word, A->root_);
      A->term_->links_[B->root_] = empty_word;
      A->term_->is_term_ = false;
      term_ = B->term_;

      nodes_in_automaton.push_back(root_);
      for (auto node:A->nodes_in_automaton){
        nodes_in_automaton.push_back(node);
      }
      for (auto node:B->nodes_in_automaton){
        nodes_in_automaton.push_back(node);
      }
    }
  }

  Automaton(Automaton* A){
    root_ = new Node();
    term_ = root_;
    root_->links_[A->root_] = empty_word;
    A->term_->links_[term_] = empty_word;
    A->term_->is_term_ = false;
    nodes_in_automaton.push_back(root_);
    for (auto node:A->nodes_in_automaton){
      nodes_in_automaton.push_back(node);
    }
  }

  void find_distant_from_root(){
    for (auto node:nodes_in_automaton){
      distant_from_root_[node] = -1;
    }
    distant_from_root_[root_] = 0;
    std::queue<Node*> Q;
    Q.push(root_);
    while (!Q.empty()){
      Node* cur_node = Q.front();
      Q.pop();
      for (auto pair: cur_node->links_){
        if (distant_from_root_[pair.first] == -1){
          if (pair.second == empty_word){
            distant_from_root_[pair.first] = distant_from_root_[cur_node];
          } else {
            distant_from_root_[pair.first] = distant_from_root_[cur_node] + 1;
          }Q.push(pair.first);
        }
      }
    }
  }

  int find_distant_to_term(Node* node){
    std::map<Node*, int> distant;
    for (auto elem:nodes_in_automaton){
      distant[elem] = -1;
    }
    distant[node] = 0;
    std::queue<Node*> Q;
    Q.push(node);
    while (!Q.empty()){
      Node* cur_node = Q.front();
      Q.pop();
      for (auto pair: cur_node->links_){
        if (distant[pair.first] == -1){
          if (pair.second == empty_word){
            distant[pair.first] = distant[cur_node];
          } else {
            distant[pair.first] = distant[cur_node] + 1;
          }
          Q.push(pair.first);
        }
      }
    }
    return distant[term_];
  }


  void find_each_distant_to_term(){
    for (auto node:nodes_in_automaton){
      distant_to_term_[node] = find_distant_to_term(node);
    }
  }

  void find(Node* node, char letter, int k, std::vector<Node*>& ends){

    std::queue<std::pair<Node*, int>> Q;
    Q.push(std::make_pair(node, 0));
    while (!Q.empty()){
      auto cur = Q.front();
      Node* cur_node = cur.first;
      if (cur.second == k){
        ends.push_back(cur_node);
      }
      Q.pop();
      for (auto pair: cur_node->links_){
        if (pair.second == letter && cur.second < k) {
          Q.push(std::make_pair(pair.first, cur.second + 1));
        } else if (pair.second == empty_word){
          Q.push(std::make_pair(pair.first, cur.second));
        }
      }
    }
  }

  int find_subword(char letter, int k){
    int min = std::numeric_limits<int>::max();
    for (auto node:nodes_in_automaton){
      std::vector<Node*> ends;
      find(node, letter, k, ends);
      if (!ends.empty()){
        for (Node* end:ends){
          if (min > distant_from_root_[node] + k + distant_to_term_[end]){
            min = distant_from_root_[node] + k + distant_to_term_[end];
          }
        }
      }
    }
    return min;
  }

 public:
  std::vector<Node*> nodes_in_automaton;
  std::map<Node*, int> distant_from_root_;
  std::map<Node*, int> distant_to_term_;
  Node* root_;
  Node* term_;

};

int find_answer(std::string s, char letter, int k){
  std::stack<Automaton> stack;
  for (auto c : s){
    if (c == 'a' || c == 'b' || c == 'c'){
      stack.push(Automaton(c));
    } else if (c == '+') {
      if (stack.size() < 2){
        std::cout<<"ERROR";
        return -1;
      }
      Automaton A = stack.top();
      stack.pop();
      Automaton B = stack.top();
      stack.pop();
      stack.push(Automaton(&A, &B, 0));
    } else if (c == '.'){
      if (stack.size() < 2){
        std::cout<<"ERROR";
        return -1;
      }
      Automaton A = stack.top();
      stack.pop();
      Automaton B = stack.top();
      stack.pop();
      stack.push(Automaton(&B, &A, 1));
    } else if (c == '*'){
      if (stack.size() < 1){
        std::cout<<"ERROR";
        return -1;
      }
      Automaton A = stack.top();
      stack.pop();
      stack.push(Automaton(&A));
    } else {
      std::cout<<"ERROR";
      return -1;
    }
  }
  if (stack.size() != 1){
    std::cout<<"ERROR";
    return -1;
  }

  Automaton A = stack.top();
  A.find_distant_from_root();
  A.find_each_distant_to_term();
  int min = A.find_subword(letter, k);
  std::cout<<min;
  return min;
}

TEST(group_1, test_1){
  std::string s = "acb..bab.c.*.ab.ba.+.+*a.";
  EXPECT_EQ(find_answer(s, 'b', 3), 7);
}

TEST(group_1, test_2){
  std::string s = "ab+c.aba.*.bac.+.+*";
  EXPECT_EQ(find_answer(s, 'b', 2), 4);
}

TEST(group_1, test_3){
  std::string s = "ac.*c.";
  EXPECT_EQ(find_answer(s, 'c', 2), 3);
}

TEST(group_2, test_1){
  std::string s = "abc..*";
  EXPECT_EQ(find_answer(s, 'c', 2), 2147483647);
}

TEST(group_2, test_2){
  std::string s = "abc..a.a+";
  EXPECT_EQ(find_answer(s, 'b', 2), 2147483647);
}

TEST(group_3, test_1){
  std::string s = "abc...";
  EXPECT_EQ(find_answer(s, 'b', 2), -1);
}

TEST(group_3, test_2){
  std::string s = "aab+++";
  EXPECT_EQ(find_answer(s, 'b', 2), -1);
}

TEST(group_3, test_3){
  std::string s = "*+*";
  EXPECT_EQ(find_answer(s, 'b', 2), -1);
}

TEST(group_3, test_4){
  std::string s = "aacccc*";
  EXPECT_EQ(find_answer(s, 'b', 2), -1);
}

TEST(group_3, test_5){
  std::string s = "ght";
  EXPECT_EQ(find_answer(s, 'b', 2), -1);
}

int main(int argc, char **argv){
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}