#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct BingoNumber {
  int row;
  int column;
  bool marked;
};

class BingoCard {
private:
  vector<int> rows;
  vector<int> columns;

  bool hasWon(vector<int> &nums) {
    for (size_t i = 0; i < nums.size(); i++) {
      if (nums[i] >= boardSize) {
        return true;
      }
    }
    return false;
  }

public:
  constexpr static int boardSize = 5;
  unordered_map<int, BingoNumber> board;

  BingoCard() {
    rows = {0, 0, 0, 0, 0};
    columns = {0, 0, 0, 0, 0};
  }

  bool hasWon() { return hasWon(rows) || hasWon(columns); }

  int getUnmarkedNumbersSum() {
    int sum = 0;
    for (auto &num : board) {
      if (!num.second.marked) {
        sum += num.first;
      }
    }
    return sum;
  }
  void mark(int number) {
    if (board.contains(number)) {
      auto &bn = board[number];
      bn.marked = true;
      rows[bn.row] = rows[bn.row] + 1;
      columns[bn.column]++;
    }
  }
};

class BingoReader {
private:
  ifstream input;

public:
  BingoReader(string path) { input.open(path); }
  ~BingoReader() { input.close(); }

  vector<int> getDrawnNumbers() {
    input.clear();
    input.seekg(0);
    string numbers;
    getline(input, numbers);
    istringstream tokenizer(numbers);
    string token;
    vector<int> drawnNumbers;
    while (getline(tokenizer, token, ',')) {
      drawnNumbers.push_back(stoi(token));
    }
    return drawnNumbers;
  }

  vector<BingoCard> getBingoCards() {
    input.clear();
    input.seekg(0);
    vector<BingoCard> cards;
    string numbers;
    getline(input, numbers); // skip drawn numbers

    while (getline(input, numbers)) {
      if (numbers == "") {
        continue;
      }
      auto card = BingoCard{};
      int n;
      for (int r = 0, c = 0; r < card.boardSize; r++, c = 0) {
        istringstream tokenizer(numbers);
        while (tokenizer >> n) {
          card.board[n] = {r, c, false};
          c++;
        }
        getline(input, numbers);
      }
      cards.push_back(card);
    }
    return cards;
  }
};

int main(int argc, char *argv[]) {
  auto path = string(argv[1]);
  BingoReader reader(path);
  auto numbers = reader.getDrawnNumbers();
  auto cards = reader.getBingoCards();
  for (auto &number : numbers) {
    for (auto it = cards.begin(); it != cards.end(); it++) {
      it->mark(number);
      if (it->hasWon()) {
        if (cards.size() > 1) {
          cards.erase(it);
          it--;
        } else {
          cout << "The loser takes it all: "
               << number * it->getUnmarkedNumbersSum();
          return 0;
        }
      }
    }
  }
  return 0;
}