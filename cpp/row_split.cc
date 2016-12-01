#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<string> row_split(string &str, int col_num, int col_idx) {
  vector<string> rows;
  vector<string> back_rows;
  if (col_idx > col_num || col_num < 1 || col_idx < 1) {
    cout << "Invalid col_num or col_idx" << endl;
    return rows;
  }
  size_t begin_idx = -1;
  size_t end_idx = 0;
  size_t idx; // start idx of special column
  for (int i = 1; i < col_idx; i++) {
    end_idx = str.find_first_of(',', begin_idx + 1);
    rows.push_back(str.substr(begin_idx + 1, end_idx - begin_idx - 1));
    begin_idx = end_idx;
  }
  idx = begin_idx + 1;
  // end_idx = str.length() if there were no semicolon in the end
  end_idx = str.length() - 1;
  for (int i = col_num; i > col_idx; i--) {
    begin_idx = str.find_last_of(',', end_idx - 1);
    back_rows.push_back(str.substr(begin_idx + 1, end_idx - begin_idx - 1));
    end_idx = begin_idx;
  }
  string special = str.substr(idx, end_idx - idx);
  int comma_count = 0;
  for (size_t i = 0; i < special.length(); i++) {
    if (special[i] == ',')
      comma_count++;
  }
  char *buffer = new char[special.length() + comma_count + 1]();
  size_t j = special.length() + comma_count - 1;
  for (int i = special.length() - 1; i >= 0; i--) {
    buffer[j--] = special[i];
    if (special[i] == ',')
      buffer[j--] = '\\';
  }
  rows.push_back(string(buffer));
  delete[] buffer;
  for (int i = back_rows.size() - 1; i >= 0; i--)
    rows.push_back(back_rows[i]);
  return rows;
}

int main() {
  string str(",,e,\\,,f,g\\h,;");
  vector<string> res = row_split(str, 5, 2);
  cout << "row: " << str << endl;
  for (vector<string>::iterator ite = res.begin(); ite != res.end(); ite++)
    cout << *ite << endl;
  return 0;
}
